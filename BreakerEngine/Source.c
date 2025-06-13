
#include <GL/glew.h>
#include <SFML/Graphics.h>
#include <SFML/OpenGL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Tools.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_PARTICLES 2000
#define VERTICES_PER_PARTICLE 32 // Maximum pour un cercle


// Structure pour les particules mortes (garde la même)
typedef struct {
	float position[3];
	float finalColor[4];
	float finalSize;
} DeadParticle;

typedef struct {
	DeadParticle particles[MAX_PARTICLES];
	int count;
} DeadParticlesList;



typedef struct {
	float position[3];
	float velocity[3];
	float life;
	float size;
	float rotation;
	float color[4];
} Particle;


// Ajouter ces champs à la structure ParticleSystem
typedef struct {
	GLuint updateProgram;
	GLuint renderProgram;
	GLuint deadParticleProgram;  // NOUVEAU: Programme pour capturer les particules mortes
	GLuint particleVBO;
	GLuint particleVAO;
	GLuint indexVBO;
	GLuint transformFeedbackVBO;
	GLuint deadParticleTFBO;     // NOUVEAU: Buffer pour les particules mortes
	GLuint texture;
	int particleCount;
	int sidesCount;
	float* circleVertices;
	GLuint* circleIndices;
	float* texCoords;
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

// Nouveau geometry shader spécialement pour capturer les particules mourantes
const char* deadParticleGeometryShader = "#version 330 core\n"
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
"out vec3 dead_position;\n"
"out vec4 dead_color;\n"
"out float dead_size;\n"
"\n"
"uniform float deltaTime;\n"
"\n"
"void main() {\n"
"    if (vs_life[0] > 0.0 && (vs_life[0] - deltaTime) <= 0.0) {\n"
"        dead_position = vs_position[0];\n"
"        dead_color = vs_color[0];\n"
"        dead_size = vs_size[0];\n"
"        EmitVertex();\n"
"    }\n"
"}\n";

// Vertex shader pour le rendu avec support texture
const char* renderVertexShader = "#version 330 core\n"
"layout(location = 0) in vec3 particlePos;\n"
"layout(location = 1) in vec3 velocity;\n"
"layout(location = 2) in float life;\n"
"layout(location = 3) in float size;\n"
"layout(location = 4) in float rotation;\n"
"layout(location = 5) in vec4 color;\n"
"layout(location = 6) in vec2 vertexPos;\n"
"layout(location = 7) in vec2 texCoord;\n"  // Coordonnées de texture
"\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"\n"
"out vec4 fragColor;\n"
"out vec2 fragTexCoord;\n"  // Passer les coordonnées de texture au fragment shader
"\n"
"void main() {\n"
"    if (life <= 0.0) {\n"
"        gl_Position = vec4(-10.0, -10.0, -10.0, 1.0);\n"
"        fragColor = vec4(0.0);\n"
"        fragTexCoord = vec2(0.0);\n"
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
"    fragTexCoord = texCoord;\n"
"}\n";

// Fragment shader pour le rendu avec support texture
const char* renderFragmentShader = "#version 330 core\n"
"in vec4 fragColor;\n"
"in vec2 fragTexCoord;\n"
"out vec4 outColor;\n"
"uniform sampler2D particleTexture;\n"
"uniform bool useTexture;\n"
"\n"
"void main() {\n"
"    vec4 color = fragColor;\n"
"    \n"
"    if (useTexture) {\n"
"        vec4 texColor = texture(particleTexture, fragTexCoord);\n"
"        color *= texColor;\n"
"    }\n"
"    \n"
"    outColor = color;\n"
"}\n";

typedef struct {
	int aliveCount;
	int deadIndices[MAX_PARTICLES];
	int deadCount;
} ParticleStats;

// Fonction pour charger une texture depuis un fichier
GLuint loadTexture(const char* filename) {
	sfImage* image = sfImage_createFromFile(filename);
	if (!image) {
		printf("Erreur: impossible de charger l'image %s\n", filename);
		return 0;
	}

	sfVector2u size = sfImage_getSize(image);
	const sfUint8* pixels = sfImage_getPixelsPtr(image);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Configurer les paramètres de texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Charger les données de l'image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	sfImage_destroy(image);

	printf("Texture chargée: %s (%dx%d)\n", filename, size.x, size.y);
	return textureID;
}

// Créer une texture procédurale simple (cercle blanc)
GLuint createDefaultTexture() {
	const int size = 64;
	unsigned char* data = malloc(size * size * 4);

	int center = size / 2;
	float radius = size / 2.0f - 2.0f;

	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			float dx = x - center;
			float dy = y - center;
			float distance = sqrt(dx * dx + dy * dy);

			int index = (y * size + x) * 4;

			if (distance <= radius) {
				// Cercle blanc avec alpha qui diminue vers les bords
				float alpha = 1.0f - (distance / radius);
				alpha = alpha * alpha; // Courbe plus douce

				data[index + 0] = 255; // R
				data[index + 1] = 255; // G
				data[index + 2] = 255; // B
				data[index + 3] = (unsigned char)(alpha * 255); // A
			}
			else {
				// Transparent
				data[index + 0] = 0;
				data[index + 1] = 0;
				data[index + 2] = 0;
				data[index + 3] = 0;
			}
		}
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	free(data);
	return textureID;
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
	if (ps->texCoords) free(ps->texCoords);

	// Generate vertices for polygon
	ps->circleVertices = malloc(sizeof(float) * (sides + 1) * 2); // +1 for center
	ps->circleIndices = malloc(sizeof(GLuint) * sides * 3);
	ps->texCoords = malloc(sizeof(float) * (sides + 1) * 2); // Coordonnées de texture

	// Center point
	ps->circleVertices[0] = 0.0f;
	ps->circleVertices[1] = 0.0f;
	ps->texCoords[0] = 0.5f;  // Centre de la texture
	ps->texCoords[1] = 0.5f;

	// Generate vertices around circle
	for (int i = 0; i < sides; i++) {
		float angle = 2.0f * M_PI * i / sides;
		ps->circleVertices[(i + 1) * 2] = cos(angle);
		ps->circleVertices[(i + 1) * 2 + 1] = sin(angle);

		// Coordonnées de texture (mapper le cercle sur la texture)
		ps->texCoords[(i + 1) * 2] = 0.5f + 0.5f * cos(angle);
		ps->texCoords[(i + 1) * 2 + 1] = 0.5f + 0.5f * sin(angle);
	}

	// Generate indices for triangles
	for (int i = 0; i < sides; i++) {
		ps->circleIndices[i * 3] = 0; // Center
		ps->circleIndices[i * 3 + 1] = i + 1;
		ps->circleIndices[i * 3 + 2] = (i + 1) % sides + 1;
	}
}

