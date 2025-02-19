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
#include "TextureManager.h"
#include "AudioManager.h"
#include "FontManager.h"
#include "MovieManager.h"
#include "SpriteManager.h"

/**
 * @file resourcesmanager.h
 * @brief This file contains all the data to manage all the resources used by the engine.
 */

 /**
  * @brief Initializes the resources manager and loads resources from the specified directory.
  * @param resource_directory_ Path to the directory containing resources to load.
  */
void InitResourcesManager(const char* resource_directory_);

/**
 * @brief Loads all the resources for a specific scene.
 * This function typically loads textures, sounds, fonts, movies, and other resources required for the scene.
 * @param scene_name Name of the scene whose resources should be loaded.
 */
void LoadScene(const char* scene_name);

/**
 * @brief Destroys the resources manager and releases all allocated resources.
 * This function is called when shutting down the application to free up memory and unload resources.
 */
void DestroyResourcesManager(void);

/**
 * @brief Retrieves the current loading progress of the resources manager.
 * @return A float value representing the percentage of resources loaded (0.0 to 1.0).
 */
float GetLoadingValue();
