#include "UI.h"
#include "Vector.h"
#include "MemoryManagement.h"

typedef enum
{
	RECTANGLE,
	CIRCLE,
	SPRITE
} UIObject_Type;

struct UIObject_Data
{
	char* name;
	sfDrawable* drawable;
	UIObject_Transform transform;
	sfColor color;
	sfMouseButton mouse_button_trigger;
	sfKeyCode key_button_trigger;
	UIUpdateFunction callback;
	UIObject_Type type;
	sfBool is_visible;
};

struct UIObjectManager_Data
{
	stdVector* uiobject_vector;
};



static void UIObject_SetPosition(UIObject* object, sfVector2f position)
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

static void UIObject_SetScale(UIObject* object, sfVector2f scale)
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

static void UIObject_SetOrigin(UIObject* object, sfVector2f origin)
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

static void UIObject_SetSize(UIObject* object, sfVector2f size)
{
	object->_Data->transform.size = size;
	sfVector2f spriteSize = (sfVector2f){ (float)sfSprite_getTextureRect((sfSprite*)object->_Data->drawable).width, (float)sfSprite_getTextureRect((sfSprite*)object->_Data->drawable).height };
	switch (object->_Data->type)
	{
	case RECTANGLE:
		sfRectangleShape_setSize((sfRectangleShape*)object->_Data->drawable, size);
		break;
	case CIRCLE:
		sfCircleShape_setRadius((sfCircleShape*)object->_Data->drawable, size.x);
		sfCircleShape_setOrigin((sfCircleShape*)object->_Data->drawable, (sfVector2f) { size.x, size.x });
		object->_Data->transform.origin = sfCircleShape_getOrigin((sfCircleShape*)object->_Data->drawable);
		break;
	case SPRITE:
		sfSprite_setScale((sfSprite*)object->_Data->drawable, (sfVector2f) { size.x / spriteSize.x, size.y / spriteSize.y });
		break;
	default:
		break;
	}
}

static void UIObject_SetRotation(UIObject* object, float rotation)
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

static void UIObject_SetColor(UIObject* object, sfColor color)
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

static void UIObject_SetDrawable(UIObject* object, sfDrawable* shape)
{
	switch (object->_Data->type)
	{
	case RECTANGLE:
		sfRectangleShape_destroy((sfRectangleShape*)object->_Data->drawable);
		break;
	case CIRCLE:
		sfCircleShape_destroy((sfCircleShape*)object->_Data->drawable);
		break;
	case SPRITE:
		sfSprite_destroy((sfSprite*)object->_Data->drawable);
		break;
	default:
		break;
	}
	object->_Data->drawable = shape;
}

static void UIObject_SetTexture(UIObject* object, sfTexture* texture, sfBool reset_rect)
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
		object->_Data->transform.size = reset_rect ? sfVector2f_Create((float)sfTexture_getSize(texture).x, (float)sfTexture_getSize(texture).y) : object->_Data->transform.size;
		break;
	default:
		break;
	}
}

static void UIObject_SetTextureRect(UIObject* object, sfIntRect rect)
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
		object->_Data->transform.size = sfVector2f_Create((float)rect.width, (float)rect.height);
		break;
	default:
		break;
	}
}

static void UIObject_SetIsVisible(UIObject* object, sfBool is_visible)
{
	object->_Data->is_visible = is_visible;
}

static const char* UIObject_GetName(UIObject* object)
{
	return object->_Data->name;
}

static sfVector2f UIObject_GetPosition(UIObject* object)
{
	return object->_Data->transform.position;
}

static sfVector2f UIObject_GetScale(UIObject* object)
{
	return object->_Data->transform.scale;
}

static sfVector2f UIObject_GetOrigin(UIObject* object)
{
	return object->_Data->transform.origin;
}

static sfVector2f UIObject_GetSize(UIObject* object)
{
	return object->_Data->transform.size;
}

static float UIObject_GetRotation(UIObject* object)
{
	return object->_Data->transform.rotation;
}

