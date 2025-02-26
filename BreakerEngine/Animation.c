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
#include "Animation.h"
#include "CParser/CParser.h"
#include "MemoryManagement.h"

struct SimpleAnim_Data
{
	Animation_Key* anim;
	sfSprite* renderer;
};

struct Animation_Key_Data
{
	char* m_name;

	sfIntRect m_int_rect;
	sfIntRect m_current_rect;
	int m_number_of_line;
	int m_frame_per_line;

	int m_total_frame;
	int m_current_frame;

	float m_frame_time;
	float m_frame_timer;

	sfBool m_has_finish_anim;
};



struct Animation_Data
{
	stdList* m_key_anim_list;
	Animation_Key* m_current_anim_key;
	char* m_name;
	sfTexture* m_texture;
	sfRectangleShape* m_renderer;

	sfBool m_animation_is_paused;
	sfBool m_animation_is_reversed;
	sfBool m_stop_at_last_frame;
};

static void UpdateAnimationKeyRect(Animation_Key* anim_key)
{
	Animation_Key_Data* anim = anim_key->_Data;
	sfIntRect current_rect = {
		.left = anim->m_int_rect.left + anim->m_int_rect.width * (anim->m_current_frame % anim->m_frame_per_line),
		.top = anim->m_int_rect.top + anim->m_int_rect.height * (anim->m_number_of_line ? (anim->m_current_frame / anim->m_frame_per_line) : 0),
		.width = anim->m_int_rect.width,
		.height = anim->m_int_rect.height
	};
	anim->m_current_rect = current_rect;
}

static void UpdateAnimationKey(Animation_Key* anim_key, sfBool animation_is_paused, sfBool animation_is_reverse, sfBool stop_at_last_frame, float deltaTime)
{
	Animation_Key_Data* anim = anim_key->_Data;
	anim->m_frame_timer += deltaTime;

	if (anim->m_frame_timer >= anim->m_frame_time)
	{
		if ((!animation_is_paused && !stop_at_last_frame) || (!animation_is_paused && stop_at_last_frame && anim->m_current_frame != anim->m_total_frame - 1))
		{
			animation_is_reverse ? anim->m_current_frame-- : anim->m_current_frame++;

			if (anim->m_has_finish_anim && animation_is_reverse)
			{
				anim->m_current_frame = anim->m_total_frame - 1;
				anim->m_has_finish_anim = sfFalse;
			}
			else if (anim->m_current_frame < 0 && animation_is_reverse)
			{
				anim->m_current_frame = 0;
				anim->m_has_finish_anim = sfTrue;
				return;
			}
			else if (anim->m_current_frame > anim->m_total_frame - 1 && !animation_is_reverse && !anim->m_has_finish_anim)
			{
				anim->m_current_frame = anim->m_total_frame - 1;
				anim->m_has_finish_anim = sfTrue;
				return;
			}
			else if (anim->m_has_finish_anim && !animation_is_reverse)
			{
				anim->m_current_frame = 0;
				anim->m_has_finish_anim = sfFalse;
			}
			anim->m_frame_timer = 0.f;
		}
	}

	anim->m_has_finish_anim = sfFalse;


	UpdateAnimationKeyRect(anim_key);
}


static int GetTotalFrame(Animation_Key* anim_key)
{
	return anim_key->_Data->m_total_frame;
}

static int GetCurrentFrame(Animation_Key* anim_key)
{
	return anim_key->_Data->m_current_frame;
}

static sfIntRect GetCurrentRect(Animation_Key* anim_key)
{
	return anim_key->_Data->m_current_rect;
}

static sfBool HasFinishAnim(Animation_Key* anim_key)
{
	return anim_key->_Data->m_has_finish_anim;
}

static void SetCurrentFrame(Animation_Key* anim_key, int frame)
{
	anim_key->_Data->m_current_frame = frame;
	anim_key->_Data->m_frame_timer = 0.f;
	UpdateAnimationKeyRect(anim_key);
}

static const char* GetAnimationKeyName(Animation_Key* anim_key)
{
	return anim_key->_Data->m_name;
}

static sfBool IsRevert(Animation* anim)
{
	return anim->_Data->m_animation_is_reversed;
}

static sfBool IsPaused(Animation* anim)
{
	return anim->_Data->m_animation_is_paused;
}
static sfBool IsStoppedAtLastFrame(Animation* anim)
{
	return anim->_Data->m_stop_at_last_frame;
}


