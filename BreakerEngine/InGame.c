#include "InGame.h"
#include "State.h"
#include "Game.h"
#include "SpriteManager.h"
#include "Pool.h"
#include "Player.h"
#include "stdString.h"

SpriteManager* SpriteInGameManager;
stdPool* ProjectilesPool, * EnemyPool;


typedef struct {
	Transform transform;
} Projectiles;

typedef struct {
	Transform transform;
} Enemy;


static void UpdateParallax(int index)
{
	stdString* name = stdStringCreate(AddChar("Parallax_", AddChar(index, "_1")));
	sfSprite* parralax_one = SpriteInGameManager->get_by_name(SpriteInGameManager, name->getData(name));
	name->replace(name, AddChar("Parallax_", AddChar(index, "_2")));
	sfSprite* parralax_two = SpriteInGameManager->get_by_name(SpriteInGameManager, name->getData(name));

	sfVector2f pos_one = sfSprite_getPosition(parralax_one);
	sfVector2f pos_two = sfSprite_getPosition(parralax_two);

	if (pos_one.x < -sfSprite_getTextureRect(parralax_one).width);
	{
		sfSprite_setPosition(parralax_one, sfVector2f_Create(pos_one.y, 1920));
	}

	name->destroy(&name);
}

void InitInGame(WindowManager* windowManager)
{
	LoadScene("Game");
	RegisterSubState("Pause", windowManager, sfFalse, sfTrue);
	RegisterSubState("Option", windowManager, sfFalse, sfTrue);

	sfTexture* spritesheet_background = GetTexture("background");

	SpriteInGameManager = CreateSpriteManager();

	ProjectilesPool = stdPool_Create(sizeof(Projectiles), 0);
	EnemyPool = stdPool_Create(sizeof(Enemy), 0);

	sfSprite* sprite_holder = SpriteInGameManager->push_back(SpriteInGameManager, "Background", spritesheet_background, sfTrue);
	sfSprite_setTextureRect(sprite_holder, (sfIntRect) { 0, 0, 1920, 1080 });

	sprite_holder = SpriteInGameManager->push_back(SpriteInGameManager, "Parallax_1_1", spritesheet_background, sfTrue);
	sfSprite_setTextureRect(sprite_holder, (sfIntRect) { 0, 1080, 6556, 1080 });
	sprite_holder = SpriteInGameManager->push_back(SpriteInGameManager, "Parallax_1_2", spritesheet_background, sfTrue);
	sfSprite_setTextureRect(sprite_holder, (sfIntRect) { 0, 1080, 6556, 1080 });


	sprite_holder = SpriteInGameManager->push_back(SpriteInGameManager, "Parallax_2_1", spritesheet_background, sfTrue);
	sfSprite_setTextureRect(sprite_holder, (sfIntRect) { 0, 2160, 6556, 1080 });
	sprite_holder = SpriteInGameManager->push_back(SpriteInGameManager, "Parallax_2_2", spritesheet_background, sfTrue);
	sfSprite_setTextureRect(sprite_holder, (sfIntRect) { 0, 2160, 6556, 1080 });

	InitPlayer();

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
	UpdatePlayer(windowManager);

}

void RenderInGame(WindowManager* windowManager)
{
	DisplayPlayer(windowManager);
}

void UIRenderInGame(WindowManager* windowManager)
{

}

void DestroyInGame(WindowManager* windowManager)
{

}

REGISTER_C_STATE(InGame)