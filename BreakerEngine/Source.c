#include "Game.h"
#include "LoadingState.h"
#include "Particles.h"

int main(void)
{
	LoadParticlesFromFile("D:/GitCours/C/C-Manager/ParticlesMaker/Export/AnimTest.part", sfVector2f_Create(0, 0), NULL);
	InitResourcesManager("../Ressources");
	StartGame(CreateWindowManager(1920, 1080, "BreakerEngine", sfDefaultStyle, NULL), "MainMenu", "Loading", &ResetLoadingState);
}