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
#include "ResourcesManager.h"
#include "WindowManager.h"

/**
 * @file animation.h
 * @brief This file defines the Animation system structure and functions for managing an animation.
 */

 /**
  * @typedef Animation_Key
  * @brief Represents an animation key structure.
  */
typedef struct Animation_Key Animation_Key;

/**
 * @typedef Animation_Key_Data
 * @brief Holds internal data for an animation key.
 */
typedef struct Animation_Key_Data Animation_Key_Data;

/**
 * @struct Animation_Key
 * @brief Encapsulates data and functions for managing an animation key.
 */
struct Animation_Key
{
	Animation_Key_Data* _Data; /**< Pointer to internal data for the animation key. */

	/**
	 * @brief Retrieves the total number of frames.
	 * @param anim_key Pointer to the animation key.
	 * @return Total number of frames.
	 */
	int (*GetTotalFrame)(Animation_Key* anim_key);

	/**
	 * @brief Retrieves the current frame index.
	 * @param anim_key Pointer to the animation key.
	 * @return Current frame index.
	 */
	int (*GetCurrentFrame)(Animation_Key* anim_key);

	/**
	 * @brief Retrieves the texture rectangle for the current frame.
	 * @param anim_key Pointer to the animation key.
	 * @return Texture rectangle for the current frame.
	 */
	sfIntRect(*GetCurrentRect)(Animation_Key* anim_key);

	/**
	 * @brief Sets the current frame index.
	 * @param anim_key Pointer to the animation key.
	 * @param frame Frame index to set.
	 */
	void (*SetCurrentFrame)(Animation_Key* anim_key, int frame);

	/**
	 * @brief Retrieves the name of the animation key.
	 * @param anim_key Pointer to the animation key.
	 * @return Name of the animation key.
	 */
	const char* (*GetAnimationKeyName)(Animation_Key* anim_key);

	/**
	 * @brief Checks if the animation has finished playing.
	 * @param anim_key Pointer to the animation key.
	 * @return sfTrue if the animation has finished, otherwise sfFalse.
	 */
	sfBool(*HasFinishAnim)(Animation_Key* anim_key);
};

/**
 * @typedef Animation
 * @brief Represents an animation structure.
 */
typedef struct Animation Animation;

/**
 * @typedef Animation_Data
 * @brief Holds internal data for an animation.
 */
typedef struct Animation_Data Animation_Data;

/**
 * @struct Animation
 * @brief Encapsulates data and functions for managing animations.
 */
struct Animation
{
	Animation_Data* _Data; /**< Pointer to internal data for the animation. */

	/**
	 * @brief Adds a new animation key to the animation.
	 * @param anim Pointer to the animation.
	 * @param anim_key Pointer to the animation key to add.
	 * @return Pointer to the added animation key.
	 */
	Animation_Key* (*AddAnimationKey)(Animation const* anim, Animation_Key* anim_key);

	/**
	 * @brief Selects an animation key by its name.
	 * @param anim Pointer to the animation.
	 * @param name Name of the animation key to select.
	 * @return Pointer to the selected animation key.
	 */
	Animation_Key* (*SelectAnimationKey)(Animation* anim, const char* name);

	/**
	 * @brief Sets various animation parameters.
	 * @param anim Pointer to the animation.
	 * @param is_paused Whether the animation is paused.
	 * @param is_revert Whether the animation plays in reverse.
	 * @param is_stopped_at_last_frame Whether the animation stops at the last frame.
	 */
	void (*SetAnimationParameters)(Animation* anim, sfBool is_paused, sfBool is_revert, sfBool is_stopped_at_last_frame);

	/**
	 * @brief Checks if the animation is reversed.
	 * @param anim Pointer to the animation.
	 * @return sfTrue if reversed, otherwise sfFalse.
	 */
	sfBool(*IsRevert)(Animation* anim);

	/**
	 * @brief Checks if the animation is paused.
	 * @param anim Pointer to the animation.
	 * @return sfTrue if paused, otherwise sfFalse.
	 */
	sfBool(*IsPaused)(Animation* anim);

