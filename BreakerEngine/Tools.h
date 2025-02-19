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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "stdString.h"
#include "FileSystem.h"
#include "SFML/Graphics.h"
#include "SFML/Audio.h"
#include "SFML/System.h"

/**
 * @file tools.h
 * @brief This file contains all the useful tools for the engine.
 *
 * It defines structures and global variables for managing the game window,
 * handling transformations (such as position, velocity, and angle), and scaling.
 */

struct WindowManager* GameWindow; /**< Pointer to the WindowManager structure responsible for managing the game window. */
float ScreenScaleFactorX; /**< Scaling factor for the X-axis of the screen, used for scaling graphics to the window size. */
float ScreenScaleFactorY; /**< Scaling factor for the Y-axis of the screen, used for scaling graphics to the window size. */

/**
 * @struct Transform
 * @brief Structure that represents the transformation of an object in the game.
 *
 * It stores the position, velocity, and angle of an object to handle its movement and rotation.
 */
typedef struct Transform Transform;
struct Transform
{
	sfVector2f position; /**< The position of the object in 2D space. */
	sfVector2f velocity; /**< The velocity of the object in 2D space, affecting its movement. */
	float angle; /**< The angle of rotation for the object in degrees. */
};

/**
 * @def DeltaTime
 * @brief The time elapsed between the current and last frame.
 */
float DeltaTime;

/**
 * @def resource_directory
 * @brief The directory containing the game resources.
 */
char resource_directory[MAX_PATH_SIZE];

/**
 * @def MAX_THREAD
 * @brief The maximum number of threads allowed.
 */
#define MAX_THREAD 10

 /**
 * @def KEY(key)
 * @brief Macro to check if a specific key is currently pressed.
 * @param key The key to check (e.g., A, B, etc.).
 * @return Boolean value indicating if the key is pressed.
 */
#define KEY(key) sfKeyboard_isKeyPressed(sfKey##key)

 /**
 * @def KEY_DOWN(key)
 * @brief Macro to check if a specific key has been pressed (down event).
 * @param key The key to check (e.g., A, B, etc.).
 * @return Boolean value indicating if the key is pressed.
 */
#define KEY_DOWN(key) sfKeyboard_isKeyDown(sfKey##key)

 /**
 * @def KEY_UP(key)
 * @brief Macro to check if a specific key has been released (up event).
 * @param key The key to check (e.g., A, B, etc.).
 * @return Boolean value indicating if the key is released.
 */
#define KEY_UP(key) sfKeyboard_isKeyUp(sfKey##key)
 /**
  * @def KEY(key)
  * @brief Macro to check if a specific key is currently pressed.
  * @param key The key to check (e.g., A, B, etc.).
  * @return Boolean value indicating if the key is pressed.
  */
#define MOUSE(button) sfMouse_isButtonPressed(sfMouse##button)

  /**
   * @def KEY_DOWN(key)
   * @brief Macro to check if a specific key has been pressed (down event).
   * @param key The key to check (e.g., A, B, etc.).
   * @return Boolean value indicating if the key is pressed.
   */
#define MOUSE_DOWN(button) sfMouse_isButtonDown(sfMouse##button)

   /**
	* @def KEY_UP(key)
	* @brief Macro to check if a specific key has been released (up event).
	* @param key The key to check (e.g., A, B, etc.).
	* @return Boolean value indicating if the key is released.
	*/
#define MOUSE_UP(button) sfMouse_isButtonUp(sfMouse##button)

	/**
	* @def STD_LIST_CREATE_POINTER(type, size, ...)
	* @brief Macro to create a list of pointers of a specific type.
	* @param type The type of data.
	* @param size The size of the list.
	*/
#define STD_LIST_CREATE_POINTER(type, size, ...) \
  stdList_Create(sizeof(type*), size, __VA_ARGS__)


	/**
	* @def STD_VECTOR_CREATE_POINTER(type, size, ...)
	* @brief Macro to create a vector of pointers of a specific type.
	* @param type The type of data.
	* @param size The size of the vector.
	*/
#define STD_VECTOR_CREATE_POINTER(type, size, ...) \
  stdVector_Create(sizeof(type*), size, __VA_ARGS__)

	/**
	* @def FOR_EACH_TEMP_LIST(name, type, initFunc, func)
	* @brief Macro to iterate over a temporary list.
	* @param name The name of the list.
	* @param type The type of elements in the list.
	* @param initFunc The function to initialize the list.
	* @param func The function to apply to each element.
	*/
