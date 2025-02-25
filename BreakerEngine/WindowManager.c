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
#include "WindowManager.h"
#include "Animation.h"
#include "Particles.h"
#include "MemoryManagement.h"

typedef struct SoundInfo SoundInfo;
struct SoundInfo
{
	char* m_name;
	float m_volume;
};

typedef struct CustomParam CustomParam;
struct CustomParam
{
	char* m_name;
	void* m_param;
	void (*m_param_func)(const WindowManager* window, void* param);
	size_t m_param_size;
};


struct WindowManager_Data
{
	sfRenderWindow* m_window;
	sfRenderTexture* m_render_texture;
	sfSprite* m_renderer;
	char* m_title;
	stdList* m_custom_param_list;
	stdList* m_sound_list;
	Clock* m_window_clock;
	sfView* m_custom_view;

	sfVector2u m_size;
	sfVector2u m_base_size;
	sfBool m_fullscreen;
	sfUint32 m_style;
	sfEvent m_event;
	float m_timer;
};


static sfBool IsFullscreen(const WindowManager* window)
{
	return window->_Data->m_fullscreen;
}

static void ToggleFullscreen(const WindowManager* window)
{
	window->_Data->m_fullscreen = !window->_Data->m_fullscreen;
	sfVector2u size = window->_Data->m_size;
	sfContextSettings settings = sfRenderWindow_getSettings(window->_Data->m_window);
	sfRenderWindow_destroy(window->_Data->m_window);
	if (window->_Data->m_fullscreen)
		window->_Data->m_style |= sfFullscreen;
	else
		window->_Data->m_style &= ~sfFullscreen;

	window->_Data->m_window = sfRenderWindow_create((sfVideoMode) { size.x, size.y, sfVideoMode_getDesktopMode().bitsPerPixel }, window->_Data->m_title, window->_Data->m_style, & settings);
}

#pragma region CUSTOM_PARAM
static void SetCustomParam(const WindowManager* window, const char* name, const void* param)
{
	FOR_EACH_LIST(window->_Data->m_custom_param_list, CustomParam, it, tmp,
		if (!strcmp(tmp->m_name, name))
		{
			memcpy_s(tmp->m_param, tmp->m_param_size, param, tmp->m_param_size);
			tmp->m_param_func(window, tmp->m_param);
			return;
		}
			)
}

static void AddNewCustomParam(const WindowManager* window_manager, const char* name, void (*param_func)(const WindowManager* window, void* param), const void* param_data, const size_t param_size)
{
	CustomParam custom_param;
	FOR_EACH_LIST(window_manager->_Data->m_sound_list, CustomParam, i, it,
		if (strcmp(name, it->m_name) == 0)
			return;
	)
		custom_param.m_name = StrAllocNCopy(name);
	custom_param.m_param_func = param_func;
	custom_param.m_param_size = param_size;
	custom_param.m_param = TrackerCalloc(1, param_size, "C:\\Users\\y.grallan\\Documents\\BreakerEngine\\BreakerEngine\\WindowManager.c", 106);
	memcpy_s(custom_param.m_param, param_size, param_data, param_size);
	window_manager->_Data->m_custom_param_list->push_back(window_manager->_Data->m_custom_param_list, &custom_param);
	SetCustomParam(window_manager, name, param_data);
}

static void* GetCustomParam(const WindowManager* window, const char* name)
{
	FOR_EACH_LIST(window->_Data->m_custom_param_list, CustomParam, it, tmp,
		if (!strcmp(tmp->m_name, name))
		{
			return tmp->m_param;;
		}
			)
		return NULL;
}

static void DestroyCustomParam(WindowManager* window)
{
	FOR_EACH_LIST(window->_Data->m_custom_param_list, CustomParam, it, tmp,
		free_d(tmp->m_name);
	free_d(tmp->m_param);
		)

		window->_Data->m_custom_param_list->destroy(&window->_Data->m_custom_param_list);
}
#pragma endregion