	/**
	 * @brief Checks if the animation stops at the last frame.
	 * @param anim Pointer to the animation.
	 * @return sfTrue if it stops at the last frame, otherwise sfFalse.
	 */
	sfBool(*IsStoppedAtLastFrame)(Animation* anim);

	/**
	 * @brief Retrieves the current animation key.
	 * @param anim Pointer to the animation.
	 * @return Pointer to the current animation key.
	 */
	Animation_Key* (*GetCurrentAnimationKey)(Animation* anim);

	/**
	 * @brief Retrieves the renderer for the animation.
	 * @param anim Pointer to the animation.
	 * @return Pointer to the rectangle shape used for rendering.
	 */
	sfRectangleShape* (*GetRenderer)(Animation* anim);

	/**
	 * @brief Updates the animation based on elapsed time.
	 * @param anim Pointer to the animation.
	 * @param deltaTime Time elapsed since the last update.
	 */
	void (*Update)(Animation* anim, float deltaTime);

	/**
	 * @brief Destroys the animation and releases associated resources.
	 * @param anim Pointer to the animation pointer to destroy.
	 */
	void (*Destroy)(Animation** anim);
};

/**
 * @brief Creates a new animation key.
 * @param name Name of the animation key.
 * @param rect Texture rectangle for the animation key.
 * @param line_number Line number in the texture sheet.
 * @param line_frame_number Number of frames in the line.
 * @param total_frame Total number of frames in the animation.
 * @param frame_time Duration of each frame.
 * @return Pointer to the created animation key.
 */
Animation_Key* CreateAnimationKey(const char* name, sfIntRect rect, int number_of_line, int frame_per_line, int total_frame, float frame_time);

/**
 * @brief Creates a new animation.
 * @param name Name of the animation.
 * @param texture Pointer to the texture used for the animation.
 * @return Pointer to the created animation.
 */
Animation* CreateAnimation(const char* name, sfTexture* texture);

/**
 * @brief Creates a new animation from a .anim file.
 * @param path File path for the animation data.
 * @param texture Pointer to the texture used for the animation.
 * @return Pointer to the created animation.
 */
Animation* CreateAnimationFromFile(const char* path, sfTexture* texture);

/**
 * @brief Renders an animation in a render window.
 * @param window Pointer to the render window.
 * @param anim Pointer to the animation to render.
 * @param states Render states to apply.
 */
void sfRenderWindow_drawAnimation(WindowManager* window, Animation* anim, sfRenderStates* states);

/**
 * @typedef SimpleAnim
 * @brief Represents a simplified animation structure.
 */
typedef struct SimpleAnim SimpleAnim;

/**
 * @typedef SimpleAnim_Data
 * @brief Holds internal data for a simple animation.
 */
typedef struct SimpleAnim_Data SimpleAnim_Data;

/**
 * @struct SimpleAnim
 * @brief Encapsulates data and functions for a simple animation.
 */
struct SimpleAnim
{
	SimpleAnim_Data* _Data; /**< Pointer to internal data for the simple animation. */

	/**
	 * @brief Retrieves the sprite used for rendering.
	 * @param anim Pointer to the simple animation.
	 * @return Pointer to the sprite.
	 */
	sfSprite* (*GetRenderer)(SimpleAnim* anim);

	/**
	 * @brief Updates the simple animation.
	 * @param anim Pointer to the simple animation.
	 * @param deltaTime Time elapsed since the last update.
	 */
	void (*Update)(SimpleAnim* anim, float deltaTime);

	/**
	 * @brief Draws the simple animation.
	 * @param anim Pointer to the simple animation.
	 * @param window Pointer to the render window.
	 * @param state Render states to apply.
	 */
	void (*Draw)(SimpleAnim* anim, sfRenderWindow* window, sfRenderStates* state);

	/**
	 * @brief Destroys the simple animation and releases resources.
	 * @param anim Pointer to the simple animation pointer to destroy.
	 */
	void (*Destroy)(SimpleAnim** anim);
};

/**
 * @brief Creates a new simple animation.
 */
SimpleAnim* CreateSimpleAnim(sfTexture* texture, sfIntRect rect, int line_number, int line_frame_number, int total_frame, float frame_time);
