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
#include "KSound.h"

/**
 * @file audiomanager.h
 * @brief This file contains all the data to load sounds and musics for the engine.
 */


/**
 * @struct Sound
 * @brief Represents a sound object that includes a sound buffer, a sound instance, and associated metadata.
 */
typedef struct Sound Sound;
struct Sound
{
    KSound* m_sound;             /**< Pointer to the sound instance for playback. */
    Path m_path;                  /**< Path to the sound file. */
    char m_name[MAX_PATH_SIZE];   /**< Name of the sound, typically used for identification. */
};

/**
 * @struct Music
 * @brief Represents a music object that includes a music stream and associated metadata.
 */
typedef struct Music Music;
struct Music
{
    KSound* m_music;            /**< Pointer to the music instance for playback. */
    Path m_path;                 /**< Path to the music file. */
    char m_name[MAX_PATH_SIZE];  /**< Name of the music, typically used for identification. */
};

/**
 * @brief Creates a new sound object from a file path.
 * @param path Path to the sound file.
 * @return A Sound object initialized with the given file.
 */
Sound CreateSound(const char* path);

/**
 * @brief Creates a new music object from a file path.
 * @param path Path to the music file.
 * @return A Music object initialized with the given file.
 */
Music CreateMusic(const char* path);

/**
 * @brief Deletes a sound object and releases its associated resources.
 * @param sound Pointer to the Sound object to delete.
 */
void DeleteSound(Sound* sound);

/**
 * @brief Deletes a music object and releases its associated resources.
 * @param music Pointer to the Music object to delete.
 */
void DeleteMusic(Music* music);

/**
 * @brief Initializes the sound manager.
 * This function sets up the necessary resources and data structures for sound management.
 */
void InitSoundManager(void);

/**
 * @brief Loads sounds associated with a specific scene.
 * @param scene Name of the scene for which sounds should be loaded.
 */
void LoadSceneSound(const char* scene, float* soundProgressValue, float* musicProgressValue);

/**
 * @brief Clears all sounds associated with the current scene.
 * This function frees memory and unloads scene-specific sounds.
 */
void ClearSceneSound(void);

/**
 * @brief Retrieves a sound instance by its name.
 * @param name Name of the sound to retrieve.
 * @return Pointer to the sound instance if found, or NULL if not found.
 */
KSound* GetSound(const char* name);

/**
 * @brief Retrieves a music instance by its name.
 * @param name Name of the music to retrieve.
 * @return Pointer to the music instance if found, or NULL if not found.
 */
KSound* GetMusic(const char* name);

/**
 * @brief Destroys the sound manager and releases all associated resources.
 */
void DestroySoundsManager(void);
