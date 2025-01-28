﻿/*
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
 * @file spritemanager.h
 * @brief This file defines the SpriteManager structure and functions for managing a multitude of sprite.
 */

/**
 * @typedef SpriteManager_Data
 * @brief Opaque structure that holds the internal data of the sprite manager.
 */
typedef struct SpriteManager_Data SpriteManager_Data;

/**
 * @typedef SpriteManager
 * @brief Manages a collection of sprites and provides functions to manipulate and render them.
 */
typedef struct SpriteManager SpriteManager;

/**
 * @struct SpriteManager
 * @brief Contains function pointers to manage sprites in a sprite collection.
 */
struct SpriteManager
{
    SpriteManager_Data* _Data;  /**< Internal data for managing sprites. */

    /**
     * @brief Adds a new sprite to the manager.
     * @param sprite_manager Pointer to the SpriteManager object.
     * @param name The name to assign to the new sprite.
     * @param texture The texture to use for the sprite.
     * @param reset_rect Flag to reset the sprite's rectangle bounds.
     * @return Pointer to the newly added sprite.
     */
    sfSprite* (*push_back)(const SpriteManager* sprite_manager, const char* name, const sfTexture* texture, sfBool reset_rect);

    /**
     * @brief Draws all sprites managed by the sprite manager to the specified window.
     * @param sprite_manager Pointer to the SpriteManager object.
     * @param window Pointer to the SFML render window to draw the sprites to.
     */
    void (*draw)(const SpriteManager* sprite_manager, sfRenderWindow* window);

    /**
     * @brief Destroys the sprite manager and frees all associated resources.
     * @param sprite_manager Pointer to the pointer of the SpriteManager object to destroy.
     */
    void (*destroy)(SpriteManager** sprite_manager);

    /**
     * @brief Returns the number of sprites managed by the sprite manager.
     * @param sprite_manager Pointer to the SpriteManager object.
     * @return The number of sprites in the manager.
     */
    size_t(*size)(SpriteManager* sprite_manager);

    /**
     * @brief Retrieves a sprite by its name.
     * @param sprite_manager Pointer to the SpriteManager object.
     * @param name The name of the sprite to retrieve.
     * @return Pointer to the sprite if found, or NULL if not found.
     */
    sfSprite* (*get_by_name)(const SpriteManager* sprite_manager, const char* name);

    /**
     * @brief Retrieves a sprite by its index.
     * @param sprite_manager Pointer to the SpriteManager object.
     * @param index The index of the sprite to retrieve.
     * @return Pointer to the sprite if found, or NULL if not found.
     */
    sfSprite* (*get_by_id)(const SpriteManager* sprite_manager, int index);

    /**
     * @brief Removes a sprite by its name.
     * @param sprite_manager Pointer to the SpriteManager object.
     * @param name The name of the sprite to remove.
     */
    void (*remove_by_name)(const SpriteManager* sprite_manager, const char* name);

    /**
     * @brief Removes a sprite by its index.
     * @param sprite_manager Pointer to the SpriteManager object.
     * @param index The index of the sprite to remove.
     */
    void (*remove_by_id)(const SpriteManager* sprite_manager, int index);
};

/**
 * @brief Creates a new SpriteManager object.
 * @return Pointer to the newly created SpriteManager object.
 */
SpriteManager* CreateSpriteManager(void);

/**
 * @brief Gets the top-left position of a sprite.
 * @param sprite Pointer to the sprite.
 * @return The top-left position of the sprite as a sfVector2f.
 */
sfVector2f sfSprite_GetTopLeftPos(const sfSprite* sprite);

/**
 * @brief Gets the center position of a sprite.
 * @param sprite Pointer to the sprite.
 * @return The center position of the sprite as a sfVector2f.
 */
sfVector2f sfSprite_GetCenterPos(const sfSprite* sprite);