#pragma region SOUND_LIST
static void AddNewSound(const WindowManager* window_manager, const char* name, float volume)
{
	FOR_EACH_LIST(window_manager->_Data->m_sound_list, SoundInfo, i, it,
		if (strcmp(name, it->m_name) == 0)
			return;
			)

		SoundInfo sound_info;
	sound_info.m_name = StrAllocNCopy(name);
	sound_info.m_volume = volume;


	window_manager->_Data->m_sound_list->push_back(window_manager->_Data->m_sound_list, &sound_info);
}

static void SetWindowSound(const WindowManager* window, const char* name, float volume)
{
	FOR_EACH_LIST(window->_Data->m_sound_list, SoundInfo, it, tmp,
		if (!strcmp(tmp->m_name, name))
		{
			tmp->m_volume = volume;
			return;
		}
			)
}

static float GetWindowSound(const WindowManager* window, const char* name)
{
	FOR_EACH_LIST(window->_Data->m_sound_list, SoundInfo, it, tmp,
		if (!strcmp(tmp->m_name, name))
		{
			return tmp->m_volume;
		}
			)
		return -1.f;
}

static void DestroySound(WindowManager* window)
{
	FOR_EACH_LIST(window->_Data->m_sound_list, SoundInfo, it, tmp,
		free_d(tmp->m_name);
		)

		window->_Data->m_sound_list->destroy(&window->_Data->m_sound_list);
}
#pragma endregion 

static float GetWindowTimer(const WindowManager* window)
{
	return window->_Data->m_timer;
}

static void ResetWindowTimer(const WindowManager* window)
{
	window->_Data->m_timer = 0.f;
}

static void RestartWindowClock(const WindowManager* window)
{
	window->_Data->m_window_clock->restartClock(window->_Data->m_window_clock);
	window->_Data->m_timer += window->_Data->m_window_clock->getDeltaTime(window->_Data->m_window_clock);
}

static sfRenderWindow* GetRenderWindow(const WindowManager* window)
{
	return window->_Data->m_window;
}

static sfEvent* GetWindowEvent(const WindowManager* window)
{
	return &window->_Data->m_event;
}

static sfBool PollEvent(const WindowManager* window)
{
	return sfRenderWindow_pollEvent(window->_Data->m_window, &window->_Data->m_event);
}

static sfVector2u GetWindowManagerSize(const WindowManager* window)
{
	return window->_Data->m_size;
}

static sfVector2u GetWindowManagerBaseSize(const WindowManager* window)
{
	return window->_Data->m_base_size;
}


static sfVector2f GetMousePos(const WindowManager* window)
{
	sfVector2f mousepos;
	mousepos.x = (float)sfMouse_getPositionRenderWindow(window->_Data->m_window).x;
	mousepos.y = (float)sfMouse_getPositionRenderWindow(window->_Data->m_window).y;
	return mousepos;
}

static void SetView(const WindowManager* window, sfView* view)
{
	window->_Data->m_custom_view = view;
	sfRenderTexture_setView(window->_Data->m_render_texture, view);
}

static sfView* GetView(const WindowManager* window)
{
	return window->_Data->m_custom_view;
}

static void SetDefaultView(const WindowManager* window)
{
	sfRenderTexture_setView(window->_Data->m_render_texture, sfRenderTexture_getDefaultView(window->_Data->m_render_texture));
}

static sfView* GetDefaultView(const WindowManager* window)
{
	return sfRenderTexture_getDefaultView(window->_Data->m_render_texture);
}

static void Clear(const WindowManager* window, sfColor color)
{
	sfRenderTexture_clear(window->_Data->m_render_texture, color);
	sfRenderWindow_clear(window->_Data->m_window, color);
}