#define FOR_EACH_TEMP_LIST(name, type, initFunc, func) \
  stdList* name = initFunc; \
  for (int i = 0; i < name->size(name); i++) { func } \
  name->destroy(&name);

	/**
	* @def FOR_EACH_LIST(list, type, it_name, data_container_name, func)
	* @brief Macro to iterate over a list and apply a function to each element.
	* @param list The list to iterate over.
	* @param type The type of elements in the list.
	* @param it_name The iterator name.
	* @param data_container_name The name for the data container.
	* @param func The function to apply to each element.
	*/
#define FOR_EACH_LIST(list, type, it_name, data_container_name, func) \
  for (int it_name = 0; it_name < list->size(list); it_name++) { \
    type* data_container_name = STD_GETDATA(list, type, it_name); \
    func }

	/**
	* @def FOR_EACH_LIST_POINTER(list, type, it_name, data_container_name, func)
	* @brief Macro to iterate over a list of pointers and apply a function to each element.
	* @param list The list to iterate over.
	* @param type The type of elements in the list.
	* @param it_name The iterator name.
	* @param data_container_name The name for the data container.
	* @param func The function to apply to each element.
	*/
#define FOR_EACH_LIST_POINTER(list, type, it_name, data_container_name, func) \
  for (int it_name = 0; it_name < list->size(list); it_name++) { \
    type* data_container_name##_ = STD_GETDATA(list, type, it_name); \
    type data_container_name = *data_container_name##_; \
    func }

	//------------------------------------------VECTOR FUNCTION----------------------------------------------//

	/**
	* @def DECLARE_ADD_VECTOR2_IN_C(vector_type, suffixe, type)
	* @brief Declares a function to add two vectors.
	* @param vector_type The type of vector (e.g., sfVector2f).
	* @param suffixe The suffix for the function (e.g., f for float).
	* @param type The type of scalar used for multiplication (e.g., float).
	*/
#define DECLARE_ADD_VECTOR2_IN_C(vector_type, suffixe, type) \
  vector_type AddVector2##suffixe(vector_type a, vector_type b) { \
    return (vector_type){a.x + b.x, a.y + b.y}; \
  }

	/**
	* @def DECLARE_SUB_VECTOR2_IN_C(vector_type, suffixe, type)
	* @brief Declares a function to subtract two vectors.
	* @param vector_type The type of vector (e.g., sfVector2f).
	* @param suffixe The suffix for the function (e.g., f for float).
	* @param type The type of scalar used for multiplication (e.g., float).
	*/
#define DECLARE_SUB_VECTOR2_IN_C(vector_type, suffixe, type) \
  vector_type SubVector2##suffixe(vector_type a, vector_type b) { \
    return (vector_type){a.x - b.x, a.y - b.y}; \
  }

	/**
	* @def DECLARE_MULTIPLY_VECTOR2_IN_C(vector_type, suffixe, type)
	* @brief Declares a function to multiply a vector by a scalar.
	* @param vector_type The type of vector (e.g., sfVector2f).
	* @param suffixe The suffix for the function (e.g., f for float).
	* @param type The type of scalar used for multiplication (e.g., float).
	*/
#define DECLARE_MULTIPLY_VECTOR2_IN_C(vector_type, suffixe, type) \
  vector_type MultiplyVector2##suffixe(vector_type a, type b) { \
    return (vector_type){a.x * b, a.y * b}; \
  }

	/**
 * @def DECLARE_DIVIDE_VECTOR2_IN_C(vector_type, suffixe, type)
 * @brief Declares a function to divide a vector by a scalar.
 * @param vector_type The type of vector (e.g., sfVector2f).
 * @param suffixe The suffix for the function (e.g., f for float).
 * @param type The type of scalar used for multiplication (e.g., float).
 */
#define DECLARE_DIVIDE_VECTOR2_IN_C(vector_type, suffixe, type) \
  vector_type DivideVector2##suffixe(vector_type a, type b) { \
    return (vector_type){a.x / b, a.y / b}; \
  }

 /**
 * @def DECLARE_CREATE_METHODE_VECTOR2_IN_C(vector_type, suffixe, type)
 * @brief Declares a function to create a vector of specified type and values.
 * @param vector_type The type of vector (e.g., sfVector2f).
 * @param suffixe The suffix for the function (e.g., f for float).
 * @param type The type of scalar used for vector creation (e.g., float).
 */
