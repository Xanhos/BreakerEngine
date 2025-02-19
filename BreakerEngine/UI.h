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
#include "WindowManager.h"


/**
 * @file ui.h
 * @brief This file contains all the logics to make UI elements.
 */

/**
 * @typedef UIObject
 * @brief Represents a UI element with transformation and interaction capabilities.
 */
typedef struct UIObject UIObject;

typedef struct UIObject_Data UIObject_Data;
typedef struct sfDrawable sfDrawable;

typedef struct UIObject_Transform UIObject_Transform;

typedef struct UIObjectManager UIObjectManager;
typedef struct UIObjectManager_Data UIObjectManager_Data;

typedef void (*UIUpdateFunction)(UIObject* object, WindowManager* window);

/**
 * @struct UIObject
 * @brief Structure representing a UI element with various properties and methods.
 *
 * This structure encapsulates the core properties and behaviors of a UI object in the game,
 * including transformations (position, scale, rotation), interactivity (hover, click),
 * rendering, and custom update logic.
 */

struct UIObject
{
    UIObject_Data* _Data; /**< Internal data storage for the UI object, such as the object’s state and properties. */
    sfBool isHover; /**< Indicates whether the UI object is currently hovered by the mouse cursor. */
    sfBool isClicked; /**< Indicates whether the UI object has been clicked. */

    // Transformation functions
    void (*setPosition)(UIObject* object, sfVector2f position); /**< Sets the position of the UI object in the 2D space. */
    void (*setScale)(UIObject* object, sfVector2f scale); /**< Sets the scale (size) of the UI object. */
    void (*setOrigin)(UIObject* object, sfVector2f origin); /**< Sets the origin point of the UI object for transformations. */
    void (*setSize)(UIObject* object, sfVector2f size); /**< Sets the size of the UI object. */
    void (*setRotation)(UIObject* object, float rotation); /**< Sets the rotation angle of the UI object. */
    void (*setColor)(UIObject* object, sfColor color); /**< Sets the color of the UI object. */
    void (*setDrawable)(UIObject* object, sfDrawable* drawable); /**< Assigns a drawable (such as a shape or sprite) to the UI object. */
    void (*setTexture)(UIObject* object, sfTexture* texture, sfBool reset_rect); /**< Assigns a texture to the UI object. */
    void (*setTextureRect)(UIObject* object, sfIntRect texture_rect); /**< Sets the portion of the texture to be used by the UI object. */
    void (*setUpdateFunction)(UIObject* object, UIUpdateFunction callback); /**< Sets a custom function to update the UI object. */
    void (*setIsVisible)(UIObject* object, sfBool is_visible); /**< Sets the visibility of the UI object. */

    // Getter functions
    const char* (*getName)(UIObject* object); /**< Retrieves the name of the UI object. */
    sfVector2f(*getPosition)(UIObject* object); /**< Retrieves the position of the UI object. */
    sfVector2f(*getScale)(UIObject* object); /**< Retrieves the scale of the UI object. */
    sfVector2f(*getOrigin)(UIObject* object); /**< Retrieves the origin point of the UI object. */
    sfVector2f(*getSize)(UIObject* object); /**< Retrieves the size of the UI object. */
    float (*getRotation)(UIObject* object); /**< Retrieves the rotation angle of the UI object. */
    sfColor(*getColor)(UIObject* object); /**< Retrieves the color of the UI object. */
    sfDrawable* (*getDrawable)(UIObject* object); /**< Retrieves the drawable object associated with the UI object. */
    sfTexture* (*getTexture)(UIObject* object); /**< Retrieves the texture of the UI object. */
    sfIntRect(*getTextureRect)(UIObject* object); /**< Retrieves the texture rectangle of the UI object. */
    sfBool(*getIsVisible)(UIObject* object); /**< Checks if the UI object is visible. */

    // Utility functions
    sfBool(*nameIs)(UIObject* object, const char* name); /**< Checks if the UI object’s name matches the provided name. */