static sfColor UIObject_GetColor(UIObject* object)
{
	return object->_Data->color;
}

static sfDrawable* UIObject_GetDrawable(UIObject* object)
{
	return object->_Data->drawable;
}

static const sfTexture* UIObject_GetTexture(UIObject* object)
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
		return NULL;
		break;
	}
}

static sfIntRect UIObject_GetTextureRect(UIObject* object)
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
		return (sfIntRect) { 0, 0, 0, 0 };
		break;
	}
}

static sfBool UIObject_GetIsVisible(UIObject* object)
{
	return object->_Data->is_visible;
}

static void UIObject_SetUpdateFunction(UIObject* object, UIUpdateFunction callback)
{
	object->_Data->callback = callback;
}

static sfBool UIObject_NameIs(UIObject* object, const char* name)
{
	return strcmp(object->_Data->name, name) == 0 ? sfTrue : sfFalse;
}

static void UIObject_Update(UIObject* object, WindowManager* window)
{
	sfVector2f mousePos = window->GetMousePos(window);
	sfVector2f objectPos = object->_Data->transform.position;
	sfVector2f objectSize = object->_Data->transform.size;

	object->isClicked = sfFalse;
	object->isHover = sfFalse;

	if ((object->_Data->type == CIRCLE && PointInCircle(mousePos, objectPos, objectSize.x, sfTrue)) ||
		(object->_Data->type != CIRCLE && PointInRectangle(mousePos, (sfFloatRect) { objectPos.x - object->_Data->transform.origin.x, objectPos.y - object->_Data->transform.origin.y, objectSize.x, objectSize.y }, sfTrue)))
	{
		object->isHover = sfTrue;
		if (sfKeyboard_isKeyDown(object->_Data->key_button_trigger) || sfMouse_isButtonDown(object->_Data->mouse_button_trigger))
		{
			object->isClicked = sfTrue;
		}
	}
	if (object->_Data->callback != NULL)
		object->_Data->callback(object, window);

}

static void UIObject_Render(WindowManager* window, UIObject* object, sfRenderStates* state)
{
	if (!object->_Data->is_visible)
		return;
	switch (object->_Data->type)
	{
	case RECTANGLE:
		window->DrawRectangleShape((window), (sfRectangleShape*)object->_Data->drawable, state);
		break;
	case CIRCLE:
		window->DrawCircleShape((window), (sfCircleShape*)object->_Data->drawable, state);
		break;
	case SPRITE:
		window->DrawSprite((window), (sfSprite*)object->_Data->drawable, state);
		break;
	default:
		break;
	}
}

static void UIObject_Destroy(UIObject** object)
{
	UIObject_Data* data = (*object)->_Data;
	UIObject_SetDrawable(*object, NULL);
	free_d(data->name);
	free_d(data);
	free_d(*object);
	*object = NULL;
}


