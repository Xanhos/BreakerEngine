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


StateInfo Current_state, New_state, Loading_state;

typedef struct
{
	StateInfo state;
	sfBool update_of_below_state;
	sfBool display_of_below_state;
} SubState;

stdList* registered_sub_state_list, * active_sub_state_list;
void(*ResetLoadingStateFunction)(WindowManager* window);


ThreadManager* thread_manager;
sfBool is_changing_state;
sfBool has_loaded_state;
sfBool is_sub_state_delete;
Clock* main_clock;

float loading_delay;


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


static sfBool UpdateSubState(WindowManager* window)
{
	if (active_sub_state_list->size(active_sub_state_list) > 0)
	{
		int size = active_sub_state_list->size(active_sub_state_list) - 1;
		while (size > -1)
		{
			SubState* state = STD_GETDATA(active_sub_state_list, SubState, size);
			state->state.Update(window);
			if (!state->update_of_below_state)
				return sfFalse;
			size--;
		}
		return sfTrue;
	}
	return sfTrue;
}


static sfBool RenderSubState(WindowManager* window)
{
	if (active_sub_state_list->size(active_sub_state_list) > 0)
	{
		sfView* customView = window->GetCustomView(window);
		window->SetDefaultView(window);

		FOR_EACH_LIST(active_sub_state_list, SubState, i, it,
			it->state.Render(window);
		it->state.UIRender(window);
		if (!it->display_of_below_state)
			return sfFalse;
			)

			if (customView)
				window->SetCustomView(window, customView);
		return sfTrue;
	}
	return sfTrue;
}

static sfBool ShouldRenderMainState(WindowManager* window)
{
	if (active_sub_state_list->size(active_sub_state_list) > 0)
	{
		int size = active_sub_state_list->size(active_sub_state_list) - 1;
		while (size > -1)
		{
			SubState* state = STD_GETDATA(active_sub_state_list, SubState, size);
			if (!state->display_of_below_state)
				return sfFalse;
			size--;
		}
		return sfTrue;
	}
	return sfTrue;
}


static void Update(WindowManager* window)
{
	UpdateKeyAndMouseState();
	CleanUpTempMemory();
	main_clock->restartClock(main_clock);
	window->RestartClock(window);
	DeltaTime = main_clock->getDeltaTime(main_clock);
	if (is_changing_state)
	{
		FOR_EACH_LIST(registered_sub_state_list, SubState, i, it,
			it->state.Destroy(window);
			);
		active_sub_state_list->clear(active_sub_state_list);
		registered_sub_state_list->clear(registered_sub_state_list);

		if (Current_state.Destroy)
			Current_state.Destroy(window);

		if (New_state.Init)
		{

			InitThreadInfo init_thread_info;
			init_thread_info.state_info = New_state;
			init_thread_info.window_manager = window;

			thread_manager->AddNewThread(thread_manager, &init_new_state, &init_thread_info, sfTrue, sizeof(init_thread_info));

		}
		Current_state = New_state;
		loading_delay = 1.f;
		is_changing_state = sfFalse;
		has_loaded_state = sfFalse;
	}

	if (!has_loaded_state)
	{
		thread_manager->Update(thread_manager);
		if (!thread_manager->GetThreadCount(thread_manager))
		{
 			loading_delay -= DeltaTime;
			if (loading_delay < 0.f)
			{
				has_loaded_state = sfTrue;
			}
		}
		if (Loading_state.Update && strcmp(Loading_state.name, "null") != 0)
		{
			Loading_state.Update(window);
			window->Clear(window, sfBlack);
			sfView* customView = window->GetCustomView(window);
			window->SetDefaultView(window);
			if (Loading_state.Render)
				Loading_state.Render(window);
			if (Loading_state.Render)
				Loading_state.UIRender(window);
			if (customView)
				window->SetCustomView(window, customView);
			window->Display(window);
		}
	}
	else
	{
		if (ResetLoadingStateFunction)
			ResetLoadingStateFunction(window);
		while (sfRenderWindow_pollEvent(window->GetWindow(window), window->GetEvent(window)))
		{
			sfEvent* event = window->GetEvent(window);
			if (event->type == sfEvtClosed)
			{
				EndGame(window);
			}
			if (Current_state.UpdateEvent)
				Current_state.UpdateEvent(window, event);
		}

		sfBool update_main_state = UpdateSubState(window);
		if (Current_state.Update && update_main_state)
			Current_state.Update(window);

		window->Clear(window, sfBlack);

		if (Current_state.Render && ShouldRenderMainState(window))
		{
			Current_state.Render(window);
			if (Current_state.UIRender)
			{
				sfView* customView = window->GetCustomView(window);
				window->SetDefaultView(window);
				Current_state.UIRender(window);
				if (customView)
					window->SetCustomView(window, customView);
			}
		}
		RenderSubState(window);
		window->Display(window);
	}
}