    // Update and render functions
    void (*update)(UIObject* object, WindowManager* window); /**< Updates the UI object, possibly changing its state or properties. */
    void (*render)(WindowManager* window, UIObject* object, sfRenderStates* state); /**< Renders the UI object on the screen. */
    void (*destroy)(UIObject** object); /**< Destroys the UI object, freeing its resources. */
};

/**
 * @struct UIObject_Transform
 * @brief Structure holding transformation data for a UI object.
 *
 * This structure stores the various transformation properties (position, scale, size, etc.) for a UI object.
 */
struct UIObject_Transform
{
    sfVector2f position; /**< Position of the UI object in 2D space. */
    sfVector2f scale; /**< Scale of the UI object, affecting its size. */
    sfVector2f origin; /**< The origin point for transformations like rotation and scaling. */
    sfVector2f size; /**< Size of the UI object, defining its width and height. */
    float rotation; /**< The rotation angle of the UI object. */
};

/**
 * @struct UIObjectManager
 * @brief Manages a collection of UI objects.
 *
 * The UIObjectManager is responsible for handling a collection of UI objects,
 * allowing for easy addition, removal, and rendering of multiple UI elements.
 */
struct UIObjectManager
{
    UIObjectManager_Data* _Data; /**< Internal data storage for the UI manager. */

    // Functions to add, retrieve, and remove UI objects
    UIObject* (*push_back)(UIObjectManager* manager, UIObject* object); /**< Adds a new UI object to the manager. */
    UIObject* (*get_by_name)(UIObjectManager* manager, const char* name); /**< Retrieves a UI object by name. */
    UIObject* (*get_by_index)(UIObjectManager* manager, int index); /**< Retrieves a UI object by its index in the manager. */
    void (*remove_by_pointer)(UIObjectManager* manager, UIObject* object); /**< Removes a UI object using its pointer. */
    void (*remove_by_index)(UIObjectManager* manager, int index); /**< Removes a UI object by its index. */
    void (*remove_by_name)(UIObjectManager* manager, char* name); /**< Removes a UI object by name. */

    // Functions for updating and rendering all UI objects
    void (*update)(UIObjectManager* manager, WindowManager* window); /**< Updates all UI objects in the manager. */
    void (*draw)(UIObjectManager* manager, WindowManager* window, sfRenderStates* state); /**< Renders all UI objects in the manager. */
    void (*destroy)(UIObjectManager** manager); /**< Destroys the UI manager and all its managed UI objects. */
};

/**
 * @brief Creates a UI object from a rectangle shape.
 * @param shape The rectangle shape (e.g., sfRectangleShape).
 * @param name The name of the UI object.
 * @param mouse_button_trigger The mouse button that triggers interaction.
 * @param key_button_trigger The keyboard key that triggers interaction.
 * @return A pointer to the created UI object.
 */
UIObject* CreateUIObjectFromRectangle(sfRectangleShape* shape, char* name, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger);

/**
 * @brief Creates a UI object from a circle shape.
 * @param shape The circle shape (e.g., sfCircleShape).
 * @param name The name of the UI object.
 * @param mouse_button_trigger The mouse button that triggers interaction.
 * @param key_button_trigger The keyboard key that triggers interaction.
 * @return A pointer to the created UI object.
 */
UIObject* CreateUIObjectFromCircleShape(sfCircleShape* shape, char* name, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger);

/**
 * @brief Creates a UI object from a sprite.
 * @param shape The sprite shape (e.g., sfSprite).
 * @param name The name of the UI object.
 * @param mouse_button_trigger The mouse button that triggers interaction.
 * @param key_button_trigger The keyboard key that triggers interaction.
 * @return A pointer to the created UI object.
 */
UIObject* CreateUIObjectFromSprite(sfSprite* shape, char* name, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger);

/**
 * @brief Creates a UI object manager.
 * @return A pointer to the newly created UI object manager.
 */
UIObjectManager* CreateUIObjectManager();
