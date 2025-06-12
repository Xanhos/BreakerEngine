#include <GL/glew.h>
#include <SFML/Graphics.h>
#include <SFML/OpenGL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_PARTICLES 100000
#define VERTICES_PER_PARTICLE 32 // Maximum pour un cercle

typedef struct {
	float position[3];
	float velocity[3];
	float life;
	float size;
	float rotation;
	float color[4];
} Particle;

typedef struct {
	GLuint updateProgram;
	GLuint renderProgram;
	GLuint particleVBO;
	GLuint particleVAO;
	GLuint indexVBO;
	GLuint transformFeedbackVBO;
	int particleCount;
	int sidesCount;
	float* circleVertices;
	GLuint* circleIndices;
} ParticleSystem;

const char* updateVertexShaderWithGS = "#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 velocity;\n"
"layout(location = 2) in float life;\n"
"layout(location = 3) in float size;\n"
"layout(location = 4) in float rotation;\n"
"layout(location = 5) in vec4 color;\n"
"\n"
"out vec3 vs_position;\n"
"out vec3 vs_velocity;\n"
"out float vs_life;\n"
"out float vs_size;\n"
"out float vs_rotation;\n"
"out vec4 vs_color;\n"
"\n"
"uniform float deltaTime;\n"
"uniform vec3 gravity;\n"
"uniform float damping;\n"
"\n"
"void main() {\n"
"    if (life <= 0.0) {\n"
"        vs_position = position;\n"
"        vs_velocity = velocity;\n"
"        vs_life = life;\n"
"        vs_size = size;\n"
"        vs_rotation = rotation;\n"
"        vs_color = color;\n"
"        return;\n"
"    }\n"
"    \n"
"    vs_velocity = velocity + gravity * deltaTime;\n"
"    vs_velocity *= damping;\n"
"    vs_position = position + vs_velocity * deltaTime;\n"
"    vs_life = life - deltaTime;\n"
"    vs_size = size;\n"
"    vs_rotation = rotation + deltaTime * 2.0;\n"
"    \n"
"    float alpha = clamp(vs_life / 3.0, 0.0, 1.0);\n"
"    vs_color = vec4(color.rgb, alpha);\n"
"}\n";

const char* updateGeometryShader = "#version 330 core\n"
"layout(points) in;\n"
"layout(points, max_vertices = 1) out;\n"
"\n"
"in vec3 vs_position[];\n"
"in vec3 vs_velocity[];\n"
"in float vs_life[];\n"
"in float vs_size[];\n"
"in float vs_rotation[];\n"
"in vec4 vs_color[];\n"
"\n"
"out vec3 out_position;\n"
"out vec3 out_velocity;\n"
"out float out_life;\n"
"out float out_size;\n"
"out float out_rotation;\n"
"out vec4 out_color;\n"
"\n"
"void main() {\n"
"    if (vs_life[0] > 0.0) {\n"
"        out_position = vs_position[0];\n"
"        out_velocity = vs_velocity[0];\n"
"        out_life = vs_life[0];\n"
"        out_size = vs_size[0];\n"
"        out_rotation = vs_rotation[0];\n"
"        out_color = vs_color[0];\n"
"        EmitVertex();\n"
"    }\n"
"}\n";

// Vertex shader pour le rendu
const char* renderVertexShader = "#version 330 core\n"
"layout(location = 0) in vec3 particlePos;\n"
"layout(location = 1) in vec3 velocity;\n"
"layout(location = 2) in float life;\n"
"layout(location = 3) in float size;\n"
"layout(location = 4) in float rotation;\n"
"layout(location = 5) in vec4 color;\n"
"layout(location = 6) in vec2 vertexPos;\n"
"\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"\n"
"out vec4 fragColor;\n"
"\n"
"void main() {\n"
"    if (life <= 0.0) {\n"
"        gl_Position = vec4(-10.0, -10.0, -10.0, 1.0);\n"
"        fragColor = vec4(0.0);\n"
"        return;\n"
"    }\n"
"    \n"
"    // Rotation matrix\n"
"    float c = cos(rotation);\n"
"    float s = sin(rotation);\n"
"    mat2 rotMat = mat2(c, -s, s, c);\n"
"    \n"
"    // Transform vertex\n"
"    vec2 rotatedVertex = rotMat * vertexPos * size;\n"
"    vec3 worldPos = particlePos + vec3(rotatedVertex, 0.0);\n"
"    \n"
"    gl_Position = projection * view * vec4(worldPos, 1.0);\n"
"    fragColor = color;\n"
"}\n";

