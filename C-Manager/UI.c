#include "UI.h"

typedef enum
{
	RECTANGLE,
	CIRCLE,
	SPRITE
} UIObject_Type;

struct UIObject_Data
{
	sfDrawable* drawable;
	UIObject_Transform transform;
	UIObject_Type type;
	sfColor color;
	sfMouseButton mouse_button_trigger;
	sfKeyCode key_button_trigger;
};



UIObject* CreateUIObject(sfDrawable* shape, UIObject_Type type, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger)
{
	UIObject* object = (UIObject*)malloc(sizeof(UIObject));
	assert(object);
	object->_Data = (UIObject_Data*)malloc(sizeof(UIObject_Data));
	assert(object->_Data);
	object->_Data->drawable = shape;
	object->_Data->type = type;
	object->_Data->color = sfWhite;
	object->_Data->mouse_button_trigger = mouse_button_trigger;
	object->_Data->key_button_trigger = key_button_trigger;
	object->isClicked = sfFalse;
	object->isHover = sfFalse;

	UIObject_Transform tmpTransform = { {0,0},{0,0},{0,0},{0,0},0 };
	switch (type)
	{
	case RECTANGLE:
		tmpTransform.position = sfRectangleShape_getPosition((sfRectangleShape*)shape);
		tmpTransform.scale = sfRectangleShape_getScale((sfRectangleShape*)shape);
		tmpTransform.origin = sfRectangleShape_getOrigin((sfRectangleShape*)shape);
		tmpTransform.size = sfRectangleShape_getSize((sfRectangleShape*)shape);
		tmpTransform.rotation = sfRectangleShape_getRotation((sfRectangleShape*)shape);
		object->_Data->transform = tmpTransform;
		break;
	case CIRCLE:
		tmpTransform.position = sfCircleShape_getPosition((sfCircleShape*)shape);
		tmpTransform.scale = sfCircleShape_getScale((sfCircleShape*)shape);
		tmpTransform.origin = sfCircleShape_getOrigin((sfCircleShape*)shape);
		tmpTransform.size = (sfVector2f){ sfCircleShape_getRadius((sfCircleShape*)shape), sfCircleShape_getRadius((sfCircleShape*)shape) };
		tmpTransform.rotation = sfCircleShape_getRotation((sfCircleShape*)shape);
		break;
	case SPRITE:
		tmpTransform.position = sfSprite_getPosition((sfSprite*)shape);
		tmpTransform.scale = sfSprite_getScale((sfSprite*)shape);
		tmpTransform.origin = sfSprite_getOrigin((sfSprite*)shape);
		tmpTransform.size = (sfVector2f){ sfSprite_getTextureRect((sfSprite*)shape).width, sfSprite_getTextureRect((sfSprite*)shape).height };
		tmpTransform.rotation = sfSprite_getRotation((sfSprite*)shape);
		break;
	default:
		break;
	}
	
	return object;
}


UIObject* CreateUIObjectFromRectangle(sfRectangleShape* shape, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger)
{
	return CreateUIObject((sfDrawable*)shape, RECTANGLE, mouse_button_trigger, key_button_trigger);
}

UIObject* CreateUIObjectFromCircleShape(sfCircleShape* shape, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger)
{
	return CreateUIObject((sfDrawable*)shape, CIRCLE, mouse_button_trigger, key_button_trigger);
}

UIObject* CreateUIObjectFromSprite(sfSprite* shape, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger)
{
	return CreateUIObject((sfDrawable*)shape, SPRITE, mouse_button_trigger, key_button_trigger);
}

void UIObject_SetPosition(UIObject* object, sfVector2f position)
{
	object->_Data->transform.position = position;
	switch (object->_Data->type)
	{
	case RECTANGLE:
		sfRectangleShape_setPosition((sfRectangleShape*)object->_Data->drawable, position);
		break;
	case CIRCLE:
		sfCircleShape_setPosition((sfCircleShape*)object->_Data->drawable, position);
		break;
	case SPRITE:
		sfSprite_setPosition((sfSprite*)object->_Data->drawable, position);
		break;
	default:
		break;
	}
}

void UIObject_SetScale(UIObject* object, sfVector2f scale)
{
	object->_Data->transform.scale = scale;
	switch (object->_Data->type)
	{
	case RECTANGLE:
		sfRectangleShape_setScale((sfRectangleShape*)object->_Data->drawable, scale);
		break;
	case CIRCLE:
		sfCircleShape_setScale((sfCircleShape*)object->_Data->drawable, scale);
		break;
	case SPRITE:
		sfSprite_setScale((sfSprite*)object->_Data->drawable, scale);
		break;
	default:
		break;
	}
}

void UIObject_SetOrigin(UIObject* object, sfVector2f origin)
{
	object->_Data->transform.origin = origin;
	switch (object->_Data->type)
	{
	case RECTANGLE:
		sfRectangleShape_setOrigin((sfRectangleShape*)object->_Data->drawable, origin);
		break;
	case CIRCLE:
		sfCircleShape_setOrigin((sfCircleShape*)object->_Data->drawable, origin);
		break;
	case SPRITE:
		sfSprite_setOrigin((sfSprite*)object->_Data->drawable, origin);
		break;
	default:
		break;
	}
}

void UIObject_SetSize(UIObject* object, sfVector2f size)
{
	object->_Data->transform.size = size;
	switch (object->_Data->type)
	{
	case RECTANGLE:
		sfRectangleShape_setSize((sfRectangleShape*)object->_Data->drawable, size);
		break;
	case CIRCLE:
		sfCircleShape_setRadius((sfCircleShape*)object->_Data->drawable, size.x);
		break;
	case SPRITE:
		sfSprite_setScale((sfSprite*)object->_Data->drawable, size);
		break;
	default:
		break;
	}
}

