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
#include "string.h"

#undef DrawText;

/**
 * @file windowmanager.h
 * @brief This file defines the WindowManager structure and functions for managing a window and all the settings.
 *
 * This code shows how to create a window.
 *
 * @code
 * // Example usage of creating a new window:
 * WindowManager* window_manager = CreateWindowManager(1080, 720, "SFML_Works", sfDefaultStyle, NULL);
 * @endcode
 *
 * The above code creates a window of size 1080x720 with a title of "SFML_Works" and the default window style.
 */


 /**
  * @typedef WindowManager
  * @brief Structure for managing the window and related operations.
  */
typedef struct WindowManager WindowManager;

/**
 * @typedef WindowManager_Data
 * @brief Internal structure for holding data related to the window manager.
 */
typedef struct WindowManager_Data WindowManager_Data;

/**
 * @struct WindowManager
 * @brief A structure that manages window operations and custom parameters.
 */
struct WindowManager
{
	WindowManager_Data* _Data; /**< Internal data for window manager. */

	/**
	 * @brief Checks if the window is currently in fullscreen mode.
	 * @param window The WindowManager instance.
	 * @return True if the window is fullscreen, false otherwise.
	 */
	sfBool(*IsFullscreen)(const WindowManager* window);

	/**
	 * @brief Toggles between fullscreen and windowed mode.
	 * @param window The WindowManager instance.
	 */
	void (*ToggleFullscreen)(const WindowManager* window);

	/**
	 * @brief Adds a new custom parameter to the window manager.
	 * @param window_manager The WindowManager instance.
	 * @param name The name of the custom parameter.
	 * @param param_func The function that handles the parameter.
	 * @param param_data The data for the custom parameter.
	 * @param param_size The size of the parameter data.
	 *
	 * @code
	 * //Example of usage by adding a new custom parameter:
	 * void setFPSMax(const WindowManager* window, void* fps_max)
	 * {
	 *     sfRenderWindow_setFramerateLimit(window->GetWindow(window), *(unsigned int*)fps_max);
	 * }
	 *
	 * int main(void)
	 * {
	 *     WindowManager* window_manager = CreateWindowManager(1080, 720, "SFML_Works", sfDefaultStyle, NULL);
	 *     unsigned int fps = 400;
	 *     window_manager->AddNewCustomParam(window_manager, "FPS", setFPSMax, &fps, sizeof(unsigned int)); // Add a custom parameter for setting the FPS limit
	 * }
	 * @endcode
	 */
	void (*AddNewCustomParam)(const WindowManager* window_manager, const char* name, void (*param_func)(const WindowManager* window, void* param), const void* param_data, const size_t param_size);

	/**
	 * @brief Sets a custom parameter for the window manager.
	 * @param window The WindowManager instance.
	 * @param name The name of the custom parameter.
	 * @param param The new value of the parameter.
	 *
	 * @code
	 * //Example of usage by getting a custom parameter:
	 * void setFPSMax(const WindowManager* window, void* fps_max)
	 * {
	 *     sfRenderWindow_setFramerateLimit(window->GetWindow(window), *(unsigned int*)fps_max);
	 * }
	 *
	 * int main(void)
	 * {
	 *     WindowManager* window_manager = CreateWindowManager(1080, 720, "SFML_Works", sfDefaultStyle, NULL);
	 *     unsigned int fps = 400;
	 *     window_manager->AddNewCustomParam(window_manager, "FPS", setFPSMax, &fps, sizeof(unsigned int)); // Add a custom parameter for setting the FPS limit
	 *
	 *     fps = 200;
	 *     window_manager->SetCustomParam(window_manager, "FPS", &fps); // Set the FPS limit to 200
	 *     window_manager->SetCustomParam(window_manager, "FPS", STD_CONVERT(unsigned int, 200)); // The use of STD_CONVERT macro is useful to set the value directly
	 * }
	 * @endcode
	 */
	void (*SetCustomParam)(const WindowManager* window, const char* name, const void* param);