// Fragment shader pour le rendu
const char* renderFragmentShader = "#version 330 core\n"
"in vec4 fragColor;\n"
"out vec4 outColor;\n"
"uniform sampler2D texture;\n"
"uniform bool shouldUseTexture;\n"
"\n"
"void main() {\n"
"    outColor = fragColor;\n"
"}\n";

typedef struct {
	int aliveCount;
	int deadIndices[MAX_PARTICLES];
	int deadCount;
} ParticleStats;

ParticleStats checkParticleStatus(ParticleSystem* ps) {
	ParticleStats stats = { 0 };

	// Lire les données du GPU
	glBindBuffer(GL_ARRAY_BUFFER, ps->particleVBO);
	Particle* particles = (Particle*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

	if (particles) {
		for (int i = 0; i < ps->particleCount; i++) {
			if (particles[i].life <= 0.0f) {
				stats.deadIndices[stats.deadCount++] = i;
			}
			else {
				stats.aliveCount++;
			}
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	return stats;
}

void compactParticleArray(ParticleSystem* ps) {
	ParticleStats stats = checkParticleStatus(ps);

	if (stats.deadCount > 0) {
		// Lire toutes les particules
		glBindBuffer(GL_ARRAY_BUFFER, ps->particleVBO);
		Particle* particles = (Particle*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

		if (particles) {
			int writeIndex = 0;

			// Compacter en déplaçant les particules vivantes
			for (int readIndex = 0; readIndex < ps->particleCount; readIndex++) {
				if (particles[readIndex].life > 0.0f) {
					if (writeIndex != readIndex) {
						particles[writeIndex] = particles[readIndex];
					}
					writeIndex++;
				}
			}

			ps->particleCount = writeIndex;
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
	}
}

GLuint compileShader(const char* source, GLenum type) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("Shader compilation failed: %s\n", infoLog);
	}

	return shader;
}

GLuint createShaderProgram(const char* vertexSource, const char* geometrySource, const char* fragmentSource, const char** varyings, int varyingCount) {
	GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;

	if (geometrySource)
	{
		geometryShader = compileShader(geometrySource, GL_GEOMETRY_SHADER);
	}

	if (fragmentSource) {
		fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	if (geometryShader) {
		glAttachShader(program, geometryShader);
	}
	if (fragmentShader) {
		glAttachShader(program, fragmentShader);
	}



	// Transform feedback setup
	if (varyings && varyingCount > 0) {
		glTransformFeedbackVaryings(program, varyingCount, varyings, GL_INTERLEAVED_ATTRIBS);
	}

	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("Program linking failed: %s\n", infoLog);
	}

	glDeleteShader(vertexShader);
	if (geometryShader) {
		glDeleteShader(geometryShader);
	}
	if (fragmentShader) {
		glDeleteShader(fragmentShader);
	}

	return program;
}

void generatePolygonGeometry(ParticleSystem* ps, int sides) {
	ps->sidesCount = sides;

	// Free previous data
	if (ps->circleVertices) free(ps->circleVertices);
	if (ps->circleIndices) free(ps->circleIndices);

	// Generate vertices for polygon
	ps->circleVertices = malloc(sizeof(float) * (sides + 1) * 2); // +1 for center
	ps->circleIndices = malloc(sizeof(GLuint) * sides * 3);

	// Center point
	ps->circleVertices[0] = 0.0f;
	ps->circleVertices[1] = 0.0f;

	// Generate vertices around circle
	for (int i = 0; i < sides; i++) {
		float angle = 2.0f * M_PI * i / sides;
		ps->circleVertices[(i + 1) * 2] = cos(angle);
		ps->circleVertices[(i + 1) * 2 + 1] = sin(angle);
	}

	// Generate indices for triangles
	for (int i = 0; i < sides; i++) {
		ps->circleIndices[i * 3] = 0; // Center
		ps->circleIndices[i * 3 + 1] = i + 1;
		ps->circleIndices[i * 3 + 2] = (i + 1) % sides + 1;
	}
}

void initParticleSystem(ParticleSystem* ps, int sides) {
	ps->particleCount = 0;

	// Generate polygon geometry
	generatePolygonGeometry(ps, sides);

	// Create shaders
	const char* varyings[] = { "out_position", "out_velocity", "out_life", "out_size", "out_rotation", "out_color" };
	ps->updateProgram = createShaderProgram(updateVertexShaderWithGS, updateGeometryShader, NULL, varyings, 6);
	ps->renderProgram = createShaderProgram(renderVertexShader, NULL, renderFragmentShader, NULL, 0);

	// Create buffers
	glGenBuffers(1, &ps->particleVBO);
	glGenBuffers(1, &ps->transformFeedbackVBO);
	glGenBuffers(1, &ps->indexVBO);
	glGenVertexArrays(1, &ps->particleVAO);

	// Setup particle buffer
	glBindBuffer(GL_ARRAY_BUFFER, ps->particleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * MAX_PARTICLES, NULL, GL_DYNAMIC_DRAW);

	// Setup transform feedback buffer
	glBindBuffer(GL_ARRAY_BUFFER, ps->transformFeedbackVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * MAX_PARTICLES, NULL, GL_DYNAMIC_DRAW);

	// Setup polygon indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ps->indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * sides * 3, ps->circleIndices, GL_STATIC_DRAW);

	// Setup VAO for rendering
	glBindVertexArray(ps->particleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, ps->particleVBO);

	// Particle attributes (instanced)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 1);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, velocity));
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, life));
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, size));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, rotation));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);

	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);

	// Create separate VBO for polygon vertices
	GLuint polygonVBO;
	glGenBuffers(1, &polygonVBO);
	glBindBuffer(GL_ARRAY_BUFFER, polygonVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (sides + 1) * 2, ps->circleVertices, GL_STATIC_DRAW);

	// Polygon vertex positions (per vertex, not instanced)
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(6);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ps->indexVBO);

	glBindVertexArray(0);
}

