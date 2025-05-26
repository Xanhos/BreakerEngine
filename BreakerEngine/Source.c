#include "Game.h"
#include "LoadingState.h"
#include "Particles.h"

int main(void)
{
	Animation* anim = CreateAnimationFromFile("C:\\Users\\y.grallan\\Downloads\\PlayerPoisson.anim",NULL);

	Animation* newanim = CopyAnimation(anim);
	anim->SelectAnimationKey(anim, "IdleF");
	anim->Update(anim, 0.016f);
	anim->Destroy(&anim);
	newanim->SelectAnimationKey(newanim, "IdleF");
	newanim->Update(newanim, 0.016f);
	newanim->Destroy(&newanim);


	InitResourcesManager("../Ressources");
	StartGame(CreateWindowManager(1920, 1080, "BreakerEngine", sfDefaultStyle, NULL), "MainMenu", "Loading", &ResetLoadingState);
}