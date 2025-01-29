#pragma once
#include "Tools.h"
#include "WindowManager.h"

typedef struct UIObject UIObject;
typedef struct UIObject_Data UIObject_Data;
typedef struct sfDrawable sfDrawable;
typedef struct UIObject_Transform UIObject_Transform;
typedef struct UIObjectManager UIObjectManager;
typedef struct UIObjectManager_Data UIObjectManager_Data;
typedef void (*UIUpdateFunction)(UIObject* object, WindowManager* window);

struct UIObject
{
	UIObject_Data* _Data;
	sfBool isHover;
	sfBool isClicked;

	void (*setPosition)(UIObject* object, sfVector2f position);
	void (*setScale)(UIObject* object, sfVector2f scale);
	void (*setOrigin)(UIObject* object, sfVector2f origin);
	void (*setSize)(UIObject* object, sfVector2f size);
	void (*setRotation)(UIObject* object, float rotation);
	void (*setColor)(UIObject* object, sfColor color);
	void (*setDrawable)(UIObject* object, sfDrawable* drawable);
	void (*setTexture)(UIObject* object, sfTexture* texture, sfBool reset_rect);
	void (*setTextureRect)(UIObject* object, sfIntRect texture_rect);
	void (*setUpdateFunction)(UIObject* object, UIUpdateFunction callback);

	const char* (*getName)(UIObject* object);
	sfVector2f (*getPosition)(UIObject* object);
	sfVector2f (*getScale)(UIObject* object);
	sfVector2f (*getOrigin)(UIObject* object);
	sfVector2f (*getSize)(UIObject* object);
	float (*getRotation)(UIObject* object);
	sfColor (*getColor)(UIObject* object);
	sfDrawable* (*getDrawable)(UIObject* object);
	sfTexture* (*getTexture)(UIObject* object);
	sfIntRect (*getTextureRect)(UIObject* object);

	void (*update)(UIObject* object, WindowManager* window);
	void (*render)(WindowManager* window, UIObject* object, sfRenderStates* state);
	void (*destroy)(UIObject** object);
};

struct UIObject_Transform
{
	sfVector2f position;
	sfVector2f scale;
	sfVector2f origin;
	sfVector2f size;
	float rotation;
};

struct UIObjectManager 
{
	UIObjectManager_Data* _Data;

	UIObject* (*Add)(UIObjectManager* manager, UIObject* object);
	UIObject* (*GetFromName)(UIObjectManager* manager, const char* name);
	UIObject* (*GetFromIndex)(UIObjectManager* manager, int index);
	void (*RemoveFromPointer)(UIObjectManager* manager, UIObject* object);
	void (*RemoveFromIndex)(UIObjectManager* manager, int index);
	void (*RemoveFromName)(UIObjectManager* manager, char* name);
	void (*Update)(UIObjectManager* manager, WindowManager* window);
	void (*Render)(UIObjectManager* manager, WindowManager* window);
	void (*Destroy)(UIObjectManager** manager);
};

UIObject* CreateUIObjectFromRectangle(sfRectangleShape* shape, char* name,sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger);
UIObject* CreateUIObjectFromCircleShape(sfCircleShape* shape, char* name,sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger);
UIObject* CreateUIObjectFromSprite(sfSprite* shape, char* name,sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger);


UIObjectManager* CreateUIObjectManager();

