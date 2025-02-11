#include "Game.h"

int main(void)
{
	InitResourcesManager("../Ressources");
	StartGame(CreateWindowManager(1080, 720, "BreakerEngine", sfDefaultStyle, NULL), "Menu", NULL, NULL);
}