void generateRectangleGeometry(ParticleSystem* ps) {
	ps->sidesCount = 2; // 2 triangles pour former un rectangle

	// Free previous data
	if (ps->circleVertices) free(ps->circleVertices);
	if (ps->circleIndices) free(ps->circleIndices);
	if (ps->texCoords) free(ps->texCoords);

	// Generate vertices for rectangle (4 corners)
	ps->circleVertices = malloc(sizeof(float) * 4 * 2);
	ps->circleIndices = malloc(sizeof(GLuint) * 6); // 2 triangles = 6 indices
	ps->texCoords = malloc(sizeof(float) * 4 * 2);

	// Rectangle corners (centered on origin)
	// Bottom-left
	ps->circleVertices[0] = -0.5f;
	ps->circleVertices[1] = -0.5f;
	ps->texCoords[0] = 0.0f;
	ps->texCoords[1] = 1.0f; // Flip Y for texture

	// Bottom-right
	ps->circleVertices[2] = 0.5f;
	ps->circleVertices[3] = -0.5f;
	ps->texCoords[2] = 1.0f;
	ps->texCoords[3] = 1.0f;

	// Top-right
	ps->circleVertices[4] = 0.5f;
	ps->circleVertices[5] = 0.5f;
	ps->texCoords[4] = 1.0f;
	ps->texCoords[5] = 0.0f;

	// Top-left
	ps->circleVertices[6] = -0.5f;
	ps->circleVertices[7] = 0.5f;
	ps->texCoords[6] = 0.0f;
	ps->texCoords[7] = 0.0f;

	// Generate indices for 2 triangles
	// Triangle 1: bottom-left, bottom-right, top-right
	ps->circleIndices[0] = 0;
	ps->circleIndices[1] = 1;
	ps->circleIndices[2] = 2;

	// Triangle 2: bottom-left, top-right, top-left
	ps->circleIndices[3] = 0;
	ps->circleIndices[4] = 2;
	ps->circleIndices[5] = 3;
}


