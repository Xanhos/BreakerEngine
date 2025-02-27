#include "Menu.h"
#include "ResourcesManager.h"
#include "UI.h"
#include "Game.h"

SpriteManager* spriteManager;
UIObjectManager* UIManager;
sfSprite* starSelection;
sfBool isButtonHovered;

void UpdateUIVisual(UIObject* object, WindowManager* window)
{
	if (object->isHover)
	{
		isButtonHovered = sfTrue;
		sfSprite_setPosition(starSelection, AddVector2f(object->getPosition(object), sfVector2f_Create(358, 65)));
	}

	if (object->isClicked)
	{
		if (object->nameIs(object, "Play") && window->GetTimer(window) > .2f)
		{
			ChangeMainState("InGame");
			window->ResetTimer(window);
		}
		else if (object->nameIs(object, "Option") && window->GetTimer(window) > .2f)
		{
			PushSubState("Option");
			window->ResetTimer(window);
		}
		else if (object->nameIs(object, "Quit") && window->GetTimer(window) > .2f)
		{
			EndGame(window);
			window->ResetTimer(window);
		}
	}
}

void InitMainMenu(WindowManager* windowManager)
{
	LoadScene("Menu");
	RegisterSubState("Option", windowManager, sfFalse, sfTrue);

	windowManager->AddNewSound(windowManager, "SFX", 0.f);
	windowManager->AddNewSound(windowManager, "Music", 50.f);

	sfTexture* menu_spritesheet = GetTexture("menu_spritesheet");


	spriteManager = CreateSpriteManager();
	UIManager = CreateUIObjectManager();



	starSelection = sfSprite_create();
	sfSprite_setTexture(starSelection, menu_spritesheet, sfTrue);
	sfSprite_setTextureRect(starSelection, (sfIntRect) { 0, 14146, 102, 97 });
	sfSprite_setOrigin(starSelection, sfVector2f_Create(51, 48.5f));


	sfSprite* spriteHolder = spriteManager->push_back(spriteManager, "BG1", menu_spritesheet, sfTrue);
	sfSprite_setTextureRect(spriteHolder, (sfIntRect) { 0, 0, 1920, 1080 });

	spriteHolder = spriteManager->push_back(spriteManager, "BG2", menu_spritesheet, sfTrue);
	sfSprite_setTextureRect(spriteHolder, (sfIntRect) { 0, 1080, 1920, 1080 });

	spriteHolder = spriteManager->push_back(spriteManager, "BG3", menu_spritesheet, sfTrue);
	sfSprite_setTextureRect(spriteHolder, (sfIntRect) { 0, 2160, 1920, 1080 });

	spriteHolder = spriteManager->push_back(spriteManager, "Title", menu_spritesheet, sfTrue);
	sfSprite_setTextureRect(spriteHolder, (sfIntRect) { 0, 6259, 563, 468 });
	sfSprite_setPosition(spriteHolder, sfVector2f_Create(678, 42));

	UIObject* UIholder = UIManager->push_back(UIManager, CreateUIObjectFromSprite(NULL, "Play", sfMouseLeft, sfKeyUnknown));
	UIholder->setTexture(UIholder, menu_spritesheet, sfFalse);
	UIholder->setTextureRect(UIholder, (sfIntRect) { 0, 14004, 358, 142 });
	UIholder->setPosition(UIholder, sfVector2f_Create(781, 520));
	UIholder->setUpdateFunction(UIholder, &UpdateUIVisual);

	UIholder = UIManager->push_back(UIManager, CreateUIObjectFromSprite(NULL, "Option", sfMouseLeft, sfKeyUnknown));
	UIholder->setTexture(UIholder, menu_spritesheet, sfFalse);
	UIholder->setTextureRect(UIholder, (sfIntRect) { 0, 13294, 358, 142 });
	UIholder->setPosition(UIholder, sfVector2f_Create(781, 680));
	UIholder->setUpdateFunction(UIholder, &UpdateUIVisual);


	UIholder = UIManager->push_back(UIManager, CreateUIObjectFromSprite(NULL, "Quit", sfMouseLeft, sfKeyUnknown));
	UIholder->setTexture(UIholder, menu_spritesheet, sfFalse);
	UIholder->setTextureRect(UIholder, (sfIntRect) { 0, 13436, 358, 142 });
	UIholder->setPosition(UIholder, sfVector2f_Create(781, 840));
	UIholder->setUpdateFunction(UIholder, &UpdateUIVisual);


}

void UpdateEventMainMenu(WindowManager* windowManager, sfEvent* evt)
{

}

void UpdateMainMenu(WindowManager* windowManager)
{
	isButtonHovered = sfFalse;
	UIManager->update(UIManager, windowManager);
}

void RenderMainMenu(WindowManager* windowManager)
{

}

void UIRenderMainMenu(WindowManager* windowManager)
{
	spriteManager->draw(spriteManager, windowManager, NULL);
	UIManager->draw(UIManager, windowManager, NULL);

	if (isButtonHovered)
		windowManager->DrawSprite(windowManager, starSelection, NULL);
}

void DestroyMainMenu(WindowManager* windowManager)
{
	spriteManager->destroy(&spriteManager);
	UIManager->destroy(&UIManager);
	sfSprite_destroy(starSelection);
}

REGISTER_STATE(MainMenu)