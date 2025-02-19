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
 * @file moviemanager.h
 * @brief This file contains all the data to load movies for the engine.
 */


/**
 * @struct Movie
 * @brief Represents a movie object with its associated metadata.
 */
typedef struct Movie Movie;
struct Movie
{
    sfeMovie* m_movie;              /**< Pointer to the movie object for playback. */
    Path m_path;                   /**< Path to the movie file. */
    char m_name[MAX_PATH_SIZE];    /**< Name of the movie, used for identification. */
};

/**
 * @brief Creates a new movie object from a file path.
 * @param path Path to the movie file.
 * @return A Movie object initialized with the given file.
 */
Movie CreateMovie(const char* path);

/**
 * @brief Deletes a movie object and releases its associated resources.
 * @param movie Pointer to the Movie object to delete.
 */
void DeleteMovie(Movie* movie);

/**
 * @brief Initializes the movie manager.
 * This function sets up resources and data structures for movie management.
 */
void InitMovieManager(void);

/**
 * @brief Loads movies associated with a specific scene.
 * @param scene Name of the scene for which movies should be loaded.
 */
void LoadSceneMovie(const char* scene, float* progressValue);

/**
 * @brief Clears all movies associated with the current scene.
 * This function frees memory and unloads scene-specific movies.
 */
void ClearSceneMovie(void);

/**
 * @brief Retrieves a movie object by its name.
 * @param name Name of the movie to retrieve.
 * @return Pointer to the movie object if found, or NULL if not found.
 */
sfeMovie* GetMovie(const char* name);

/**
 * @brief Destroys the movie manager and releases all associated resources.
 */
void DestroyMoviesManager(void);