// Fonction modifiée pour initialiser le système (remplacer initParticleSystem)
void initParticleSystem(ParticleSystem* ps, int sides) {
	ps->particleCount = 0;

	// Generate polygon geometry
	generatePolygonGeometry(ps, sides);

	// Create shaders - programmes existants
	const char* varyings[] = { "out_position", "out_velocity", "out_life", "out_size", "out_rotation", "out_color" };
	ps->updateProgram = createShaderProgram(updateVertexShaderWithGS, updateGeometryShader, NULL, varyings, 6);
	ps->renderProgram = createShaderProgram(renderVertexShader, NULL, renderFragmentShader, NULL, 0);

	// NOUVEAU: Créer le programme pour capturer les particules mortes
	const char* deadVaryings[] = { "dead_position", "dead_color", "dead_size" };
	ps->deadParticleProgram = createShaderProgram(updateVertexShaderWithGS, deadParticleGeometryShader, NULL, deadVaryings, 3);

	// Créer une texture par défaut
	ps->texture = createDefaultTexture();

	// Create buffers existants
	glGenBuffers(1, &ps->particleVBO);
	glGenBuffers(1, &ps->transformFeedbackVBO);
	glGenBuffers(1, &ps->indexVBO);
	glGenVertexArrays(1, &ps->particleVAO);

	// NOUVEAU: Créer le buffer pour les particules mortes
	glGenBuffers(1, &ps->deadParticleTFBO);
	glBindBuffer(GL_ARRAY_BUFFER, ps->deadParticleTFBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DeadParticle) * MAX_PARTICLES, NULL, GL_DYNAMIC_READ);

	// Setup particle buffer
	glBindBuffer(GL_ARRAY_BUFFER, ps->particleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * MAX_PARTICLES, NULL, GL_DYNAMIC_DRAW);

	// Setup transform feedback buffer
	glBindBuffer(GL_ARRAY_BUFFER, ps->transformFeedbackVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * MAX_PARTICLES, NULL, GL_DYNAMIC_DRAW);

	// Setup polygon indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ps->indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * sides * 3, ps->circleIndices, GL_STATIC_DRAW);

	// Setup VAO for rendering (code existant...)
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

	// Create separate VBO for texture coordinates
	GLuint texCoordVBO;
	glGenBuffers(1, &texCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (sides + 1) * 2, ps->texCoords, GL_STATIC_DRAW);

	// Texture coordinates (per vertex, not instanced)
	glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(7);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ps->indexVBO);
	glBindVertexArray(0);
}


// Fonction pour changer la texture d'un système de particules
void setParticleTexture(ParticleSystem* ps, const char* filename) {
	if (ps->texture != 0) {
		glDeleteTextures(1, &ps->texture);
	}

	GLuint oldSidesCount = ps->sidesCount;
	GLuint polygonVBO = 0, texCoordVBO = 0;

	if (filename) {
		ps->texture = loadTexture(filename);
		if (ps->texture == 0) {
			// Si le chargement échoue, utiliser la texture par défaut
			ps->texture = createDefaultTexture();
			// Garder la géométrie existante pour texture par défaut
		}
		else {
			// Texture chargée avec succès, passer en mode rectangle
			generateRectangleGeometry(ps);

			// Mise à jour des buffers de géométrie
			glBindVertexArray(ps->particleVAO);

			// Mettre à jour le buffer des vertices
			glGenBuffers(1, &polygonVBO);
			glBindBuffer(GL_ARRAY_BUFFER, polygonVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, ps->circleVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(6);

			// Mettre à jour le buffer des coordonnées de texture
			glGenBuffers(1, &texCoordVBO);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, ps->texCoords, GL_STATIC_DRAW);
			glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(7);

			// Mettre à jour le buffer d'indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ps->indexVBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, ps->circleIndices, GL_STATIC_DRAW);

			glBindVertexArray(0);
		}
	}
	else {
		ps->texture = createDefaultTexture();
		// Garder la géométrie existante pour texture par défaut
	}
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
	particle.size = 100.0f + ((float)rand() / RAND_MAX) * 20.0f;
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

void resetOpenGLState()
{
	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Désactiver tous les attributs de vertex
	for (int i = 0; i < 16; i++) {
		glDisableVertexAttribArray(i);
		glVertexAttribDivisor(i, 0); // Reset divisor
	}

	// Restaurer les états de blend pour SFML
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Désactiver les états qui peuvent interférer
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_RASTERIZER_DISCARD);

	// Restaurer le viewport
	glViewport(0, 0, 1200, 800);

	// Nettoyer les bindings de transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
}


