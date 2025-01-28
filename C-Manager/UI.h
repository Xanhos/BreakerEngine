#pragma once
#include "Tools.h"
#include "WindowManager.h"

typedef struct UIObject UIObject;
typedef struct UIObject_Data UIObject_Data;
typedef struct sfDrawable sfDrawable;
typedef struct UIObject_Transform UIObject_Transform;

struct UIObject
{
	UIObject_Data* _Data;
	sfBool isHover;
	sfBool isClicked;
};

struct UIObject_Transform
{
	sfVector2f position;
	sfVector2f scale;
	sfVector2f origin;
	sfVector2f size;
	float rotation;
};

UIObject* CreateUIObjectFromRectangle(sfRectangleShape*, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger);
UIObject* CreateUIObjectFromCircleShape(sfCircleShape*, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger);
UIObject* CreateUIObjectFromSprite(sfSprite*, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger);

void UIObject_SetPosition(UIObject*, sfVector2f);
void UIObject_SetScale(UIObject*, sfVector2f);
void UIObject_SetOrigin(UIObject*, sfVector2f);
void UIObject_SetSize(UIObject*, sfVector2f);
void UIObject_SetRotation(UIObject*, float);
void UIObject_SetColor(UIObject*, sfColor);
void UIObject_SetDrawable(UIObject*, sfDrawable*);
void UIObject_SetTexture(UIObject*, sfTexture*, sfBool);
void UIObject_SetTextureRect(UIObject*, sfIntRect);

sfVector2f UIObject_GetPosition(UIObject*);
sfVector2f UIObject_GetScale(UIObject*);
sfVector2f UIObject_GetOrigin(UIObject*);
sfVector2f UIObject_GetSize(UIObject*);
float UIObject_GetRotation(UIObject*);
sfColor UIObject_GetColor(UIObject*);
sfDrawable* UIObject_GetDrawable(UIObject*);
sfTexture* UIObject_GetTexture(UIObject*, sfTexture*);
sfIntRect UIObject_GetTextureRect(UIObject*);


void UIObject_Update(UIObject*, WindowManager*);
void UIObject_Render(UIObject*, WindowManager*, sfRenderStates*);