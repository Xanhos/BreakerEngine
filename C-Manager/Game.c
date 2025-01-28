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
#include "Game.h"

#include "MemoryManagement.h"
#include "Animation.h"
#include "time.h"

DECLARE_BLANK_STATE(Game)
REGISTER_C_STATE(Game)


StateInfo actual_state, new_state;

Animation* loading_screen;

ThreadManager* thread_manager;
sfBool is_changing_state;
Clock* main_clock;


typedef struct {
	StateInfo state_info;
	WindowManager* window_manager;
} InitThreadInfo;


static void init_new_state(void* state_info)
{
	InitThreadInfo* new_state_info = state_info;

	if (new_state_info->state_info.Init)
		new_state_info->state_info.Init(new_state_info->window_manager);
}


static void Update(WindowManager* window)
{
	main_clock->restartClock(main_clock);
	DeltaTime = main_clock->getDeltaTime(main_clock);
	if (is_changing_state)
	{
		if (actual_state.Destroy)
			actual_state.Destroy(window);

		if (new_state.Init)
		{

			InitThreadInfo init_thread_info;
			init_thread_info.state_info = new_state;
			init_thread_info.window_manager = window;

			thread_manager->AddNewThread(thread_manager, &init_new_state, &init_thread_info, sfTrue, sizeof(init_thread_info));

		}
		actual_state = new_state;
		is_changing_state = sfFalse;
	}

	if (thread_manager->GetThreadCount(thread_manager))
	{
		thread_manager->Update(thread_manager);
		if (loading_screen)
		{
			loading_screen->Update(loading_screen, DeltaTime);
			sfRenderWindow* rdwindow = window->GetWindow(window);
			sfRenderWindow_clear(rdwindow, sfBlack);
			sfView* customView = window->GetCustomView(window);
			sfRenderWindow_setView(rdwindow, sfRenderWindow_getDefaultView(rdwindow));
			sfRenderWindow_drawAnimation(rdwindow, loading_screen, NULL);
			if (customView)
				window->SetCustomView(window, customView);
			sfRenderWindow_display(rdwindow);
		}
	}
	else
	{
		window->RestartClock(window);
		while (sfRenderWindow_pollEvent(window->GetWindow(window), window->GetEvent(window)))
		{
			sfEvent* event = window->GetEvent(window);
			if (event->type == sfEvtClosed)
			{
				sfRenderWindow_close(window->GetWindow(window));
			}
			if (actual_state.UpdateEvent)
				actual_state.UpdateEvent(window, event);
		}
		if (actual_state.Update)
			actual_state.Update(window);
		if (actual_state.Render)
		{
			sfRenderWindow* rdwindow = window->GetWindow(window);
			sfRenderWindow_clear(rdwindow, sfBlack);
			actual_state.Render(window);
			if (actual_state.UIRender)
			{
				sfView* customView = window->GetCustomView(window);
				sfRenderWindow_setView(rdwindow, sfRenderWindow_getDefaultView(rdwindow));
				actual_state.UIRender(window);
				if (customView)
					window->SetCustomView(window, customView);
			}
			sfRenderWindow_display(rdwindow);
		}
	}
}



void ChangeState(char* state_name)
{
	new_state = GetState(state_name);
	if (strcmp(new_state.name, "null") == 0)
	{
		printf_d("ERROR, UNKNOW STATE !!!!\n");
	}
	is_changing_state = sfTrue;
}

void StartGame(WindowManager* window_manager, char* starting_state, Animation* loading_screen_animation, const char* key_anim_name)
{
	ChangeState(starting_state);
	srand(time(NULL));
	loading_screen = loading_screen_animation;
	if (loading_screen)
		loading_screen->SelectAnimationKey(loading_screen, key_anim_name);
	thread_manager = CreateThreadManager(2);
	main_clock = CreateClock();
	while (sfRenderWindow_isOpen(window_manager->GetWindow(window_manager)))
	{
		Update(window_manager);
	}
	ReportLeaks();
}