static void Display(const WindowManager* window)
{
	sfRenderTexture_display(window->_Data->m_render_texture);
	window->_Data->m_size = sfRenderWindow_getSize(window->_Data->m_window);
	ScreenScaleFactorX = (float)window->_Data->m_size.x / (float)window->_Data->m_base_size.x;
	ScreenScaleFactorY = (float)window->_Data->m_size.y / (float)window->_Data->m_base_size.y;
	sfSprite_setTexture(window->_Data->m_renderer, sfRenderTexture_getTexture(window->_Data->m_render_texture), sfTrue);
	sfRenderWindow_drawSprite(window->_Data->m_window, window->_Data->m_renderer, NULL);
	sfRenderWindow_display(window->_Data->m_window);
}

static void WindowManagerDrawSprite(const WindowManager* window, const sfSprite* object, const sfRenderStates* state)
{
	sfRenderTexture_drawSprite(window->_Data->m_render_texture, object, state);
}
static void WindowManagerDrawText(const WindowManager* window, const sfText* object, const sfRenderStates* state)
{
	sfRenderTexture_drawText(window->_Data->m_render_texture, object, state);
}

static void WindowManagerDrawShape(const WindowManager* window, const sfShape* object, const sfRenderStates* state)
{
	sfRenderTexture_drawShape(window->_Data->m_render_texture, object, state);
}

static void WindowManagerDrawCircleShape(const WindowManager* window, const sfCircleShape* object, const sfRenderStates* state)
{
	sfRenderTexture_drawCircleShape(window->_Data->m_render_texture, object, state);
}

static void WindowManagerDrawConvexShape(const WindowManager* window, const sfConvexShape* object, const sfRenderStates* state)
{
	sfRenderTexture_drawConvexShape(window->_Data->m_render_texture, object, state);
}

static void WindowManagerDrawRectangleShape(const WindowManager* window, const sfRectangleShape* object, const sfRenderStates* state)
{
	sfRenderTexture_drawRectangleShape(window->_Data->m_render_texture, object, state);
}

static void WindowManagerDrawVertexArray(const WindowManager* window, const sfVertexArray* object, const sfRenderStates* state)
{
	sfRenderTexture_drawVertexArray(window->_Data->m_render_texture, object, state);
}

static void WindowManagerDrawVertexBuffer(const WindowManager* window, const sfVertexBuffer* object, const sfRenderStates* state)
{
	sfRenderTexture_drawVertexBuffer(window->_Data->m_render_texture, object, state);
}

static void WindowManagerDrawPrimitives(const WindowManager* window, const sfVertex* object, size_t vertexCount,sfPrimitiveType type ,const sfRenderStates* state)
{
	sfRenderTexture_drawPrimitives(window->_Data->m_render_texture, object, vertexCount, type, state);
}

static void WindowManagerDrawAnimation(const WindowManager* window, const Animation* object, const sfRenderStates* state)
{
	sfRenderTexture_drawRectangleShape(window->_Data->m_render_texture, object->GetRenderer(object), state);
}

static void WindowManagerDrawParticles(const WindowManager* window, const Particles* object, const sfRenderStates* state)
{
	sfRenderTexture_drawParticles(window->_Data->m_render_texture, object, state);
}

static void DestroyWindowManager(WindowManager** window)
{
	DestroySound(*window);
	DestroyCustomParam(*window);
	WindowManager* tmp = *window;
	free_d(tmp->_Data->m_title);
	tmp->_Data->m_window_clock->destroy(&tmp->_Data->m_window_clock);
	sfRenderWindow_close(tmp->_Data->m_window);
	sfRenderWindow_destroy(tmp->_Data->m_window);
	free_d(tmp->_Data);
	free_d(tmp);
	*window = NULL;
}