Animation_Key* CreateAnimationKey(const char* name, sfIntRect rect, int number_of_line, int frame_per_line, int total_frame, float frame_time)
{
	Animation_Key* animation_key = calloc_d(Animation_Key, 1);
	Animation_Key_Data* animation_key_data = calloc_d(Animation_Key_Data, 1);
	assert(animation_key);
	assert(animation_key_data);
	animation_key_data->m_name = StrAllocNCopy(name);
	animation_key_data->m_current_frame = 0;
	animation_key_data->m_total_frame = total_frame;
	animation_key_data->m_frame_per_line = frame_per_line;
	animation_key_data->m_number_of_line = number_of_line;
	animation_key_data->m_frame_time = frame_time;
	animation_key_data->m_frame_timer = 0.f;
	animation_key_data->m_int_rect = rect;

	animation_key->GetCurrentFrame = &GetCurrentFrame;
	animation_key->GetTotalFrame = &GetTotalFrame;
	animation_key->GetCurrentRect = &GetCurrentRect;
	animation_key->SetCurrentFrame = &SetCurrentFrame;
	animation_key->GetAnimationKeyName = &GetAnimationKeyName;
	animation_key->HasFinishAnim = &HasFinishAnim;

	animation_key->_Data = animation_key_data;

	return animation_key;
}


static void ResetAnimationKey(Animation_Key* anim_key)
{
	anim_key->_Data->m_current_frame = 0;
	anim_key->_Data->m_frame_timer = 0.f;
	anim_key->_Data->m_has_finish_anim = sfFalse;
}



static void SetAnimationParameters(Animation* anim, sfBool isPaused, sfBool isRevert, sfBool isStoppingAtLastFrame)
{
	anim->_Data->m_animation_is_paused = isPaused;
	anim->_Data->m_animation_is_reversed = isRevert;
	anim->_Data->m_stop_at_last_frame = isStoppingAtLastFrame;
}

static sfRectangleShape* GetRenderer(Animation* anim)
{
	return anim->_Data->m_renderer;
}


static Animation_Key* AddAnimationKey(Animation const* anim, Animation_Key* anim_key)
{
	anim->_Data->m_key_anim_list->push_back(anim->_Data->m_key_anim_list, &anim_key);
	return anim_key;
}

static Animation_Key* SelectAnimKey(Animation* anim, const char* name)
{
	printf_d("Selecting key : %s\n", name);
	printf_d("List size : %d\n", anim->_Data->m_key_anim_list->size(anim->_Data->m_key_anim_list));
	FOR_EACH_LIST_POINTER(anim->_Data->m_key_anim_list, Animation_Key*, it, tmp,
		if (strcmp((tmp)->_Data->m_name, name) == 0)
		{
			if (anim->_Data->m_current_anim_key != NULL)
				ResetAnimationKey(anim->_Data->m_current_anim_key);
			anim->_Data->m_current_anim_key = tmp;
			sfRectangleShape_setSize(anim->_Data->m_renderer, (sfVector2f) { (float)(tmp)->_Data->m_int_rect.width, (float)(tmp)->_Data->m_int_rect.height });
			sfRectangleShape_setTextureRect(anim->_Data->m_renderer, (tmp)->_Data->m_int_rect);
			return tmp;
		}
			)
		printf_d("Animation key : %s not found", name);
	return NULL;
}

static Animation_Key* GetCurrentAnimationKey(Animation* anim)
{
	return anim->_Data->m_current_anim_key;
}

static void UpdateAnim(Animation* anim, float deltaTime)
{
	UpdateAnimationKey(anim->_Data->m_current_anim_key, anim->_Data->m_animation_is_paused, anim->_Data->m_animation_is_reversed,
		anim->_Data->m_stop_at_last_frame, deltaTime);

	sfRectangleShape_setSize(anim->_Data->m_renderer, (sfVector2f) { (float)anim->_Data->m_current_anim_key->_Data->m_current_rect.width, (float)anim->_Data->m_current_anim_key->_Data->m_current_rect.height });
	sfRectangleShape_setTextureRect(anim->_Data->m_renderer, anim->_Data->m_current_anim_key->_Data->m_current_rect);
}

void sfRenderWindow_drawAnimation(WindowManager* window, Animation* anim, sfRenderStates* states)
{
	window->DrawRectangleShape(window, anim->_Data->m_renderer, states);
}


static void DestroyAnimation(Animation** anim_data)
{
	Animation* anim = *anim_data;
	FOR_EACH_LIST_POINTER(anim->_Data->m_key_anim_list, Animation_Key*, it, tmp,
		free_d(tmp->_Data->m_name);
	free_d(tmp->_Data);
	free_d(tmp);
		);
	free_d(anim->_Data->m_name);
	free_d(anim->_Data);
	free_d(anim);
	*anim_data = NULL;
}


