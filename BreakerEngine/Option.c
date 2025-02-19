#include "Option.h"
#include "SpriteManager.h"
#include "UI.h"
#include "ResourcesManager.h"
#include "Game.h"

SpriteManager* spriteManagerOption;
UIObjectManager* UIManagerOption;
sfBool sliderIsTargeted[2];

sfVector2f sliderLimit;

static void UpdateSoundUIButton(UIObject* object, WindowManager* window)
{
	int objectId = strcmp(object->getName(object), "SFX") == 0 ? 0 : 1;
	object->setPosition(object, sfVector2f_Create(LERP(sliderLimit.x, sliderLimit.y, window->GetSound(window, object->getName(object)) / 100.f), object->getPosition(object).y));
	if (object->isClicked)
		sliderIsTargeted[objectId] = sfTrue;

	if (sliderIsTargeted[objectId])
	{
		if (sfMouse_isButtonUp(sfMouseLeft))
		{
			sliderIsTargeted[objectId] = sfFalse;
			return;
		}

		object->setPosition(object, sfVector2f_Create(fClamp(window->GetMousePos(window).x, sliderLimit.x, sliderLimit.y), object->getPosition(object).y));
		window->SetSound(window, object->getName(object), INVERT_LERP(sliderLimit.x, sliderLimit.y, object->getPosition(object).x) * 100.f);
	}

	if (sliderIsTargeted[objectId] || object->isHover)
	{
		object->setTextureRect(object, (sfIntRect) { 51, 12042, 51, 49 });
	}
	else
	{
		object->setTextureRect(object, (sfIntRect) { 0, 12042, 51, 49 });
	}
}


void InitOption(WindowManager* windowManager)
{
	sfTexture* menu_spritesheet = GetTexture("menu_spritesheet");

	spriteManagerOption = CreateSpriteManager();
	UIManagerOption = CreateUIObjectManager();

	sfSprite* sprite_holder = spriteManagerOption->push_back(spriteManagerOption, "BG_Option", menu_spritesheet, sfTrue);
	sfSprite_setPosition(sprite_holder, sfVector2f_Create(594, 58));
	sfSprite_setTextureRect(sprite_holder, (sfIntRect) { 0, 11112, 730, 877 });

	sprite_holder = spriteManagerOption->push_back(spriteManagerOption, "fullscreen_square", menu_spritesheet, sfTrue);
	sfSprite_setPosition(sprite_holder, sfVector2f_Create(1079 - 7, 678 - 7));
	sfSprite_setTextureRect(sprite_holder, (sfIntRect) { 0, 14243, 67, 67 });

	UIObject* UI_holder = UIManagerOption->push_back(UIManagerOption, CreateUIObjectFromSprite(NULL, "SFX", sfMouseLeft, sfKeyUnknown));
	UI_holder->setTexture(UI_holder, menu_spritesheet, sfTrue);
	UI_holder->setTextureRect(UI_holder, (sfIntRect) { 0, 12042, 51, 49 });
	UI_holder->setPosition(UI_holder, sfVector2f_Create(804, 353));
	UI_holder->setUpdateFunction(UI_holder, &UpdateSoundUIButton);
	UI_holder->setOrigin(UI_holder, sfVector2f_Create(51 / 2.f, 0));

	UI_holder = UIManagerOption->push_back(UIManagerOption, CreateUIObjectFromSprite(NULL, "Music", sfMouseLeft, sfKeyUnknown));
	UI_holder->setTexture(UI_holder, menu_spritesheet, sfTrue);
	UI_holder->setTextureRect(UI_holder, (sfIntRect) { 0, 12042, 51, 49 });
	UI_holder->setPosition(UI_holder, sfVector2f_Create(804, 491));
	UI_holder->setUpdateFunction(UI_holder, &UpdateSoundUIButton);
	UI_holder->setOrigin(UI_holder, sfVector2f_Create(51 / 2.f, 0));

	UI_holder = UIManagerOption->push_back(UIManagerOption, CreateUIObjectFromSprite(NULL, "Fullscreen_button", sfMouseLeft, sfKeyUnknown));
	UI_holder->setTexture(UI_holder, menu_spritesheet, sfTrue);
	UI_holder->setTextureRect(UI_holder, (sfIntRect) { 0, 11989, 53, 53 });
	UI_holder->setPosition(UI_holder, sfVector2f_Create(1079, 678));

	UI_holder = UIManagerOption->push_back(UIManagerOption, CreateUIObjectFromSprite(NULL, "Back", sfMouseLeft, sfKeyUnknown));
	UI_holder->setTexture(UI_holder, menu_spritesheet, sfTrue);
	UI_holder->setTextureRect(UI_holder, (sfIntRect) { 0, 12726, 358, 142 });
	UI_holder->setPosition(UI_holder, sfVector2f_Create(780, 867));


	sliderLimit = sfVector2f_Create(804 + 51 / 2.f, 1212 + 51 / 2.f);
}

void UpdateEventOption(WindowManager* windowManager, sfEvent* evt)
{

}

void UpdateOption(WindowManager* windowManager)
{
	UIManagerOption->update(UIManagerOption, windowManager);

	UIObject* fullscreen_button = UIManagerOption->get_by_name(UIManagerOption, "Fullscreen_button");
	UIObject* back_button = UIManagerOption->get_by_name(UIManagerOption, "Back");

	spriteManagerOption->sprite_is_visible_by_name(spriteManagerOption, fullscreen_button->isHover, "fullscreen_square");
	fullscreen_button->setIsVisible(fullscreen_button, windowManager->IsFullscreen(windowManager));
	if (windowManager->GetTimer(windowManager) > 0.2f && fullscreen_button->isClicked)
	{
		windowManager->ResetTimer(windowManager);
		windowManager->ToggleFullscreen(windowManager);
	}

	int color = !back_button->isHover ? 200 : 255;
	back_button->setColor(back_button, CreateColor(color, color, color, 255));
	if (back_button->isClicked)
	{
		windowManager->ResetTimer(windowManager);
		PopSubState();
	}
}

void RenderOption(WindowManager* windowManager)
{

}

void UIRenderOption(WindowManager* windowManager)
{
	spriteManagerOption->draw(spriteManagerOption, windowManager, NULL);
	UIManagerOption->draw(UIManagerOption, windowManager, NULL);

}

void DestroyOption(WindowManager* windowManager)
{
	spriteManagerOption->destroy(&spriteManagerOption);
	UIManagerOption->destroy(&UIManagerOption);
}


REGISTER_STATE(Option)