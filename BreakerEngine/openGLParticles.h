
#include <GL/glew.h>
#include <SFML/Graphics.h>
#include <SFML/OpenGL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Tools.h"


#define VERTICES_PER_PARTICLE 32 // Maximum pour un cercle


// Structure pour les particules mortes (garde la même)
typedef struct {
	float position[3];
	float finalColor[4];
	float finalSize;
} DeadParticle;

typedef struct {
	DeadParticle* particles;
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
	int maxParticles;
	float* circleVertices;
	GLuint* circleIndices;
	float* texCoords;
	float* projection;
	float* view;
	DeadParticlesList deadParticlesList;
} ParticleSystem;



void initParticleSystem(ParticleSystem* ps, int sides, unsigned int maxParticles, sfRenderWindow* window);
void setParticleTexture(ParticleSystem* ps, const char* filename);
void addParticle(ParticleSystem* ps, float x, float y, float z);
void updateParticles(ParticleSystem* ps, float deltaTime);
void renderParticles(ParticleSystem* ps, int useTexture, sfRenderWindow* window);
void cleanupParticleSystem(ParticleSystem* ps);
ParticleSystem createEmptyParticlesSystem();