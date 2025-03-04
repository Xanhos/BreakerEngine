#include "Game.h"
#include "LoadingState.h"
#include "Particles.h"

int main(void)
{
	InitResourcesManager("../Ressources");
	StartGame(CreateWindowManager(1920, 1080, "BreakerEngine", sfDefaultStyle, NULL), "MainMenu", "Loading", &ResetLoadingState);
}