static UIObject* CreateUIObject(sfDrawable* shape, char* name, UIObject_Type type, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger)
{
	UIObject* object = (UIObject*)malloc(sizeof(UIObject));
	assert(object);
	object->_Data = (UIObject_Data*)malloc(sizeof(UIObject_Data));
	assert(object->_Data);

	if (shape == NULL)
	{
		switch (type)
		{
		case RECTANGLE:
			shape = (sfDrawable*)sfRectangleShape_create();
			break;
		case CIRCLE:
			shape = (sfDrawable*)sfCircleShape_create();
			break;
		case SPRITE:
			shape = (sfDrawable*)sfSprite_create();
			break;
		default:
			break;
		}
	}

	object->_Data->drawable = shape;
	object->_Data->type = type;
	object->_Data->color = sfWhite;
	object->_Data->mouse_button_trigger = mouse_button_trigger;
	object->_Data->key_button_trigger = key_button_trigger;
	object->_Data->name = StrAllocNCopy(name);
	object->_Data->callback = NULL;
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
		sfCircleShape_setOrigin((sfCircleShape*)shape, (sfVector2f) { sfCircleShape_getRadius((sfCircleShape*)shape), sfCircleShape_getRadius((sfCircleShape*)shape) });
		tmpTransform.origin = sfCircleShape_getOrigin((sfCircleShape*)shape);
		tmpTransform.size = (sfVector2f){ sfCircleShape_getRadius((sfCircleShape*)shape), sfCircleShape_getRadius((sfCircleShape*)shape) };
		tmpTransform.rotation = sfCircleShape_getRotation((sfCircleShape*)shape);
		object->_Data->transform = tmpTransform;
		break;
	case SPRITE:
		tmpTransform.position = sfSprite_getPosition((sfSprite*)shape);
		tmpTransform.scale = sfSprite_getScale((sfSprite*)shape);
		tmpTransform.origin = sfSprite_getOrigin((sfSprite*)shape);
		tmpTransform.size = (sfVector2f){ (float)sfSprite_getTextureRect((sfSprite*)shape).width, (float)sfSprite_getTextureRect((sfSprite*)shape).height };
		tmpTransform.rotation = sfSprite_getRotation((sfSprite*)shape);
		object->_Data->transform = tmpTransform;
		break;
	default:
		break;
	}

	object->setColor = &UIObject_SetColor;
	object->setDrawable = &UIObject_SetDrawable;
	object->setPosition = &UIObject_SetPosition;
	object->setScale = &UIObject_SetScale;
	object->setOrigin = &UIObject_SetOrigin;
	object->setSize = &UIObject_SetSize;
	object->setRotation = &UIObject_SetRotation;
	object->setTexture = &UIObject_SetTexture;
	object->setTextureRect = &UIObject_SetTextureRect;
	object->setUpdateFunction = &UIObject_SetUpdateFunction;
	object->setIsVisible = &UIObject_SetIsVisible;

	object->getName = &UIObject_GetName;
	object->getPosition = &UIObject_GetPosition;
	object->getScale = &UIObject_GetScale;
	object->getOrigin = &UIObject_GetOrigin;
	object->getSize = &UIObject_GetSize;
	object->getRotation = &UIObject_GetRotation;
	object->getColor = &UIObject_GetColor;
	object->getDrawable = &UIObject_GetDrawable;
	object->getTexture = &UIObject_GetTexture;
	object->getTextureRect = &UIObject_GetTextureRect;
	object->getIsVisible = &UIObject_GetIsVisible;

	object->nameIs = &UIObject_NameIs;

	object->update = &UIObject_Update;
	object->render = &UIObject_Render;
	object->destroy = &UIObject_Destroy;



	return object;
}


UIObject* CreateUIObjectFromRectangle(sfRectangleShape* shape, char* name, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger)
{
	return CreateUIObject((sfDrawable*)shape, name, RECTANGLE, mouse_button_trigger, key_button_trigger);
}

UIObject* CreateUIObjectFromCircleShape(sfCircleShape* shape, char* name, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger)
{
	return CreateUIObject((sfDrawable*)shape, name, CIRCLE, mouse_button_trigger, key_button_trigger);
}

UIObject* CreateUIObjectFromSprite(sfSprite* shape, char* name, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger)
{
	return CreateUIObject((sfDrawable*)shape, name, SPRITE, mouse_button_trigger, key_button_trigger);
}

static UIObject* UIObjectManager_Add(UIObjectManager* manager, UIObject* object)
{
	manager->_Data->uiobject_vector->push_back(manager->_Data->uiobject_vector, &object);
	return object;
}

static UIObject* UIObjectManager_GetFromName(UIObjectManager* manager, const char* name)
{
	UIObject* result = ((void*)0); for (int i = 0; i < manager->_Data->uiobject_vector->size(manager->_Data->uiobject_vector); i++)
	{
		UIObject** it_ = ((UIObject**)manager->_Data->uiobject_vector->getData(manager->_Data->uiobject_vector, i));
		UIObject* it = *it_;
		if (strcmp(name, (it)->_Data->name) == 0)
		{
			result = it; break;
		}
	};
	return result;
}

