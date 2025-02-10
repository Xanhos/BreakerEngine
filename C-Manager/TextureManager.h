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
 * @file texturemanager.h
 * @brief This file contains all the data to load textures for the engine.
 */

/**
 * @struct Texture
 * @brief Represents a texture object, storing the texture data and its metadata.
 */
typedef struct Texture Texture;
struct Texture
{
    sfTexture* m_texture;          /**< Pointer to the SFML texture object. */
    Path m_path;                   /**< Path to the texture file. */
    char m_name[MAX_PATH_SIZE];    /**< Name of the texture used for identification. */
};

/**
 * @brief Creates a new texture from a file path.
 * @param path Path to the texture file.
 * @return A Texture object initialized with the given file.
 */
Texture CreateTexture(const char* path);

/**
 * @brief Deletes a texture object and releases its associated resources.
 * @param texture Pointer to the Texture object to delete.
 */
void DeleteTexture(Texture* texture);

/**
 * @brief Initializes the texture manager and loads necessary resources.
 * This function sets up resources and data structures for texture management.
 */
void InitTextureManager(void);

/**
 * @brief Loads all textures associated with a specific scene.
 * @param scene Name of the scene for which textures should be loaded.
 */
void LoadSceneTexture(const char* scene, float* progressValue);

/**
 * @brief Clears all textures associated with the current scene.
 * This function frees memory and unloads scene-specific textures.
 */
void ClearSceneTexture(void);

/**
 * @brief Retrieves a texture object by its name.
 * @param name Name of the texture to retrieve.
 * @return Pointer to the texture object if found, or NULL if not found.
 */
sfTexture* GetTexture(const char* name);

/**
 * @brief Destroys the texture manager and releases all associated resources.
 * This function is called when shutting down the application to free up memory and unload textures.
 */
void DestroyTexturesManager(void);