void addParticle(ParticleSystem* ps, float x, float y, float z) {
	if (ps->particleCount >= MAX_PARTICLES) return;

	Particle particle;
	particle.position[0] = x;
	particle.position[1] = y;
	particle.position[2] = z;

	// Random velocity
	particle.velocity[0] = ((float)rand() / RAND_MAX - 0.5f) * 200.0f;
	particle.velocity[1] = ((float)rand() / RAND_MAX - 0.5f) * 200.0f;
	particle.velocity[2] = 0.0f;

	particle.life = 5.0f + ((float)rand() / RAND_MAX) * 3.0f;
	particle.size = 10.0f + ((float)rand() / RAND_MAX) * 20.0f;
	particle.rotation = 0.0f;

	// Random color
	particle.color[0] = (float)rand() / RAND_MAX;
	particle.color[1] = (float)rand() / RAND_MAX;
	particle.color[2] = (float)rand() / RAND_MAX;
	particle.color[3] = 1.0f;

	// Upload to GPU
	glBindBuffer(GL_ARRAY_BUFFER, ps->particleVBO);
	glBufferSubData(GL_ARRAY_BUFFER, ps->particleCount * sizeof(Particle), sizeof(Particle), &particle);

	ps->particleCount++;
}

void updateParticles(ParticleSystem* ps, float deltaTime) {
	if (ps->particleCount == 0) return;

	//compactParticleArray(ps);
	glUseProgram(ps->updateProgram);

	// Set uniforms 
	glUniform1f(glGetUniformLocation(ps->updateProgram, "deltaTime"), deltaTime);
	glUniform3f(glGetUniformLocation(ps->updateProgram, "gravity"), 0.0f, 0.f, 0.0f);
	glUniform1f(glGetUniformLocation(ps->updateProgram, "damping"), 0.99f);

	// Bind current particle data
	glBindBuffer(GL_ARRAY_BUFFER, ps->particleVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, velocity));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, life));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, size));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, rotation));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
	glEnableVertexAttribArray(5);

	//// Bind transform feedback buffer
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, ps->transformFeedbackVBO);

	//// Disable rasterizer
	glEnable(GL_RASTERIZER_DISCARD);

	ps->particleCount = getActiveParticleCount(ps);

	// Re-enable rasterizer
	glDisable(GL_RASTERIZER_DISCARD);


	// Swap buffers
	GLuint temp = ps->particleVBO;
	ps->particleVBO = ps->transformFeedbackVBO;
	ps->transformFeedbackVBO = temp;
}

