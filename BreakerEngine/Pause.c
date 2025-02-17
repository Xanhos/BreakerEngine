#include "Pause.h"
#include "UI.h"
#include "SpriteManager.h"
#include "ResourcesManager.h"
#include "Game.h"

UIObjectManager* UIPauseManager;
SpriteManager* SpritePauseManager;

static void UpdateUIPauseButton(UIObject* object, WindowManager* window)
{
	int color = object->isHover ? 255 : 200;
	object->setColor(object, CreateColor(color, color, color, 255));

	if (object->isClicked)
	{
		if (object->nameIs(object, "ReturnToGame"))
		{
			PopSubState();
		}
		else if (object->nameIs(object, "Option"))
		{
			PushSubState("Option");
		}
		else if (object->nameIs(object, "Quit"))
		{
			ChangeMainState("MainMenu");
		}
	}
}

void InitPause(WindowManager* windowManager)
{
	RegisterSubState("Option", windowManager, sfFalse, sfTrue);

	sfTexture* menu_spritesheet = GetTexture("menu_spritesheet");
	UIPauseManager = CreateUIObjectManager();
	SpritePauseManager = CreateSpriteManager();

	sfSprite* sprite_holder = SpritePauseManager->push_back(SpritePauseManager, "Pause_BG", menu_spritesheet, sfTrue);
	sfSprite_setTextureRect(sprite_holder, (sfIntRect) { 0, 8481, 730, 877 });
	sfSprite_setPosition(sprite_holder, sfVector2f_Create(594, 58));

	UIObject* UI_holder = UIPauseManager->push_back(UIPauseManager, CreateUIObjectFromSprite(NULL, "ReturnToGame", sfMouseLeft, sfKeyUnknown));
	UI_holder->setTexture(UI_holder, menu_spritesheet, sfTrue);
	UI_holder->setTextureRect(UI_holder, (sfIntRect) { 0, 13578, 358, 142 });
	UI_holder->setPosition(UI_holder, sfVector2f_Create(780, 312));
	UI_holder->setUpdateFunction(UI_holder, &UpdateUIPauseButton);

	UI_holder = UIPauseManager->push_back(UIPauseManager, CreateUIObjectFromSprite(NULL, "Option", sfMouseLeft, sfKeyUnknown));
	UI_holder->setTexture(UI_holder, menu_spritesheet, sfTrue);
	UI_holder->setTextureRect(UI_holder, (sfIntRect) { 0, 13294, 358, 142 });
	UI_holder->setPosition(UI_holder, sfVector2f_Create(780, 510));
	UI_holder->setUpdateFunction(UI_holder, &UpdateUIPauseButton);

	UI_holder = UIPauseManager->push_back(UIPauseManager, CreateUIObjectFromSprite(NULL, "Quit", sfMouseLeft, sfKeyUnknown));
	UI_holder->setTexture(UI_holder, menu_spritesheet, sfTrue);
	UI_holder->setTextureRect(UI_holder, (sfIntRect) { 0, 13436, 358, 142 });
	UI_holder->setPosition(UI_holder, sfVector2f_Create(780, 707));
	UI_holder->setUpdateFunction(UI_holder, &UpdateUIPauseButton);
}

void UpdateEventPause(WindowManager* windowManager, sfEvent* evt)
{

}

void UpdatePause(WindowManager* windowManager)
{
	UIPauseManager->update(UIPauseManager, windowManager);
}

void RenderPause(WindowManager* windowManager)
{

}

void UIRenderPause(WindowManager* windowManager)
{
	SpritePauseManager->draw(SpritePauseManager, windowManager, NULL);
	UIPauseManager->draw(UIPauseManager, windowManager, NULL);
}

void DestroyPause(WindowManager* windowManager)
{
	SpritePauseManager->destroy(&SpritePauseManager);
	UIPauseManager->destroy(&UIPauseManager);
}

REGISTER_STATE(Pause)