// Nouvelle fonction pour capturer les particules mortes sur GPU
int captureDeadParticlesGPU(ParticleSystem* ps, float deltaTime) {
	if (ps->particleCount == 0) return 0;

	glUseProgram(ps->deadParticleProgram);
	glUniform1f(glGetUniformLocation(ps->deadParticleProgram, "deltaTime"), deltaTime);

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

	// Bind dead particle transform feedback buffer
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, ps->deadParticleTFBO);

	// Use query to count dead particles
	GLuint query;
	glGenQueries(1, &query);
	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);

	glEnable(GL_RASTERIZER_DISCARD);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, ps->particleCount);
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

	GLuint deadCount;
	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &deadCount);
	glDeleteQueries(1, &query);

	resetOpenGLState();
	return (int)deadCount;
}

// Fonction pour lire les particules mortes depuis le GPU
DeadParticlesList readDeadParticlesFromGPU(ParticleSystem* ps, int deadCount) {
	DeadParticlesList deadList;
	deadList.count = deadCount;

	if (deadCount > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, ps->deadParticleTFBO);
		DeadParticle* gpuData = (DeadParticle*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

		if (gpuData) {
			// Copier seulement le nombre de particules mortes
			int copyCount = (deadCount < MAX_PARTICLES) ? deadCount : MAX_PARTICLES;
			memcpy(deadList.particles, gpuData, copyCount * sizeof(DeadParticle));
			deadList.count = copyCount;
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
		else {
			deadList.count = 0;
		}
	}

	return deadList;
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

// Fonction updateParticles() complètement réécrite
void updateParticles(ParticleSystem* ps, float deltaTime, DeadParticlesList* deadParticles) {
	// Capturer les particules mortes AVANT la mise à jour
	int deadCount = 0;
	if (deadParticles) {
		deadCount = captureDeadParticlesGPU(ps, deltaTime);
		*deadParticles = readDeadParticlesFromGPU(ps, deadCount);
	}

	if (ps->particleCount == 0) return;

	// Mise à jour normale des particules
	glUseProgram(ps->updateProgram);
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

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, ps->transformFeedbackVBO);

	glEnable(GL_RASTERIZER_DISCARD);
	ps->particleCount = getActiveParticleCount(ps);
	glDisable(GL_RASTERIZER_DISCARD);

	// Swap buffers
	GLuint temp = ps->particleVBO;
	ps->particleVBO = ps->transformFeedbackVBO;
	ps->transformFeedbackVBO = temp;

	resetOpenGLState();
}

void renderParticles(ParticleSystem* ps, float* projection, float* view, int useTexture) {
	if (ps->particleCount == 0) return;

	glUseProgram(ps->renderProgram);

	// Set matrices
	glUniformMatrix4fv(glGetUniformLocation(ps->renderProgram, "projection"), 1, GL_FALSE, projection);
	glUniformMatrix4fv(glGetUniformLocation(ps->renderProgram, "view"), 1, GL_FALSE, view);

	// Set texture uniforms
	glUniform1i(glGetUniformLocation(ps->renderProgram, "useTexture"), useTexture);
	if (useTexture && ps->texture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ps->texture);
		glUniform1i(glGetUniformLocation(ps->renderProgram, "particleTexture"), 0);
	}

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

	// Calculer le nombre d'indices à dessiner
	int indexCount = (ps->sidesCount == 2) ? 6 : ps->sidesCount * 3;

	// Draw instanced polygons
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, ps->particleCount);

	// NOUVEAU: Nettoyer les états immédiatement après le rendu
	resetOpenGLState();
}

void createIdentityMatrix(float* matrix) {
	memset(matrix, 0, 16 * sizeof(float));
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
}
#undef near;
#undef far;

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