void renderParticles(ParticleSystem* ps, float* projection, float* view) {
	if (ps->particleCount == 0) return;

	glUseProgram(ps->renderProgram);

	// Set matrices
	glUniformMatrix4fv(glGetUniformLocation(ps->renderProgram, "projection"), 1, GL_FALSE, projection);
	glUniformMatrix4fv(glGetUniformLocation(ps->renderProgram, "view"), 1, GL_FALSE, view);

	glBindVertexArray(ps->particleVAO);

	// Update particle buffer binding
	glBindBuffer(GL_ARRAY_BUFFER, ps->particleVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, velocity));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, life));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, size));
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, rotation));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw instanced polygons
	glDrawElementsInstanced(GL_TRIANGLES, ps->sidesCount * 3, GL_UNSIGNED_INT, 0, ps->particleCount);

	glDisable(GL_BLEND);
	glBindVertexArray(0);
}

void createIdentityMatrix(float* matrix) {
	memset(matrix, 0, 16 * sizeof(float));
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
}
#undef near;
#undef far
;
void createOrthoMatrix(float* matrix, float left, float right, float bottom, float top, float near, float far) {
	memset(matrix, 0, 16 * sizeof(float));
	matrix[0] = 2.0f / (right - left);
	matrix[5] = 2.0f / (top - bottom);
	matrix[10] = -2.0f / (far - near);
	matrix[12] = -(right + left) / (right - left);
	matrix[13] = -(top + bottom) / (top - bottom);
	matrix[14] = -(far + near) / (far - near);
	matrix[15] = 1.0f;
}

int getActiveParticleCount(ParticleSystem* ps) {
	// Utiliser une Query pour obtenir le nombre de primitives générées
	GLuint query;
	glGenQueries(1, &query);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);

	// Faire le transform feedback...
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, ps->particleCount);
	glEndTransformFeedback();

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

	GLuint result;
	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &result);
	glDeleteQueries(1, &query);

	return (int)result;
}

void cleanupParticleSystem(ParticleSystem* ps) {
	glDeleteProgram(ps->updateProgram);
	glDeleteProgram(ps->renderProgram);
	glDeleteBuffers(1, &ps->particleVBO);
	glDeleteBuffers(1, &ps->transformFeedbackVBO);
	glDeleteBuffers(1, &ps->indexVBO);
	glDeleteVertexArrays(1, &ps->particleVAO);

	if (ps->circleVertices) free(ps->circleVertices);
	if (ps->circleIndices) free(ps->circleIndices);
}

ParticleSystem CreateEmpty()
{
	ParticleSystem tmp = {
		.circleIndices = NULL,
		.circleVertices = NULL,
		.indexVBO = 0,
		.particleCount = 0,
		.particleVAO = 0,
		.particleVBO = 0,
		.renderProgram = 0,
		.sidesCount = 0,
		.transformFeedbackVBO = 0,
		.updateProgram = 0
	};
	return tmp;
}

