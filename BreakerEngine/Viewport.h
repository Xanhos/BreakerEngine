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
#include "Tools.h"

/**
 * @file viewport.h
 * @brief This file defines the Viewport structure and functions for managing a viewport within a window.
 *
 * This code shows how to create a viewport that can be used for various purposes, such as a mini-map or split screen.
 *
 * @code
 * // Example usage of creating a new viewport:
 * WindowManager* window_manager = CreateWindowManager(1080, 720, "SFML_Works", sfDefaultStyle, NULL);
 * Viewport* viewport = CreateViewport(window_manager->GetWindowSize(window_manager), sfVector2f_Create(100, 100), (sfFloatRect) { 0, 0, 100, 100 });
 * @endcode
 *
 * The above code creates a viewport of size 100x100 at the top left corner of the window, with the viewport itself having the same dimensions. This can be useful for creating a mini-map or implementing split-screen functionality.
 */

/**
 * @typedef Viewport
 * @brief Structure for managing a viewport in a window.
 */

typedef struct Viewport Viewport;

/**
 * @typedef Viewport_Data
 * @brief Internal structure for holding data related to the viewport.
 */
typedef struct Viewport_Data Viewport_Data;

/**
 * @struct Viewport
 * @brief A structure used to manage a viewport within a window.
 */
struct Viewport {
    Viewport_Data* _Data; /**< Internal data related to the viewport. */

    /**
     * @brief Updates the viewport with a new area (sfFloatRect).
     * @param viewport The viewport instance.
     * @param port The new sfFloatRect representing the viewport area.
     */
    void (*UpdatePort)(Viewport* viewport, sfFloatRect port);

    /**
     * @brief Updates the viewport when the window size changes.
     * @param viewport The viewport instance.
     * @param size The new window size.
     */
    void (*UpdateWindowSize)(Viewport* viewport, sfVector2u size);

    /**
     * @brief Retrieves the current view of the viewport.
     * @param viewport The viewport instance.
     * @return The current sfView of the viewport.
     */
    sfView* (*GetView)(Viewport* viewport);

    /**
     * @brief Zooms in the viewport by the specified factor.
     * @param viewport The viewport instance.
     * @param factor The zoom factor, greater than 1 for zooming in.
     */
    void (*Zoom)(Viewport* viewport, float factor);

    /**
     * @brief Dezooms the viewport by the specified factor.
     * @param viewport The viewport instance.
     * @param factor The dezoom factor, greater than 1 for zooming out.
     */
    void (*Dezoom)(Viewport* viewport, float factor);
};

/**
 * @brief Creates a new viewport with the given size and area.
 *
 * This function creates a new viewport with a specific size and area (sfFloatRect)
 * to manage a portion of the window or to create effects like mini-maps or split-screens.
 *
 * @param windowSize The size of the window (width and height).
 * @param size The size of the viewport (width and height).
 * @param port The area (sfFloatRect) representing the viewport.
 * @return A pointer to the newly created Viewport instance.
 *
 * @code
 * // Example of creating a new viewport
 * sfVector2u window_size = { 1080, 720 };
 * sfVector2f viewport_size = { 100, 100 };
 * sfFloatRect viewport_area = { 0, 0, 100, 100 };
 * Viewport* new_viewport = CreateViewport(window_size, viewport_size, viewport_area);
 * @endcode
 */
Viewport* CreateViewport(sfVector2u windowSize, sfVector2f size, sfFloatRect port);