void UIObject_SetRotation(UIObject* object, float rotation)
{
	object->_Data->transform.rotation = rotation;

	switch (object->_Data->type)
	{
	case RECTANGLE:
		sfRectangleShape_setRotation((sfRectangleShape*)object->_Data->drawable, rotation);
		break;
	case CIRCLE:
		sfCircleShape_setRotation((sfCircleShape*)object->_Data->drawable, rotation);
		break;
	case SPRITE:
		sfSprite_setRotation((sfSprite*)object->_Data->drawable, rotation);
		break;
	default:
		break;
	}
}

void UIObject_SetColor(UIObject* object, sfColor color)
{
	object->_Data->color = color;

	switch (object->_Data->type)
	{
	case RECTANGLE:
		sfRectangleShape_setFillColor((sfRectangleShape*)object->_Data->drawable, color);
		break;
	case CIRCLE:
		sfCircleShape_setFillColor((sfCircleShape*)object->_Data->drawable, color);
		break;
	case SPRITE:
		sfSprite_setColor((sfSprite*)object->_Data->drawable, color);
		break;
	default:
		break;
	}
}

void UIObject_SetDrawable(UIObject* object, sfDrawable* shape)
{
	object->_Data->drawable = shape;
}

void UIObject_SetTexture(UIObject* object, sfTexture* texture, sfBool reset_rect)
{	
	switch (object->_Data->type)
	{
	case RECTANGLE:
		sfRectangleShape_setTexture((sfRectangleShape*)object->_Data->drawable, texture, reset_rect);
		break;
	case CIRCLE:
		sfCircleShape_setTexture((sfCircleShape*)object->_Data->drawable, texture, reset_rect);
		break;
	case SPRITE:
		sfSprite_setTexture((sfSprite*)object->_Data->drawable, texture, reset_rect);
		break;
	default:
		break;
	}
}

void UIObject_SetTextureRect(UIObject* object, sfIntRect rect)
{

	switch (object->_Data->type)
	{
	case RECTANGLE:
		sfRectangleShape_setTextureRect((sfRectangleShape*)object->_Data->drawable, rect);
		break;
	case CIRCLE:
		sfCircleShape_setTextureRect((sfCircleShape*)object->_Data->drawable, rect);
		break;
	case SPRITE:
		sfSprite_setTextureRect((sfSprite*)object->_Data->drawable, rect);
		break;
	default:
		break;
	}
}

sfVector2f UIObject_GetPosition(UIObject* object)
{
	return object->_Data->transform.position;
}

sfVector2f UIObject_GetScale(UIObject* object) 
{
	return object->_Data->transform.scale;
}

sfVector2f UIObject_GetOrigin(UIObject* object)
{
	return object->_Data->transform.origin;
}

sfVector2f UIObject_GetSize(UIObject* object)
{
	return object->_Data->transform.size;
}

float UIObject_GetRotation(UIObject* object)
{
	return object->_Data->transform.rotation;
}

sfColor UIObject_GetColor(UIObject* object)
{
	return object->_Data->color;
}

sfDrawable* UIObject_GetDrawable(UIObject* object)
{
	return object->_Data->drawable;
}

sfTexture* UIObject_GetTexture(UIObject* object)
{
	switch (object->_Data->type)
	{
	case RECTANGLE:
		return sfRectangleShape_getTexture((sfRectangleShape*)object->_Data->drawable);
		break;
	case CIRCLE:
		return sfCircleShape_getTexture((sfCircleShape*)object->_Data->drawable);
		break;
	case SPRITE:
		return sfSprite_getTexture((sfSprite*)object->_Data->drawable);
		break;
	default:
		break;
	}
}

sfIntRect UIObject_GetTextureRect(UIObject* object)
{
	switch (object->_Data->type)
	{
	case RECTANGLE:
		return sfRectangleShape_getTextureRect((sfRectangleShape*)object->_Data->drawable);
		break;
	case CIRCLE:
		return sfCircleShape_getTextureRect((sfCircleShape*)object->_Data->drawable);
		break;
	case SPRITE:
		return sfSprite_getTextureRect((sfSprite*)object->_Data->drawable);
		break;
	default:
		break;
	}
}


void UIObject_Update(UIObject* object, WindowManager* window)
{
	sfVector2f mousePos = window->GetMousePos(window);
	sfVector2f objectPos = object->_Data->transform.position;
	sfVector2f objectSize = object->_Data->transform.size;


	if ((object->_Data->type == CIRCLE && PointInCircle(mousePos, objectPos, objectSize.x)) || 
		(object->_Data->type != CIRCLE && PointInRectangle(mousePos,(sfFloatRect){objectPos.x,objectPos.y,objectSize.x,objectSize.y})))
	{
		object->isHover = sfTrue;
		if (sfKeyboard_isKeyDown(object->_Data->key_button_trigger))
		{
			object->isClicked = sfTrue;
		}
		else
		{
			object->isClicked = sfFalse;
		}
	}

}

void UIObject_Render(UIObject* object, WindowManager* window, sfRenderStates* state)
{
	switch (object->_Data->type)
	{
	case RECTANGLE:
		return sfRenderWindow_drawRectangleShape(window->GetWindow(window), (sfRectangleShape*)object->_Data->drawable, state);
		break;
	case CIRCLE:
		return sfRenderWindow_drawCircleShape(window->GetWindow(window), (sfCircleShape*)object->_Data->drawable, state);
		break;
	case SPRITE:
		return sfRenderWindow_drawSprite(window->GetWindow(window), (sfSprite*)object->_Data->drawable, state);
		break;
	default:
		break;
	}
}