int main() {
	// Initialize SFML window
	sfVideoMode mode = { 1200, 800, 32 };
	sfRenderWindow* window = sfRenderWindow_create(mode, "GPU Particle System", sfResize | sfClose, NULL);

	if (!window) {
		printf("Failed to create SFML window\n");
		return -1;
	}

	sfRenderWindow_setActive(window, sfTrue);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW\n");
		return -1;
	}

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

	// Initialize particle system with different shapes
	ParticleSystem triangleSystem = CreateEmpty(), squareSystem = CreateEmpty(), circleSystem = CreateEmpty();
	initParticleSystem(&triangleSystem, 3);  // Triangle
	initParticleSystem(&squareSystem, 4);    // Square
	initParticleSystem(&circleSystem, 5);   // Circle (16 sides)

	// Setup matrices
	float projection[16], view[16];
	createOrthoMatrix(projection, 0, 1200, 800, 0, -1, 1);
	createIdentityMatrix(view);

	sfClock* clock = sfClock_create();
	sfEvent event;

	printf("Controls:\n");
	printf("1 - Triangle particles\n");
	printf("2 - Square particles\n");
	printf("3 - Circle particles\n");
	printf("Click to spawn particles\n");

	int currentMode = 2; // Start with circles

	while (sfRenderWindow_isOpen(window)) {
		float deltaTime = sfTime_asSeconds(sfClock_restart(clock));

		// Handle events
		while (sfRenderWindow_pollEvent(window, &event)) {
			if (event.type == sfEvtClosed) {
				sfRenderWindow_close(window);
			}

			if (event.type == sfEvtKeyPressed) {
				if (event.key.code == sfKeyNum1) currentMode = 0;
				else if (event.key.code == sfKeyNum2) currentMode = 1;
				else if (event.key.code == sfKeyNum3) currentMode = 2;
			}

			if (event.type == sfEvtMouseButtonPressed) {
				float x = (float)event.mouseButton.x;
				float y = (float)event.mouseButton.y;

				// Spawn particles based on current mode
				for (int i = 0; i < MAX_PARTICLES; i++) {
					if (currentMode == 0) {
						addParticle(&triangleSystem, x + (rand() % 400 - 20), y + (rand() % 400 - 20), 0);
					}
					else if (currentMode == 1) {
						addParticle(&squareSystem, x + (rand() % 400 - 20), y + (rand() % 400 - 20), 0);
					}
					else {
						addParticle(&circleSystem, x + (rand() % 400 - 20), y + (rand() % 400 - 20), 0);
					}
				}
			}
		}


		// Update particles
		updateParticles(&triangleSystem, deltaTime);
		updateParticles(&squareSystem, deltaTime);
		updateParticles(&circleSystem, deltaTime);

		// Clear screen
		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render particles
		renderParticles(&triangleSystem, projection, view);
		renderParticles(&squareSystem, projection, view);
		renderParticles(&circleSystem, projection, view);

		// Display
		sfRenderWindow_display(window);
	}

	// Cleanup
	cleanupParticleSystem(&triangleSystem);
	cleanupParticleSystem(&squareSystem);
	cleanupParticleSystem(&circleSystem);
	sfClock_destroy(clock);
	sfRenderWindow_destroy(window);

	return 0;
}
//#include "Game.h"
//#include "LoadingState.h"
//#include "Particles.h"
//#include <time.h>
//
//int main(void)
//{
//	sfRenderStates states;
//	states.shader = sfShader_createFromFile(NULL, NULL, "shader.frag");
//	states.transform = sfTransform_Identity;
//	states.blendMode = sfBlendAlpha;
//	states.texture = NULL;
//
//	srand(time(NULL));
//	sfTexture* texture = sfTexture_createFromFile("E:\\BreakerEngine\\Ressources\\ALL\\Textures\\loading.png", NULL);
//	sfTexture_setRepeated(texture, sfFalse);
//	sfShader_setTextureUniform(states.shader, "texture", texture);
//
//	sfRenderWindow* window = sfRenderWindow_create((sfVideoMode_getDesktopMode()), "Shader", sfDefaultStyle, NULL);
//	sfEvent evt;
//
//	// CHANGEMENT 1: Rectangle transparent au lieu d'opaque
//	sfRectangleShape* rect = sfRectangleShape_create();
//	sfRectangleShape_setSize(rect, sfVector2f_Create(1920, 1080));
//	// Rendre le rectangle transparent - c'est crucial !
//	sfRectangleShape_setFillColor(rect, sfTransparent);
//
//	while (sfRenderWindow_isOpen(window))
//	{
//		while (sfRenderWindow_pollEvent(window, &evt))
//		{
//			if (evt.type == sfEvtClosed)
//			{
//				sfRenderWindow_close(window);
//				break;
//			}
//		}
//
//		// CHANGEMENT 2: Clear avec une couleur transparente ou différente
//		sfRenderWindow_clear(window, sfTransparent); // ou une autre couleur
//
//		int count = rand_int(1, 256);
//		sfShader_setIntUniform(states.shader, "count", count);
//
//		for (size_t i = 0; i < count; i++)
//		{
//			char* particle = AddChar("particles[", AddChar(IntToString(i), "].rect_pos"));
//			sfVector2f size = sfVector2f_Create(50.f, 50.f);
//			sfShader_setVec2Uniform(states.shader, particle,
//				sfVector2f_Create(rand_float(0, sfVideoMode_getDesktopMode().width - size.x),
//					rand_float(0, sfVideoMode_getDesktopMode().height - size.y)));
//
//			particle = AddChar("particles[", AddChar(IntToString(i), "].rect_size"));
//			sfShader_setVec2Uniform(states.shader, particle, size);
//			CleanUpTempMemory();
//		}
//
//		sfRenderWindow_drawRectangleShape(window, rect, &states);
//		sfRenderWindow_display(window);
//	}
//
//
//
//
//
//	//InitResourcesManager("../Ressources");
//	//StartGame(CreateWindowManager(1920, 1080, "BreakerEngine", sfDefaultStyle, NULL), "MainMenu", "Loading", &ResetLoadingState);
//}