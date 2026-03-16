#include "Game.h"
#include "LoadingState.h"
#include "Particles.h"
#include "CrashHandler.h"
 

int main(void)
{
	srand((unsigned int)time(NULL));
	MainThreadHasStarted = sfTrue;
	SetupLogFile();
	SetCustomExceptionHandler();

	InitResourcesManager("../Ressources");
	StartGame(CreateWindowManager(1920, 1080, "BreakerEngine", sfDefaultStyle, NULL), "MainMenu", "Loading", &ResetLoadingState);
	 
}