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
 * @param loading_state Name of the loading state.
 * @param ResetLoadingStateFunc Function pointer to reset the loading state.
 */
void StartGame(WindowManager* window_manager, const char* starting_state, const char* loading_state, void(*ResetLoadingStateFunc)(WindowManager* window));

/**
 * @brief Ends the game and exits the StartGame function.
 * @param window Pointer to the WindowManager object.
 *
 * @warning This function should call the destroy function of the current state and destroy the window.
 */
void EndGame(WindowManager* window);

/**
 * @brief Changes the current state of the application.
 * @param state_name Name of the new state.
 */
void ChangeMainState(const char* state_name);

/**
 * @brief Registers a sub-state to the application.
 * @param state_name Name of the sub-state.
 * @param window Pointer to the WindowManager object.
 * @param update_of_below_state Boolean indicating if the state below should continue updating.
 * @param display_of_below_state Boolean indicating if the state below should continue rendering.
 */
void RegisterSubState(char* state_name, WindowManager* window, sfBool update_of_below_state, sfBool display_of_below_state);

/**
 * @brief Pushes a sub-state onto the state stack.
 * @param state_name Name of the sub-state to push.
 */
void PushSubState(char* state_name);

/**
 * @brief Pops the current sub-state from the stack, returning to the previous state.
 */
void PopSubState();

const char* GetCurrentState();