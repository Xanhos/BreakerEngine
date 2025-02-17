/*
	Author: GRALLAN Yann

	Description: An advanced game engine for CSFML

	Date: 2025/01/22

	MIT License

	Copyright (c) 2025 GRALLAN Yann


	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "SpriteManager.h"
#include "MemoryManagement.h"


typedef struct SpriteHolder SpriteHolder;
struct SpriteHolder
{
	sfSprite* m_sprite;
	char* name;
	sfBool m_is_visible;
};

struct SpriteManager_Data
{
	stdList* m_sprite_list;
};


sfVector2f sfSprite_GetTopLeftPos(const sfSprite* sprite)
{
	return SubVector2f(sfSprite_getPosition(sprite), sfSprite_getOrigin(sprite));
}

sfVector2f sfSprite_GetCenterPos(const sfSprite* sprite)
{
	return AddVector2f(sfSprite_GetTopLeftPos(sprite), sfVector2f_Create(sfSprite_getGlobalBounds(sprite).width * sfSprite_getScale(sprite).x / 2.f, sfSprite_getGlobalBounds(sprite).height * sfSprite_getScale(sprite).y / 2.f));
}

static SpriteHolder CreateSpriteHolder(const char* name, const sfTexture* texture, sfBool reset_rect)
{
	SpriteHolder sprite_holder;
	sprite_holder.m_is_visible = sfTrue;
	sprite_holder.m_sprite = sfSprite_create();
	sprite_holder.name = StrAllocNCopy(name);
	sfSprite_setTexture(sprite_holder.m_sprite, texture, reset_rect);
	return sprite_holder;
}

static sfSprite* AddNewSprite(const SpriteManager* sprite_manager, const char* name, const sfTexture* texture, sfBool reset_rect)
{
	stdList* sprite_list = sprite_manager->_Data->m_sprite_list;
	SpriteHolder sprite_holder = CreateSpriteHolder(name, texture, reset_rect);
	sprite_list->push_back(sprite_list, &sprite_holder);
	return sprite_holder.m_sprite;
}

static void Draw(const SpriteManager* sprite_manager, WindowManager* window, sfRenderStates* states)
{
	FOR_EACH_LIST(sprite_manager->_Data->m_sprite_list, SpriteHolder, i, tmp,
		if (tmp->m_is_visible)
			window->DrawSprite(window, tmp->m_sprite, states);
			)
}

static void DestroySpriteManager(SpriteManager** sprite_manager)
{
	SpriteManager* s_manager = *sprite_manager;

	FOR_EACH_LIST(s_manager->_Data->m_sprite_list, SpriteHolder, it, tmp,
		free_d(tmp->name);
	sfSprite_destroy(tmp->m_sprite);
		)
		s_manager->_Data->m_sprite_list->destroy(&s_manager->_Data->m_sprite_list);
	free_d(s_manager->_Data);
	free_d(s_manager);
	*sprite_manager = NULL;
}

static sfSprite* GetSpriteByName(const SpriteManager* sprite_manager, const char* name)
{
	FOR_EACH_LIST(sprite_manager->_Data->m_sprite_list, SpriteHolder, it, tmp,
		if (strcmp(tmp->name, name) == 0)
			return tmp->m_sprite;
			)
		return NULL;
}

static sfSprite* GetSpriteByIndex(const SpriteManager* sprite_manager, int index)
{
	FOR_EACH_LIST(sprite_manager->_Data->m_sprite_list, SpriteHolder, it, tmp,
		if (index == it)
			return tmp->m_sprite;
			)
		return NULL;
}

static size_t GetSize(SpriteManager* sprite_manager)
{
	return sprite_manager->_Data->m_sprite_list->size(sprite_manager->_Data->m_sprite_list);
}

static void RemoveSpriteByName(const SpriteManager* sprite_manager, const char* name)
{
	FOR_EACH_LIST(sprite_manager->_Data->m_sprite_list, SpriteHolder, it, tmp,
		if (strcmp(tmp->name, name) == 0)
		{
			sprite_manager->_Data->m_sprite_list->erase(sprite_manager->_Data->m_sprite_list, it);
			return;
		}
			)
}

static void RemoveSpriteByIndex(const SpriteManager* sprite_manager, int index)
{
	FOR_EACH_LIST(sprite_manager->_Data->m_sprite_list, SpriteHolder, it, tmp,
		if (index == it)
		{
			sprite_manager->_Data->m_sprite_list->erase(sprite_manager->_Data->m_sprite_list, it);
			return;
		}
			)
}

static void SetSpriteIsVisibleByName(const SpriteManager* sprite_manager, sfBool is_visible, const char* name)
{
	FOR_EACH_LIST(sprite_manager->_Data->m_sprite_list, SpriteHolder, it, tmp,
		if (strcmp(tmp->name, name) == 0)
			tmp->m_is_visible = is_visible;
			)
}

static void SetSpriteIsVisibleById(const SpriteManager* sprite_manager, sfBool is_visible, int id)
{
	FOR_EACH_LIST(sprite_manager->_Data->m_sprite_list, SpriteHolder, it, tmp,
		if (id == it)
			tmp->m_is_visible = is_visible;
			)
}

SpriteManager* CreateSpriteManager(void)
{
	SpriteManager* sprite_manager = calloc_d(SpriteManager, 1);
	SpriteManager_Data* sprite_manager_data = calloc_d(SpriteManager_Data, 1);
	assert(sprite_manager);
	assert(sprite_manager_data);
	sprite_manager_data->m_sprite_list = STD_LIST_CREATE(SpriteHolder, 0);


	sprite_manager->_Data = sprite_manager_data;

	sprite_manager->destroy = &DestroySpriteManager;
	sprite_manager->draw = &Draw;
	sprite_manager->get_by_id = &GetSpriteByIndex;
	sprite_manager->get_by_name = &GetSpriteByName;
	sprite_manager->push_back = &AddNewSprite;
	sprite_manager->size = &GetSize;
	sprite_manager->remove_by_id = &RemoveSpriteByIndex;
	sprite_manager->remove_by_name = &RemoveSpriteByName;
	sprite_manager->sprite_is_visible_by_name = &SetSpriteIsVisibleByName;
	sprite_manager->sprite_is_visible_by_id = &SetSpriteIsVisibleById;

	return sprite_manager;
}