#define DECLARE_CREATE_METHODE_VECTOR2_IN_C(vector_type, suffixe, type) \
  vector_type vector_type##_Create(type a, type b) { \
    return (vector_type){a, b}; \
  }

 /**
 * @def DECLARE_PRINT_VECTOR_VALUE_IN_C(vector_type, suffixe, type)
 * @brief Declares a function to print the values of a vector.
 * @param vector_type The type of vector (e.g., sfVector2f).
 * @param suffixe The suffix for the function (e.g., f for float).
 * @param type The type of scalar used for printing (e.g., float).
 */
#define DECLARE_PRINT_VECTOR_VALUE_IN_C(vector_type, suffixe, type) \
  void PrintVector2##suffixe(vector_type vec, sfBool go_to_next_line) { \
    NEW_CHAR(sentence_debug, 100) \
    strcat_s(sentence_debug, 100, "x = %"); \
    strcat_s(sentence_debug, 100, #suffixe); \
    strcat_s(sentence_debug, 100, " y = %"); \
    strcat_s(sentence_debug, 100, #suffixe); \
    printf_d(sentence_debug, vec.x, vec.y); \
    if (go_to_next_line) \
      printf("\n"); \
  }

 // Declare basic vector operations for vector2 of any type, such as sfVector2f, sfVector2i, or sfVector2u.

#define DECLARE_ALL_BASICS_OPERATION_VECTOR2_IN_H(vector_type, suffix, type)   \
    /**                                                                 \
     * @brief Adds two vectors of type vector_type.                      \
     * @param a First vector.                                             \
     * @param b Second vector.                                            \
     * @return The result of adding vectors a and b.                      \
     */                                                                   \
    vector_type AddVector2##suffix(vector_type a, vector_type b);          \
                                                                            \
    /**                                                                 \
     * @brief Subtracts vector b from vector a.                           \
     * @param a First vector.                                             \
     * @param b Second vector.                                            \
     * @return The result of subtracting vector b from vector a.           \
     */                                                                   \
    vector_type SubVector2##suffix(vector_type a, vector_type b);          \
                                                                            \
    /**                                                                 \
     * @brief Multiplies a vector by a scalar value.                       \
     * @param a Vector to multiply.                                       \
     * @param b Scalar value to multiply the vector by.                    \
     * @return The result of multiplying vector a by scalar b.             \
     */                                                                   \
    vector_type MultiplyVector2##suffix(vector_type a, type b);            \
                                                                            \
    /**                                                                 \
     * @brief Divides a vector by a scalar value.                          \
     * @param a Vector to divide.                                          \
     * @param b Scalar value to divide the vector by.                       \
     * @return The result of dividing vector a by scalar b.                \
     */                                                                   \
    vector_type DivideVector2##suffix(vector_type a, type b);              \
                                                                            \
    /**                                                                 \
     * @brief Creates a vector of type vector_type with specified values. \
     * @param a First component of the vector.                             \
     * @param b Second component of the vector.                            \
     * @return A vector of type vector_type created with values a and b.   \
     */                                                                   \
    vector_type vector_type##_Create(type a, type b);                      \
                                                                            \
    /**                                                                 \
     * @brief Prints the components of the vector.                         \
     * @param vec The vector to print.                                    \
     */                                                                   \
    void PrintVector2##suffix(vector_type vec);

// Implement basic vector operations for vector2 of any type (e.g., sfVector2f, sfVector2i, sfVector2u).

#define DECLARE_ALL_BASICS_OPERATION_VECTOR2_IN_C(vector_type, suffix, type)  \
    DECLARE_ADD_VECTOR2_IN_C(vector_type, suffix, type)                       \
    DECLARE_SUB_VECTOR2_IN_C(vector_type, suffix, type)                       \
    DECLARE_MULTIPLY_VECTOR2_IN_C(vector_type, suffix, type)                  \
    DECLARE_DIVIDE_VECTOR2_IN_C(vector_type, suffix, type)                    \
    DECLARE_CREATE_METHODE_VECTOR2_IN_C(vector_type, suffix, type)            \
    DECLARE_PRINT_VECTOR_VALUE_IN_C(vector_type, suffix, type)

// Declare operations for specific vector types

DECLARE_ALL_BASICS_OPERATION_VECTOR2_IN_H(sfVector2f, f, float)
DECLARE_ALL_BASICS_OPERATION_VECTOR2_IN_H(sfVector2i, i, int)
DECLARE_ALL_BASICS_OPERATION_VECTOR2_IN_H(sfVector2u, u, unsigned int)


//------------------------------------------MATHS FUNCTION----------------------------------------------//

/**
* @def PI
* @brief The constant value of Pi.
*/
#define PI 3.14159265359

/**
* @def RAD2DEG
* @brief The constant to convert radians to degrees.
*/
#define RAD2DEG 57.295779f

/**
* @def DEG2RAD
* @brief The constant to convert degrees to radians.
*/
#define DEG2RAD 0.017453f

/**
* @def LERP(a, b, t)
* @brief Linearly interpolates between two values.
* @param a The start value.
* @param b The end value.
* @param t The interpolation factor.
* @return The interpolated value.
*/
#define LERP(a, b, t) (b - a) * t + a

#define INVERT_LERP(a, b, value) (value - a) / (b - a)

/**
* @def MAX(a, b)
* @brief Returns the maximum of two values.
* @param a The first value.
* @param b The second value.
* @return The maximum of a and b.
*/
#define MAX(a, b) a >= b ? a : b

/**
* @def MIN(a, b)
* @brief Returns the minimum of two values.
* @param a The first value.
* @param b The second value.
* @return The minimum of a and b.
*/
#define MIN(a, b) a <= b ? a : b

/**
 * @brief Checks if two circles are in collision.
 * @param _pos1 The position of the first circle.
 * @param _pos2 The position of the second circle.
 * @param _rayon1 The radius of the first circle.
 * @param _rayon2 The radius of the second circle.
 * @param shouldScale Whether to apply scaling adjustments.
 * @return True if there is a collision, false otherwise.
 */
sfBool Circle_Collision(sfVector2f _pos1, sfVector2f _pos2, float _rayon1, float _rayon2, sfBool shouldScale);

/**
 * @brief Checks if two rectangles are in collision.
 * @param _box1 The first rectangle (position, width, height).
 * @param _box2 The second rectangle (position, width, height).
 * @param shouldScale Whether to apply scaling adjustments.
 * @return True if there is a collision, false otherwise.
 */
sfBool Rectangle_Collision(sfFloatRect _box1, sfFloatRect _box2, sfBool shouldScale);

/**
 * @brief Checks if a circle and a rectangle are in collision.
 * @param pos The position of the circle.
 * @param radius The radius of the circle.
 * @param _rect The rectangle (position, width, height).
 * @param shouldScale Whether to apply scaling adjustments.
 * @return True if there is a collision, false otherwise.
 */
sfBool CircleRect_Collision(sfVector2f pos, float radius, sfFloatRect _rect, sfBool shouldScale);

/**
 * @brief Checks if a line and a circle are in collision.
 * @param start_line The start position of the line.
 * @param end_line The end position of the line.
 * @param circle_pos The position of the circle.
 * @param r The radius of the circle.
 * @param shouldScale Whether to apply scaling adjustments.
 * @return True if there is a collision, false otherwise.
 */
sfBool LineCircle_Collision(sfVector2f start_line, sfVector2f end_line, sfVector2f circle_pos, float r, sfBool shouldScale);

/**
 * @brief Checks if a point is on a line segment.
 * @param start_line The start position of the line.
 * @param end_line The end position of the line.
 * @param point The point to check.
 * @param offset The tolerance offset.
 * @param shouldScale Whether to apply scaling adjustments.
 * @return True if the point is on the line, false otherwise.
 */
sfBool LinePoint_Collision(sfVector2f start_line, sfVector2f end_line, sfVector2f point, float offset, sfBool shouldScale);

/**
 * @brief Checks if two line segments intersect.
 * @param start_line_one The start position of the first line.
 * @param end_line_one The end position of the first line.
 * @param start_line_two The start position of the second line.
 * @param end_line_two The end position of the second line.
 * @param shouldScale Whether to apply scaling adjustments.
 * @return True if there is a collision, false otherwise.
 */
sfBool LineLine_Collision(sfVector2f start_line_one, sfVector2f end_line_one, sfVector2f start_line_two, sfVector2f end_line_two, sfBool shouldScale);

/**
 * @brief Checks if a line and a rectangle are in collision.
 * @param start_line The start position of the line.
 * @param end_line The end position of the line.
 * @param rect The rectangle (position, width, height).
 * @param shouldScale Whether to apply scaling adjustments.
 * @return True if there is a collision, false otherwise.
 */
sfBool LineRect_Collision(sfVector2f start_line, sfVector2f end_line, sfFloatRect rect, sfBool shouldScale);

/**
 * @brief Checks if a point is inside a circle.
 * @param _pos The point to check.
 * @param _circle_pos The position of the circle.
 * @param _rayon The radius of the circle.
 * @param shouldScale Whether to apply scaling adjustments.
 * @return True if the point is inside the circle, false otherwise.
 */
sfBool PointInCircle(sfVector2f _pos, sfVector2f _circle_pos, float _rayon, sfBool shouldScale);

/**
 * @brief Checks if a point is inside a rectangle.
 * @param _pos The point to check.
 * @param _box The rectangle (position, width, height).
 * @param shouldScale Whether to apply scaling adjustments.
 * @return True if the point is inside the rectangle, false otherwise.
 */
sfBool PointInRectangle(sfVector2f _pos, sfFloatRect _box, sfBool shouldScale);

/**
 * @brief Generates a random integer between the given minimum and maximum values.
 * @param _min The minimum value.
 * @param _max The maximum value.
 * @return A random integer between _min and _max.
 */
int rand_int(int _min, int _max);

/**
 * @brief Generates a random float between the given minimum and maximum values.
 * @param _min The minimum value.
 * @param _max The maximum value.
 * @return A random float between _min and _max.
 */
double rand_float(float _min, float _max);

/**
 * @brief Returns a random position within a given rectangle.
 * @param rect The rectangle (position, width, height) to get the random position within.
 * @return A random position inside the rectangle.
 */
sfVector2f GetRandomPosInRectangle(sfFloatRect rect);

/**
 * @brief Returns a random position within a given circle.
 * @param pos The center position of the circle.
 * @param radius The radius of the circle.
 * @return A random position inside the circle.
 */
sfVector2f GetRandomPosInCircle(sfVector2f pos, float radius);

/**
 * @brief Computes the distance between two points.
 * @param a_ The first point.
 * @param b_ The second point.
 * @return The distance between the two points.
 */
float GetDistance(sfVector2f a_, sfVector2f b_);

/**
 * @brief Computes the norm (magnitude) of a vector.
 * @param _vec The vector.
 * @return The norm of the vector.
 */
float GetNorme(sfVector2f _vec);

/**
 * @brief Normalizes a vector (scales it to have a unit length).
 * @param _vec The vector to normalize.
 * @return The normalized vector.
 */
sfVector2f NormalizeVector2f(sfVector2f _vec);

/**
 * @brief Creates a vector from an angle and length.
 * @param pos The position of the vector.
 * @param lenght The length of the vector.
 * @param angle The angle of the vector in degrees.
 * @return The created vector.
 */
sfVector2f GetVectorFromAngle(sfVector2f pos, float lenght, float angle);

/**
 * @brief Creates an sfColor with the specified red, green, blue, and alpha values.
 * @param r The red component (0-255).
 * @param g The green component (0-255).
 * @param b The blue component (0-255).
 * @param alpha The alpha (transparency) component (0-255).
 * @return The created color.
 */
sfColor CreateColor(int r, int g, int b, int alpha);

/**
 * @brief Clamps an integer value between a minimum and maximum.
 * @param value The value to clamp.
 * @param min The minimum value.
 * @param max The maximum value.
 * @return The clamped value.
 */
int iClamp(int value, int min, int max);

/**
 * @brief Clamps a float value between a minimum and maximum.
 * @param value The value to clamp.
 * @param min The minimum value.
 * @param max The maximum value.
 * @return The clamped value.
 */
float fClamp(float value, float min, float max);

/**
 * @brief Prints a debug message with optional arguments.
 * @param string The format string.
 * @param ... The arguments to include in the debug message.
 */
#define printf_d(string, ...) DebugPrint(string, __VA_ARGS__)

 /**
  * @brief Structure for managing thread information.
  */
typedef struct Thread_Info thread_info;

/**
 * @struct Thread_Info
 * @brief Contains information about the files to process in a thread.
 */
struct Thread_Info
{
	stdList* files_info; /**< The list of files to process. */
	int start; /**< The starting index for processing. */
	int end; /**< The ending index for processing. */
	float* progressValue; /**< Pointer to track the progress value. */
	float* totalSize; /**< Pointer to track the total size of the files. */
	float* currentSize; /**< Pointer to track the current size during processing. */
	void (*func)(const char*); /**< The function to apply to each file. */
};

/**
 * @typedef clock_data
 * @brief Structure for internal data related to the clock.
 */
typedef struct clock_data clock_data;

/**
 * @typedef Clock
 * @brief Structure for managing time and frame rate.
 */
typedef struct Clock Clock;

/**
 * @struct Clock
 * @brief Structure for managing time and frame rate.
 */
struct Clock
{
	void (*restartClock)(Clock* clock); /**< Restarts the clock. */
	float (*getDeltaTime)(Clock* clock); /**< Returns the time difference between frames. */
	float (*frameRate)(Clock* clock); /**< Returns the current frame rate. */
	void (*destroy)(Clock** clock); /**< Destroys the clock and frees resources. */
	clock_data* _Data; /**< Internal data for managing the clock. */
};

/**
 * @brief Creates a new clock.
 * @return A pointer to the newly created Clock.
 */
Clock* CreateClock(void);

/**
 * @brief Converts a string to lowercase.
 * @param sentence The string to convert.
 */
void ToLower(char* sentence);

/**
 * @brief Converts a string to uppercase.
 * @param sentence The string to convert.
 */
void ToUpper(char* sentence);

/**
 * @brief Allocates memory and copies a string into it.
 * @param name The string to copy.
 * @return A newly allocated string containing the copied content.
 */
char* StrAllocNCopy(const char* name);

/**
 * @brief Searches for files with a specified extension in a given path.
 * @param path The directory path to search.
 * @param extension The file extension to search for.
 * @return A list of file information.
 */
stdList* SearchFilesInfos(const char* path, const char* extension);

/**
 * @brief Structure for managing file information.
 */
typedef struct FilesInfo FilesInfo;

/**
 * @struct FilesInfo
 * @brief Contains file path and name.
 */
struct FilesInfo
{
	char m_name[MAX_PATH_SIZE]; /**< The name of the file. */
	char m_path[MAX_PATH_SIZE]; /**< The full path to the file. */
};

/**
 * @brief Checks if a specific key is currently pressed down.
 *
 * This function returns whether a given key is pressed down in the current frame.
 *
 * @param key The key to check.
 * @return sfTrue if the key is currently pressed down, sfFalse otherwise.
 */
sfBool sfKeyboard_isKeyDown(sfKeyCode key);

/**
 * @brief Checks if a specific key has been released.
 *
 * This function returns whether a given key has been released in the current frame.
 *
 * @param key The key to check.
 * @return sfTrue if the key is currently released, sfFalse otherwise.
 */
sfBool sfKeyboard_isKeyUp(sfKeyCode key);

/**
 * @brief Checks if a specific mouse button is currently pressed down.
 *
 * This function returns whether a given mouse button is pressed down in the current frame.
 *
 * @param button The mouse button to check.
 * @return sfTrue if the mouse button is currently pressed down, sfFalse otherwise.
 */
sfBool sfMouse_isButtonDown(sfMouseButton button);

/**
 * @brief Checks if a specific mouse button has been released.
 *
 * This function returns whether a given mouse button has been released in the current frame.
 *
 * @param button The mouse button to check.
 * @return sfTrue if the mouse button is currently released, sfFalse otherwise.
 */
sfBool sfMouse_isButtonUp(sfMouseButton button);

/**
 * @brief Prints a formatted debug message.
 *
 * This function allows for formatted printing to the debug console, similar to printf.
 * It supports variable arguments to print dynamic data.
 *
 * @param string The format string that specifies how the arguments should be printed.
 * @param ... The arguments to format and print.
 */
void DebugPrint(const char* const string, ...);

/**
 * @brief Get the size of a file.
 *
 * @param filePath the path of the file.
 * @return The size of the file in bytes.
 */
float GetFileSizeCustom(const char* filePath);

/**
 * @brief Loads files in a separate thread.
 *
 * This function loads the specified files asynchronously in a separate thread to avoid blocking the main thread.
 *
 * @param thread_infos The information about the thread to load the files.
 */
void __LoadWithThread(void* thread_infos);

/**
 * @brief Loads a scene from a file.
 *
 * This function loads a scene from the specified file path, with support for different file types and extensions.
 * A callback function can be provided to perform actions after the scene is loaded.
 *
 * @param scene The scene to load.
 * @param extension The file extension of the scene.
 * @param type The type of scene to load.
 * @param progressValue A pointer to track the progress of the loading.
 * @param func A callback function to execute after loading the scene.
 */
void __LoadScene(const char* scene, const char* extension, const char* type, float* progressValue, void (*func)(const char*));

/**
 * @brief Updates the key and mouse states.
 *
 * This function updates the current state of the keys and mouse buttons for the current frame.
 */
void UpdateKeyAndMouseState(void);