void ChangeMainState(const char* state_name)
{
	New_state = GetState(state_name);
	if (strcmp(New_state.name, "null") == 0)
	{
		printf_d("ERROR, UNKNOW STATE !!!!\n");
		return;
	}
	is_changing_state = sfTrue;
}

void RegisterSubState(char* state_name, WindowManager* window, sfBool update_of_below_state, sfBool display_of_below_state)
{
	StateInfo state = GetState(state_name);

	if (strcmp(state.name, "null") == 0)
	{
		printf_d("ERROR, UNKNOW STATE !!!!\n");
		return;
	}

	SubState sub_state = {
		.state = state,
		.update_of_below_state = update_of_below_state,
		.display_of_below_state = display_of_below_state
	};
	registered_sub_state_list->push_back(registered_sub_state_list, &sub_state);
	sub_state.state.Init(window);
}

void PushSubState(char* state_name)
{
	for (int i = 0; i < registered_sub_state_list->size(registered_sub_state_list); i++)
	{
		SubState* it = ((SubState*)registered_sub_state_list->getData(registered_sub_state_list, i));
		if (strcmp(it->state.name, state_name) == 0)
		{
			active_sub_state_list->push_back(active_sub_state_list, it);
			return;
		}
	};
	printf_d("ERROR, UNKNOW SUB STATE !!!!\n");
	return;
}

void PopSubState()
{
	active_sub_state_list->erase(active_sub_state_list, active_sub_state_list->size(active_sub_state_list) - 1);
}

const char* GetCurrentState()
{
	return Current_state.name;
}

static void CleanUpGame()
{
	FOR_EACH(registered_sub_state_list, StateInfo, i, it,
		it->Destroy(GameWindow);
		);
	Current_state.Destroy(GameWindow);
	main_clock->destroy(&main_clock);
	thread_manager->Destroy(&thread_manager);
	GameWindow->Destroy(&GameWindow);
}


void StartGame(WindowManager* window_manager, const char* starting_state, const char* loading_state, void(*ResetLoadingStateFunc)(WindowManager* window))
{
	srand((unsigned int)time(NULL));
	GameWindow = window_manager;
	Loading_state = GetState(loading_state);
	ResetLoadingStateFunction = ResetLoadingStateFunc;
	loading_delay = 0.f;
	if (Loading_state.Init)
		Loading_state.Init(window_manager);
	thread_manager = CreateThreadManager(2);
	main_clock = CreateClock();
	registered_sub_state_list = STD_LIST_CREATE(SubState, 0);
	active_sub_state_list = STD_LIST_CREATE(SubState, 0);
	ChangeMainState(starting_state);
	while (sfRenderWindow_isOpen(window_manager->GetWindow(window_manager)))
	{
		Update(window_manager);
	}
	CleanUpGame();
	ReportLeaks();
}

void EndGame(WindowManager* window)
{
	sfRenderWindow_close(window->GetWindow(window));
}