	/**
	 * @brief Retrieves a custom parameter from the window manager.
	 * @param window The WindowManager instance.
	 * @param name The name of the custom parameter.
	 * @return The custom parameter data.
	 *
	 * @code
	 * //Example of usage by getting a custom parameter:
	 * void setFPSMax(const WindowManager* window, void* fps_max)
	 * {
	 *     sfRenderWindow_setFramerateLimit(window->GetWindow(window), *(unsigned int*)fps_max);
	 * }
	 *
	 * int main(void)
	 * {
	 *     WindowManager* window_manager = CreateWindowManager(1080, 720, "SFML_Works", sfDefaultStyle, NULL);
	 *     unsigned int fps = 400;
	 *     window_manager->AddNewCustomParam(window_manager, "FPS", setFPSMax, &fps, sizeof(unsigned int)); // Add a custom parameter for setting the FPS limit
	 *
	 *     unsigned int fpsLimt = *(unsigned int*)window_manager->GetCustomParam(window_manager, "FPS"); // Get the current FPS limit
	 * }
	 * @endcode
	 */
	void* (*GetCustomParam)(const WindowManager* window, const char* name);

	/**
	 * @brief Adds a new sound with the specified volume.
	 * @param window_manager The WindowManager instance.
	 * @param name The name of the sound.
	 * @param volume The volume of the sound (0.0 to 100.0).
	 */
	void (*AddNewSound)(const WindowManager* window_manager, const char* name, float volume);

	/**
	 * @brief Sets the volume of a specific sound.
	 * @param window The WindowManager instance.
	 * @param name The name of the sound.
	 * @param volume The volume to set (0.0 to 100.0).
	 */
	void (*SetSound)(const WindowManager* window, const char* name, float volume);

	/**
	 * @brief Retrieves the volume of a specific sound.
	 * @param window The WindowManager instance.
	 * @param name The name of the sound.
	 * @return The current volume of the sound.
	 */
	float (*GetSound)(const WindowManager* window, const char* name);

	/**
	 * @brief Retrieves the current timer value.
	 * @param window The WindowManager instance.
	 * @return The current timer value.
	 */
	float (*GetTimer)(const WindowManager* window);

	/**
	 * @brief Resets the window manager's timer.
	 * @param window The WindowManager instance.
	 */
	void (*ResetTimer)(const WindowManager* window);

	/**
	 * @brief Restarts the window manager's clock.
	 * @param window The WindowManager instance.
	 */
	void (*RestartClock)(const WindowManager* window);

	/**
	 * @brief Retrieves the SFML window object.
	 * @param window The WindowManager instance.
	 * @return The SFML render window.
	 */
	sfRenderWindow* (*GetWindow)(const WindowManager* window);

	/**
	 * @brief Retrieves the current event in the window manager.
	 * @param window The WindowManager instance.
	 * @return The current SFML event.
	 */
	sfEvent* (*GetEvent)(const WindowManager* window);

	/**
	 * @brief Polls for the next event in the window manager.
	 * @param window The WindowManager instance.
	 * @return True if an event is available, false otherwise.
	 */
	sfBool(*PollEvent)(const WindowManager* window);

	/**
	 * @brief Retrieves the size of the window.
	 * @param window The WindowManager instance.
	 * @return The size of the window as an sfVector2u.
	 */
	sfVector2u(*GetSize)(const WindowManager* window);

	sfVector2u(*GetBaseSize)(const WindowManager* window);

	/**
	 * @brief Retrieves the current mouse position in the window.
	 * @param window The WindowManager instance.
	 * @return The mouse position as an sfVector2f.
	 */
	sfVector2f(*GetMousePos)(const WindowManager* window);

	/**
	 * @brief Sets a custom view for the window manager.
	 * @param window The WindowManager instance.
	 * @param view The new custom sfView to set.
	 */
	void(*SetCustomView)(const WindowManager* window, sfView* view);

	/**
	 * @brief Retrieves the current custom view of the window manager.
	 * @param window The WindowManager instance.
	 * @return The current custom sfView.
	 */
	sfView* (*GetCustomView)(const WindowManager* window);

	/**
	 * @brief Sets the default view for the window manager.
	 * @param window The WindowManager instance.
	 */
	void (*SetDefaultView)(const WindowManager* window);

	/**
	 * @brief Retrieves the default view for the window manager.
	 * @param window The WindowManager instance.
	 * @return The default sfView.
	 */
	sfView* (*GetDefaultView)(const WindowManager* window);