// Modification du cleanup
void cleanupParticleSystem(ParticleSystem* ps) {
	glDeleteProgram(ps->updateProgram);
	glDeleteProgram(ps->renderProgram);
	glDeleteProgram(ps->deadParticleProgram);  // NOUVEAU
	glDeleteBuffers(1, &ps->particleVBO);
	glDeleteBuffers(1, &ps->transformFeedbackVBO);
	glDeleteBuffers(1, &ps->deadParticleTFBO);  // NOUVEAU
	glDeleteBuffers(1, &ps->indexVBO);
	glDeleteVertexArrays(1, &ps->particleVAO);

	if (ps->circleVertices) free(ps->circleVertices);
	if (ps->circleIndices) free(ps->circleIndices);
	if (ps->texCoords) free(ps->texCoords);  // NOUVEAU
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

	setParticleTexture(&squareSystem, "..\\Ressources\\ALL\\Textures\\loading.png");

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
	sfSprite* rect = sfSprite_create();
	sfSprite_setTexture(rect, sfTexture_createFromFile("..\\Ressources\\ALL\\Textures\\loading.png", NULL), sfTrue);
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

			//if (event.type == sfEvtMouseButtonPressed) {
			//	float x = (float)event.mouseButton.x;
			//	float y = (float)event.mouseButton.y;

			//	// Spawn particles based on current mode
			//	for (int i = 0; i < MAX_PARTICLES; i++) {
			//		if (currentMode == 0) {
			//		addParticle(&triangleSystem, x + (rand() % 400 - 20), y + (rand() % 400 - 20), 0);
			//	}
			//else if (currentMode == 1) {
			//	addParticle(&squareSystem, x + (rand() % 20 - 20), y + (rand() % 20 - 20), 0);
			//}
			//else {
			//	addParticle(&circleSystem, x + (rand() % 400 - 20), y + (rand() % 400 - 20), 0);
			//}
			//	}
			//}
		}

		if (sfMouse_isButtonPressed(sfMouseLeft))
		{
			sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
			if (currentMode == 0) {
				addParticle(&triangleSystem, mousePos.x , mousePos.y , 0);
			}
			else if (currentMode == 1) {
				addParticle(&squareSystem, mousePos.x , mousePos.y , 0);
			}
			else {
				addParticle(&circleSystem, mousePos.x , mousePos.y, 0);
			}
		}

		DeadParticlesList deadTriangles, deadSquares, deadCircles;

		updateParticles(&triangleSystem, deltaTime, &deadTriangles);
		updateParticles(&squareSystem, deltaTime, &deadSquares);
		updateParticles(&circleSystem, deltaTime, &deadCircles);

		// Utiliser les particules mortes
		if (deadTriangles.count > 0) {
			printf("Frame: %d particules triangles mortes\n", deadTriangles.count);
			for (int i = 0; i < deadTriangles.count; i++) {
				printf("  Triangle mort à: (%.2f, %.2f, %.2f)\n",
					deadTriangles.particles[i].position[0],
					deadTriangles.particles[i].position[1],
					deadTriangles.particles[i].position[2]);
			}
		}

		if (deadSquares.count > 0) {
			printf("Frame: %d particules carrés mortes\n", deadSquares.count);
			for (int i = 0; i < deadSquares.count; i++) {
				printf("  Carré mort à: (%.2f, %.2f, %.2f)\n",
					deadSquares.particles[i].position[0],
					deadSquares.particles[i].position[1],
					deadSquares.particles[i].position[2]);
			}
		}

		if (deadCircles.count > 0) {
			printf("Frame: %d particules cercles mortes\n", deadCircles.count);
			for (int i = 0; i < deadCircles.count; i++) {
				printf("  Cercle mort à: (%.2f, %.2f, %.2f)\n",
					deadCircles.particles[i].position[0],
					deadCircles.particles[i].position[1],
					deadCircles.particles[i].position[2]);
				sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
				addParticle(&circleSystem, mousePos.x, mousePos.y, 0);
			}
		}

		// Clear screen
		sfRenderWindow_clear(window, sfBlack);

		renderParticles(&triangleSystem, projection, view, 0);
		sfRenderWindow_drawSprite(window, rect, NULL);
		renderParticles(&squareSystem, projection, view, 1);

		renderParticles(&circleSystem, projection, view, 0);



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
