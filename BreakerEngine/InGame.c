#include "InGame.h"
#include "State.h"
#include "Game.h"
#include "SpriteManager.h"
#include "Pool.h"
#include "Player.h"
#include "stdString.h"
#include "Projectile.h"
#include "Enemy.h"

SpriteManager* SpriteInGameManager;


typedef struct {
	Transform transform;
} Projectiles;



static void UpdateParallax(int index)
{
	stdString* name = stdStringCreate(AddChar("Parallax_", AddChar(IntToString(index), "_1")));
	sfSprite* parralax_one = SpriteInGameManager->get_by_name(SpriteInGameManager, name->getData(name));
	name->replace(name, AddChar("Parallax_", AddChar(IntToString(index), "_2")));
	sfSprite* parralax_two = SpriteInGameManager->get_by_name(SpriteInGameManager, name->getData(name));

	sfVector2f pos_one = sfSprite_getPosition(parralax_one);
	sfVector2f pos_two = sfSprite_getPosition(parralax_two);

	sfSprite_setPosition(parralax_one, AddVector2f(pos_one, MultiplyVector2f(sfVector2f_Create(-500, 0), DeltaTime)));
	sfSprite_setPosition(parralax_two, AddVector2f(pos_two, MultiplyVector2f(sfVector2f_Create(-500, 0), DeltaTime)));

	if (pos_one.x < -sfSprite_getTextureRect(parralax_one).width)
	{
		sfSprite_setPosition(parralax_one, sfVector2f_Create(pos_two.x + sfSprite_getTextureRect(parralax_two).width, pos_one.y));
	}
	if (pos_two.x < -sfSprite_getTextureRect(parralax_two).width)
	{
		sfSprite_setPosition(parralax_two, sfVector2f_Create(pos_one.x + sfSprite_getTextureRect(parralax_one).width, pos_two.y ));
	}

	name->destroy(&name);
}

void InitInGame(WindowManager* windowManager)
{
	LoadScene("Game");
	RegisterSubState("Pause", windowManager, sfFalse, sfTrue);

	sfTexture* spritesheet_background = GetTexture("background");

	SpriteInGameManager = CreateSpriteManager();


	sfSprite* sprite_holder = SpriteInGameManager->push_back(SpriteInGameManager, "Background", spritesheet_background, sfTrue);
	sfSprite_setTextureRect(sprite_holder, (sfIntRect) { 0, 0, 1920, 1080 });

	sprite_holder = SpriteInGameManager->push_back(SpriteInGameManager, "Parallax_1_1", spritesheet_background, sfTrue);
	sfSprite_setTextureRect(sprite_holder, (sfIntRect) { 0, 1080, 6556, 1080 });
	sprite_holder = SpriteInGameManager->push_back(SpriteInGameManager, "Parallax_1_2", spritesheet_background, sfTrue);
	sfSprite_setTextureRect(sprite_holder, (sfIntRect) { 0, 1080, 6556, 1080 });
	sfSprite_setPosition(sprite_holder, sfVector2f_Create(6556, 0));


	sprite_holder = SpriteInGameManager->push_back(SpriteInGameManager, "Parallax_2_1", spritesheet_background, sfTrue);
	sfSprite_setTextureRect(sprite_holder, (sfIntRect) { 0, 2160, 6556, 1080 });
	sprite_holder = SpriteInGameManager->push_back(SpriteInGameManager, "Parallax_2_2", spritesheet_background, sfTrue);
	sfSprite_setTextureRect(sprite_holder, (sfIntRect) { 0, 2160, 6556, 1080 });
	sfSprite_setPosition(sprite_holder, sfVector2f_Create(6556, 0));

	InitPlayer();
	InitProjectiles();
	InitEnemies();

}

void UpdateEventInGame(WindowManager* windowManager, sfEvent* evt)
{

}

void UpdateInGame(WindowManager* windowManager)
{
	if (KEY_DOWN(Escape))
	{
		PushSubState("Pause");
		return;
	}
	UpdateParallax(1);
	UpdateParallax(2);


	UpdatePlayer(windowManager);
	UpdateEnemies(windowManager);
	UpdateProjectiles(windowManager);
}

void RenderInGame(WindowManager* windowManager)
{
	SpriteInGameManager->draw(SpriteInGameManager, windowManager, NULL);
	DisplayProjectiles(windowManager);
	DisplayEnemies(windowManager);
	DisplayPlayer(windowManager);
}

void UIRenderInGame(WindowManager* windowManager)
{

}

void DestroyInGame(WindowManager* windowManager)
{
	SpriteInGameManager->destroy(&SpriteInGameManager);
	DestroyPlayer();
	DestroyProjectiles();
	DestroyEnemies();
}

REGISTER_STATE(InGame)