	/**
	 * @brief Clears the window with the given color.
	 * @param window The WindowManager instance.
	 * @param color The color to clear the window with.
	 */
	void (*Clear)(const WindowManager* window, sfColor color);

	/**
	 * @brief Displays the window content.
	 * @param window The WindowManager instance.
	 */
	void(*Display)(const WindowManager* window);

	/**
	 * @brief Draws a sprite object to the window.
	 * @param window The WindowManager instance.
	 * @param object The sprite object to draw.
	 * @param states Render states for the sprite.
	 */
	void(*DrawSprite)(const WindowManager* window, const sfSprite* object, sfRenderStates* states);

	/**
	 * @brief Draws a text object to the window.
	 * @param window The WindowManager instance.
	 * @param object The text object to draw.
	 * @param states Render states for the text.
	 */
	void(*DrawText)(const WindowManager* window, const sfText* object, sfRenderStates* states);

	/**
	 * @brief Draws a shape object to the window.
	 * @param window The WindowManager instance.
	 * @param object The shape object to draw.
	 * @param states Render states for the shape.
	 */
	void(*DrawShape)(const WindowManager* window, const sfShape* object, sfRenderStates* states);

	/**
	 * @brief Draws a circle shape object to the window.
	 * @param window The WindowManager instance.
	 * @param object The circle shape object to draw.
	 * @param states Render states for the circle shape.
	 */
	void(*DrawCircleShape)(const WindowManager* window, const sfCircleShape* object, sfRenderStates* states);

	/**
	 * @brief Draws a convex shape object to the window.
	 * @param window The WindowManager instance.
	 * @param object The convex shape object to draw.
	 * @param states Render states for the convex shape.
	 */
	void(*DrawConvexShape)(const WindowManager* window, const sfConvexShape* object, sfRenderStates* states);

	/**
	 * @brief Draws a rectangle shape object to the window.
	 * @param window The WindowManager instance.
	 * @param object The rectangle shape object to draw.
	 * @param states Render states for the rectangle shape.
	 */
	void(*DrawRectangleShape)(const WindowManager* window, const sfRectangleShape* object, sfRenderStates* states);

	/**
	 * @brief Draws a vertex array object to the window.
	 * @param window The WindowManager instance.
	 * @param object The vertex array object to draw.
	 * @param states Render states for the vertex array.
	 */
	void(*DrawVertexArray)(const WindowManager* window, const sfVertexArray* object, sfRenderStates* states);

	/**
	 * @brief Draws a vertex buffer object to the window.
	 * @param window The WindowManager instance.
	 * @param object The vertex buffer object to draw.
	 * @param states Render states for the vertex buffer.
	 */
	void(*DrawVertexBuffer)(const WindowManager* window, const sfVertexBuffer* object, sfRenderStates* states);

	/**
	 * @brief Draws a primitive object to the window.
	 * @param window The WindowManager instance.
	 */
	void(*DrawPrimitives)(const WindowManager* window, const sfVertex* object, size_t vertexCount, sfPrimitiveType type, const sfRenderStates* state);

	/**
	 * @brief Draws an animation object to the window.
	 * @param window The WindowManager instance.
	 * @param object The animation object to draw.
	 * @param states Render states for the animation.
	 */
	void(*DrawAnimation)(const WindowManager* window, const struct Animation* object, sfRenderStates* states);

	/**
	 * @brief Draws particle effects to the window.
	 * @param window The WindowManager instance.
	 * @param object The particles object to draw.
	 * @param states Render states for the particles.
	 */
	void(*DrawParticles)(const WindowManager* window, const struct Particles* object, sfRenderStates* states);


	/**
	 * @brief Destroys the window manager and frees resources.
	 * @param window A pointer to the WindowManager instance to destroy.
	 */
	void (*Destroy)(WindowManager** window);
};

/**
 * @brief Creates a new window manager with the given parameters.
 * @param width The width of the window.
 * @param height The height of the window.
 * @param title The title of the window.
 * @param style The window style (e.g., sfClose).
 * @param settings The context settings for the window.
 * @return A pointer to the newly created WindowManager instance.
 */
WindowManager* CreateWindowManager(const unsigned int width, const unsigned int height, const char* title, const sfUint32 style, const sfContextSettings* settings);