Animation* CreateAnimation(const char* name, sfTexture* texture)
{
	Animation* anim = calloc_d(Animation, 1);
	Animation_Data* anim_data = calloc_d(Animation_Data, 1);
	assert(anim);
	assert(anim_data);
	anim_data->m_name = StrAllocNCopy(name);
	anim_data->m_key_anim_list = STD_LIST_CREATE_POINTER(Animation_Key*, 0);
	anim_data->m_texture = texture;
	anim_data->m_renderer = sfRectangleShape_create();
	sfRectangleShape_setSize(anim_data->m_renderer, (sfVector2f) { (float)sfTexture_getSize(texture).x, (float)sfTexture_getSize(texture).y });
	sfRectangleShape_setTexture(anim_data->m_renderer, texture, sfTrue);

	anim->_Data = anim_data;

	anim->Update = &UpdateAnim;
	anim->AddAnimationKey = &AddAnimationKey;
	anim->SelectAnimationKey = &SelectAnimKey;
	anim->SetAnimationParameters = &SetAnimationParameters;
	anim->IsPaused = &IsPaused;
	anim->IsRevert = &IsRevert;
	anim->IsStoppedAtLastFrame = &IsStoppedAtLastFrame;
	anim->Destroy = &DestroyAnimation;
	anim->GetRenderer = &GetRenderer;
	anim->GetCurrentAnimationKey = &GetCurrentAnimationKey;

	return anim;
}

Animation* CreateAnimationFromFile(const char* path, sfTexture* texture)
{


	Animation* tmp = CreateAnimation("tmp", texture);
	parser_reader* pr = parser_reader_create(path);

	char* animName = NULL;

	while (parser_reader_still_reading(pr))
	{
		parser_reader_read(pr);
		if (parser_reader_actual_object_is(pr, "Animation"))
		{
			parser_reader_get_value_string(pr, "Name", &animName);
		}
		if (parser_reader_actual_object_is(pr, "KeyAnim"))
		{
			sfIntRect animRect = { 0,0,0,0 };
			char* keyAnimName = NULL;
			int lineSize = 0, rowNumber = 0, frameMax = 0;
			float frameTime = 0.f;
			while (parser_reader_actual_object_is(pr, "KeyAnim"))
			{
				parser_reader_read(pr);
				parser_reader_get_value_string(pr, "Name", &keyAnimName);
				parser_reader_get_value_int(pr, "Rect_left", &animRect.left);
				parser_reader_get_value_int(pr, "Rect_top", &animRect.top);
				parser_reader_get_value_int(pr, "Rect_width", &animRect.width);
				parser_reader_get_value_int(pr, "Rect_height", &animRect.height);
				parser_reader_get_value_int(pr, "LineSize", &lineSize);
				parser_reader_get_value_int(pr, "RowNumber", &rowNumber);
				parser_reader_get_value_int(pr, "FrameMax", &frameMax);
				parser_reader_get_value_float(pr, "FrameTime", &frameTime);
			}

			Animation_Key* tmpAnimKey = CreateAnimationKey(keyAnimName, animRect, lineSize, rowNumber, frameMax, frameTime);
			//free(keyAnimName); free make the code crash
			tmp->AddAnimationKey(tmp, tmpAnimKey);
		}
	}

	return tmp;
}

static void SimpleAnimUpdate(SimpleAnim* anim, float deltaTime)
{
	UpdateAnimationKey(anim->_Data->anim, sfFalse, sfFalse, sfFalse, deltaTime);
}

static void SimpleAnimDraw(SimpleAnim* anim, sfRenderWindow* window, sfRenderStates* state)
{
	sfSprite_setTextureRect(anim->_Data->renderer, anim->_Data->anim->_Data->m_current_rect);
	sfRenderWindow_drawSprite(window, anim->_Data->renderer, state);
}

static sfSprite* SimpleAnimGetRenderer(SimpleAnim* anim)
{
	return anim->_Data->renderer;
}

static void SimpleAnimDestroy(SimpleAnim** anim)
{
	sfSprite_destroy((*anim)->_Data->renderer);
	free_d(((*anim)->_Data->anim->_Data->m_name));
	free_d(((*anim)->_Data->anim->_Data));
	free_d(((*anim)->_Data->anim));
	free_d(((*anim)->_Data));
	free_d(((*anim)));
	*anim = NULL;
}

SimpleAnim* CreateSimpleAnim(sfTexture* texture, sfIntRect rect, int line_number, int line_frame_number, int total_frame, float frame_time)
{
	SimpleAnim* anim = calloc_d(SimpleAnim, 1);;
	assert(anim);
	anim->_Data = calloc_d(SimpleAnim_Data, 1);
	assert(anim->_Data);
	anim->_Data->anim = CreateAnimationKey("SimpleAnim", rect, line_number, line_frame_number, total_frame, frame_time);
	anim->_Data->renderer = sfSprite_create();
	sfSprite_setTexture(anim->_Data->renderer, texture, sfTrue);

	anim->Update = &SimpleAnimUpdate;
	anim->Draw = &SimpleAnimDraw;
	anim->Destroy = &SimpleAnimDestroy;
	anim->GetRenderer = &SimpleAnimGetRenderer;

	return anim;
}