WindowManager* CreateWindowManager(const unsigned int width, const unsigned int height, const char* title, const sfUint32 style, const sfContextSettings* settings)
{
	WindowManager* window_manager = calloc_d(WindowManager, 1);
	assert(window_manager);
	WindowManager_Data* window_manager_data = calloc_d(WindowManager_Data, 1);
	assert(window_manager_data);
	window_manager_data->m_size = (sfVector2u){ MIN(width,sfVideoMode_getDesktopMode().width), MIN(height, sfVideoMode_getDesktopMode().height) };
	window_manager_data->m_base_size = (sfVector2u){ width, height };
	window_manager_data->m_render_texture = sfRenderTexture_create(width, height, sfFalse);
	window_manager_data->m_renderer = sfSprite_create();
	window_manager_data->m_window = sfRenderWindow_create((sfVideoMode) { window_manager_data->m_size.x, window_manager_data->m_size.y, sfVideoMode_getDesktopMode().bitsPerPixel }, title, style, settings);
	window_manager_data->m_style = style;
	window_manager_data->m_title = StrAllocNCopy(title);
	window_manager_data->m_fullscreen = sfFullscreen & style ? sfTrue : sfFalse;
	window_manager_data->m_custom_param_list = STD_LIST_CREATE(CustomParam, 0);
	window_manager_data->m_sound_list = STD_LIST_CREATE(SoundInfo, 0);
	window_manager_data->m_window_clock = CreateClock();
	window_manager->_Data = window_manager_data;

	window_manager->Destroy = &DestroyWindowManager;
	window_manager->GetEvent = &GetWindowEvent;
	window_manager->GetSize = &GetWindowManagerSize;
	window_manager->GetBaseSize = &GetWindowManagerBaseSize;
	window_manager->GetSound = &GetWindowSound;
	window_manager->GetTimer = &GetWindowTimer;
	window_manager->GetWindow = &GetRenderWindow;
	window_manager->IsFullscreen = &IsFullscreen;
	window_manager->PollEvent = &PollEvent;
	window_manager->ResetTimer = &ResetWindowTimer;
	window_manager->RestartClock = &RestartWindowClock;
	window_manager->SetSound = &SetWindowSound;
	window_manager->ToggleFullscreen = &ToggleFullscreen;
	window_manager->AddNewSound = &AddNewSound;
	window_manager->GetCustomParam = &GetCustomParam;
	window_manager->SetCustomParam = &SetCustomParam;
	window_manager->AddNewCustomParam = &AddNewCustomParam;
	window_manager->GetCustomView = &GetView;
	window_manager->SetCustomView = &SetView;
	window_manager->SetDefaultView = &SetDefaultView;
	window_manager->GetDefaultView = &GetDefaultView;
	window_manager->GetMousePos = &GetMousePos;
	window_manager->Clear = &Clear;
	window_manager->Display = &Display;

	window_manager->DrawSprite = &WindowManagerDrawSprite;
	window_manager->DrawText = &WindowManagerDrawText;
	window_manager->DrawShape = &WindowManagerDrawShape;
	window_manager->DrawCircleShape = &WindowManagerDrawCircleShape;
	window_manager->DrawConvexShape = &WindowManagerDrawConvexShape;
	window_manager->DrawRectangleShape = &WindowManagerDrawRectangleShape;
	window_manager->DrawVertexArray = &WindowManagerDrawVertexArray;
	window_manager->DrawVertexBuffer = &WindowManagerDrawVertexBuffer;
	window_manager->DrawPrimitives = &WindowManagerDrawPrimitives;
	window_manager->DrawAnimation = &WindowManagerDrawAnimation;
	window_manager->DrawParticles = &WindowManagerDrawParticles;

	ScreenScaleFactorX = (float)window_manager->_Data->m_size.x / (float)window_manager->_Data->m_base_size.x;
	ScreenScaleFactorY = (float)window_manager->_Data->m_size.y / (float)window_manager->_Data->m_base_size.y;
	sfSprite_setScale(window_manager->_Data->m_renderer, sfVector2f_Create(ScreenScaleFactorX, ScreenScaleFactorY));

	return window_manager;
}
