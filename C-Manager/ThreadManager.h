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
#include "List.h"
#include "SFML/System.h"

/**
 * @file threadmanager.h
 * @brief This file defines the ThreadManager structure and functions for managing a multitude of thread.
 *
 * This code shows how to create a ThreadManager that can be used for various purposes, such as resources loading or data treatment.
 *
 * @code
 * // Example usage of creating a new viewport:
 * ThreadManager* viewport = CreateThreadManager(5);
 * @endcode
 *
 * The above code creates a ThreadManager that can handle up to 5 threads simultaneous.
 */

/**
 * @typedef ThreadManager_Data
 * @brief Opaque structure that holds the internal data for the thread manager.
 */
typedef struct ThreadManager_Data ThreadManager_Data;

/**
 * @typedef ThreadManager
 * @brief Manages a collection of threads and provides functionality for creating, updating, and destroying threads.
 */
typedef struct ThreadManager ThreadManager;

/**
 * @struct ThreadManager
 * @brief Contains function pointers to manage threads, including adding, updating, and destroying threads.
 */
struct ThreadManager
{
    ThreadManager_Data* _Data; /**< Internal data for managing threads. */

    /**
     * @brief Adds a new thread to the manager.
     * @param thread_manager Pointer to the ThreadManager object.
     * @param func The function to be executed by the new thread.
     * @param func_data Data to pass to the function, if any.
     * @param copy_data Flag to indicate whether to copy the data or not.
     * @param data_size Size of the data to pass to the function.
     * 
     * @warning if the thread manager is full, this function will block the main thread until the new task can be added. Update of the thread manager is automatically make in that case.
     */
    void (*AddNewThread)(ThreadManager* thread_manager, void (*func)(void*), void* func_data, sfBool copy_data, size_t data_size);

    /**
     * @brief Gets the current number of threads managed by the ThreadManager.
     * @param thread_manager Pointer to the ThreadManager object.
     * @return The number of threads managed by the manager.
     */
    size_t(*GetThreadCount)(ThreadManager* thread_manager);

    /**
     * @brief Updates the state of all threads managed by the ThreadManager.
     * @param thread_manager Pointer to the ThreadManager object.
     */
    void (*Update)(ThreadManager* thread_manager);

    /**
     * @brief Destroys the ThreadManager and releases all associated resources.
     * @param thread_manager Pointer to the pointer of the ThreadManager object to destroy.
     */
    void (*Destroy)(ThreadManager** thread_manager);
};

/**
 * @brief Creates a new ThreadManager with a specified thread limit.
 * @param limit The maximum number of threads that can be handled by the manager.
 * @return Pointer to the newly created ThreadManager object.
 */
ThreadManager* CreateThreadManager(size_t limit);
