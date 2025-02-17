#include "LoadingState.h"
#include "Animation.h"

sfRectangleShape* loading_bar;
sfRectangleShape* loading_bar_outline;
sfSprite* loading_sprite;
sfSprite* background;

void InitLoading(WindowManager* windowManager)
{ 
	loading_bar = sfRectangleShape_create();
	sfRectangleShape_setSize(loading_bar, sfVector2f_Create(0,50));
	sfRectangleShape_setFillColor(loading_bar, sfCyan);
	sfRectangleShape_setPosition(loading_bar, sfVector2f_Create(560, 800));

	loading_bar_outline = sfRectangleShape_create();
	sfRectangleShape_setSize(loading_bar_outline, sfVector2f_Create(800, 50));
	sfRectangleShape_setOutlineColor(loading_bar_outline, sfBlack);
	sfRectangleShape_setOutlineThickness(loading_bar_outline, 5.f);
	sfRectangleShape_setFillColor(loading_bar_outline, sfTransparent);
	sfRectangleShape_setPosition(loading_bar_outline, sfVector2f_Create(560, 800));

	loading_sprite = sfSprite_create();
	sfSprite_setTexture(loading_sprite, GetTexture("loading"), sfTrue);
	sfSprite_setOrigin(loading_sprite, sfVector2f_Create(128, 128));
	sfSprite_setScale(loading_sprite, sfVector2f_Create(0.5f, 0.5f));

	background = sfSprite_create();
	sfSprite_setTexture(background, GetTexture("menu_spritesheet"), sfTrue);
	sfSprite_setTextureRect(background, (sfIntRect) { 0, 0, 1920, 1080 });
} 

void UpdateEventLoading(WindowManager* windowManager, sfEvent* evt) 
{ 

} 

void UpdateLoading(WindowManager* windowManager) 
{ 
	sfRectangleShape_setSize(loading_bar, sfVector2f_Create(LERP(sfRectangleShape_getSize(loading_bar).x, LERP(0, 800, GetLoadingValue()) ,DeltaTime * 5.f), sfRectangleShape_getSize(loading_bar).y));
	sfSprite_setRotation(loading_sprite, sfSprite_getRotation(loading_sprite) + 36.f * DeltaTime);
}

void RenderLoading(WindowManager* windowManager) 
{
	windowManager->DrawSprite(windowManager, background, NULL);
	windowManager->DrawRectangleShape(windowManager, loading_bar, NULL);
	windowManager->DrawRectangleShape(windowManager, loading_bar_outline, NULL);
	sfSprite_setPosition(loading_sprite, sfVector2f_Create(400, 800));
	windowManager->DrawSprite(windowManager, loading_sprite, NULL);
	sfSprite_setPosition(loading_sprite, sfVector2f_Create(1520, 800));
	windowManager->DrawSprite(windowManager, loading_sprite, NULL);
} 

void UIRenderLoading(WindowManager* windowManager)
{

} 

void DestroyLoading(WindowManager* windowManager) 
{
	sfRectangleShape_destroy(loading_bar);
	sfRectangleShape_destroy(loading_bar_outline);
	sfSprite_destroy(loading_sprite);
	sfSprite_destroy(background);

}

REGISTER_STATE(Loading)

void ResetLoadingState(WindowManager* window)
{
	sfRectangleShape_setSize(loading_bar, sfVector2f_Create(0, 50));
}
