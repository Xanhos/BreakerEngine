#include "Game.h"

int main(void)
{
	InitResourcesManager("../Ressources");
	StartGame(CreateWindowManager(1920, 1080, "BreakerEngine", sfDefaultStyle, NULL), "MainMenu", NULL, NULL);
}