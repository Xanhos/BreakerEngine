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
 * @file fontmanager.h
 * @brief This file contains all the data to load fonts for the engine.
 */


#undef CreateFont /**< Undefine the macro CreateFont to avoid conflicts. */

/**
 * @struct Font
 * @brief Represents a font object with its associated metadata.
 */
typedef struct Font Font;
struct Font
{
    sfFont* m_font;              /**< Pointer to the loaded font object. */
    Path m_path;                 /**< Path to the font file. */
    char m_name[MAX_PATH_SIZE];  /**< Name of the font, used for identification. */
};

/**
 * @brief Creates a new font object from a file path.
 * @param path Path to the font file.
 * @return A Font object initialized with the given file.
 */
Font CreateFont(const char* path);

/**
 * @brief Deletes a font object and releases its associated resources.
 * @param font Pointer to the Font object to delete.
 */
void DeleteFont(Font* font);

/**
 * @brief Initializes the font manager.
 * This function sets up resources and data structures for font management.
 */
void InitFontManager(void);

/**
 * @brief Loads fonts associated with a specific scene.
 * @param scene Name of the scene for which fonts should be loaded.
 */
void LoadSceneFont(const char* scene, float* progressValue);

/**
 * @brief Clears all fonts associated with the current scene.
 * This function frees memory and unloads scene-specific fonts.
 */
void ClearSceneFont(void);

/**
 * @brief Retrieves a font object by its name.
 * @param name Name of the font to retrieve.
 * @return Pointer to the font object if found, or NULL if not found.
 */
sfFont* GetFont(const char* name);

/**
 * @brief Destroys the font manager and releases all associated resources.
 */
void DestroyFontsManager(void);
