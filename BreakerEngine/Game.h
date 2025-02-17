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
#pragma once
#include "ResourcesManager.h"
#include "WindowManager.h"
#include "ThreadManager.h"
#include "Animation.h"
#include "State.h"


/**
 * @file game.h
 * @brief This file contains the core of the game engine.
*/



/**
 * @brief Starts the game with a specified loading screen animation.
 * @param window_manager Pointer to the WindowManager object for managing the application window.
 * @param starting_state Name of the starting state.
 * @param loading_screen_animation Pointer to the Animation object for the loading screen.
 * @param key_anim_name Name of the animation key to use for the loading screen.
 */
void StartGame(WindowManager* window_manager,const char* starting_state, const char* loading_state, void(*ResetLoadingStateFunc)(WindowManager* window));

/**
 * @brief End the game and exit the StartGame function.
 * @param WindowManager* window Window where the game is going.
 *
 * @warning pass in the destroy function of the current state and destroy the window
 */
void EndGame(WindowManager* window);


/**
 * @brief Changes the current state of the application.
 * @param char state_name Name of the new state.
 */
void ChangeMainState(char* state_name);


void RegisterSubState(char* state_name, WindowManager* window, sfBool update_of_below_state,sfBool display_of_below_state);

void PushSubState(char* state_name);

void PopSubState();