static UIObject* UIObjectManager_GetFromIndex(UIObjectManager* manager, int index)
{
	return STD_GETDATA(manager->_Data->uiobject_vector, UIObject, index);
}

static void UIObjectManager_RemoveFromPointer(UIObjectManager* manager, UIObject* object)
{
	for (int i = 0; i < manager->_Data->uiobject_vector->size(manager->_Data->uiobject_vector); i++)
	{
		UIObject* iterator = ((UIObject*)manager->_Data->uiobject_vector->getData(manager->_Data->uiobject_vector, i));
		if (strcmp(iterator->_Data->name, object->_Data->name) == 0)
		{
			UIObject_Destroy(&iterator);
			manager->_Data->uiobject_vector->erase(manager->_Data->uiobject_vector, i);
			break;
		}
	};
}

static void UIObjectManager_RemoveFromIndex(UIObjectManager* manager, int index)
{
	for (int i = 0; i < manager->_Data->uiobject_vector->size(manager->_Data->uiobject_vector); i++)
	{
		UIObject* iterator = ((UIObject*)manager->_Data->uiobject_vector->getData(manager->_Data->uiobject_vector, i));
		if (i == index)
		{
			UIObject_Destroy(&iterator);
			manager->_Data->uiobject_vector->erase(manager->_Data->uiobject_vector, i);
			break;
		}
	};
}

static void UIObjectManager_RemoveFromName(UIObjectManager* manager, char* name)
{
	for (int i = 0; i < manager->_Data->uiobject_vector->size(manager->_Data->uiobject_vector); i++)
	{
		UIObject* iterator = ((UIObject*)manager->_Data->uiobject_vector->getData(manager->_Data->uiobject_vector, i));
		if (strcmp(iterator->_Data->name, name) == 0)
		{
			UIObject_Destroy(&iterator);
			manager->_Data->uiobject_vector->erase(manager->_Data->uiobject_vector, i);
			break;
		}
	};
}

static void UIObjectManager_Update(UIObjectManager* manager, WindowManager* window)
{
	FOR_EACH_POINTER(manager->_Data->uiobject_vector, UIObject, i, it,
		UIObject_Update(it, window);
		);
}

static void UIObjectManager_Render(UIObjectManager* manager, WindowManager* window, sfRenderStates* states)
{
	for (int i = 0; i < manager->_Data->uiobject_vector->size(manager->_Data->uiobject_vector); i++)
	{
		UIObject** it_ = ((UIObject**)manager->_Data->uiobject_vector->getData(manager->_Data->uiobject_vector, i));
		UIObject* it = *it_;
		UIObject_Render(window, it, states);
	};
}

static void UIObjectManager_Destroy(UIObjectManager** manager)
{
	FOR_EACH_POINTER((*manager)->_Data->uiobject_vector, UIObject, i, it,
		UIObject_Destroy(&it);
		);
	(*manager)->_Data->uiobject_vector->destroy(&(*manager)->_Data->uiobject_vector);
	free_d((*manager)->_Data);
	free_d(*manager);
	*manager = NULL;
}



UIObjectManager* CreateUIObjectManager()
{
	UIObjectManager* object = calloc_d(UIObjectManager, 1);
	assert(object);
	object->_Data = calloc_d(UIObjectManager_Data, 1);
	assert(object->_Data);
	object->_Data->uiobject_vector = STD_VECTOR_CREATE_POINTER(UIObject, 0);

	object->push_back = &UIObjectManager_Add;
	object->get_by_name = &UIObjectManager_GetFromName;
	object->get_by_index = &UIObjectManager_GetFromIndex;
	object->remove_by_pointer = &UIObjectManager_RemoveFromPointer;
	object->remove_by_index = &UIObjectManager_RemoveFromIndex;
	object->remove_by_name = &UIObjectManager_RemoveFromName;
	object->update = &UIObjectManager_Update;
	object->draw = &UIObjectManager_Render;
	object->destroy = &UIObjectManager_Destroy;

	return object;
}
