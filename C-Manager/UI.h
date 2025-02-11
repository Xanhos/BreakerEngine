#pragma once
#include "Tools.h"
#include "WindowManager.h"

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
 */
struct UIObject
{
	UIObject_Data* _Data; /**< Internal data storage for the UI object. */
	sfBool isHover; /**< Indicates whether the UI object is hovered by the cursor. */
	sfBool isClicked; /**< Indicates whether the UI object is clicked. */

	void (*setPosition)(UIObject* object, sfVector2f position); /**< Sets the position of the UI object. */
	void (*setScale)(UIObject* object, sfVector2f scale); /**< Sets the scale of the UI object. */
	void (*setOrigin)(UIObject* object, sfVector2f origin); /**< Sets the origin point of the UI object. */
	void (*setSize)(UIObject* object, sfVector2f size); /**< Sets the size of the UI object. */
	void (*setRotation)(UIObject* object, float rotation); /**< Sets the rotation angle of the UI object. */
	void (*setColor)(UIObject* object, sfColor color); /**< Sets the color of the UI object. */
	void (*setDrawable)(UIObject* object, sfDrawable* drawable); /**< Assigns a drawable object to the UI object. */
	void (*setTexture)(UIObject* object, sfTexture* texture, sfBool reset_rect); /**< Assigns a texture to the UI object. */
	void (*setTextureRect)(UIObject* object, sfIntRect texture_rect); /**< Sets the texture rectangle for the UI object. */
	void (*setUpdateFunction)(UIObject* object, UIUpdateFunction callback); /**< Sets a custom update function for the UI object. */

	const char* (*getName)(UIObject* object); /**< Retrieves the name of the UI object. */
	sfVector2f(*getPosition)(UIObject* object); /**< Retrieves the position of the UI object. */
	sfVector2f(*getScale)(UIObject* object); /**< Retrieves the scale of the UI object. */
	sfVector2f(*getOrigin)(UIObject* object); /**< Retrieves the origin point of the UI object. */
	sfVector2f(*getSize)(UIObject* object); /**< Retrieves the size of the UI object. */
	float (*getRotation)(UIObject* object); /**< Retrieves the rotation angle of the UI object. */
	sfColor(*getColor)(UIObject* object); /**< Retrieves the color of the UI object. */
	sfDrawable* (*getDrawable)(UIObject* object); /**< Retrieves the drawable object of the UI object. */
	sfTexture* (*getTexture)(UIObject* object); /**< Retrieves the texture of the UI object. */
	sfIntRect(*getTextureRect)(UIObject* object); /**< Retrieves the texture rectangle of the UI object. */

	void (*update)(UIObject* object, WindowManager* window); /**< Updates the UI object. */
	void (*render)(WindowManager* window, UIObject* object, sfRenderStates* state); /**< Renders the UI object. */
	void (*destroy)(UIObject** object); /**< Destroys the UI object. */
};

/**
 * @struct UIObject_Transform
 * @brief Structure holding transformation data for a UI object.
 */
struct UIObject_Transform
{
	sfVector2f position; /**< Position of the UI object. */
	sfVector2f scale; /**< Scale of the UI object. */
	sfVector2f origin; /**< Origin point of the UI object. */
	sfVector2f size; /**< Size of the UI object. */
	float rotation; /**< Rotation angle of the UI object. */
};

/**
 * @struct UIObjectManager
 * @brief Manages a collection of UI objects.
 */
struct UIObjectManager
{
	UIObjectManager_Data* _Data; /**< Internal data storage for the UI manager. */

	UIObject* (*push_back)(UIObjectManager* manager, UIObject* object); /**< Adds a new UI object to the manager. */
	UIObject* (*get_by_name)(UIObjectManager* manager, const char* name); /**< Retrieves a UI object by name. */
	UIObject* (*get_by_index)(UIObjectManager* manager, int index); /**< Retrieves a UI object by index. */
	void (*remove_by_pointer)(UIObjectManager* manager, UIObject* object); /**< Removes a UI object using a pointer. */
	void (*remove_by_index)(UIObjectManager* manager, int index); /**< Removes a UI object by index. */
	void (*remove_by_name)(UIObjectManager* manager, char* name); /**< Removes a UI object by name. */
	void (*update)(UIObjectManager* manager, WindowManager* window); /**< Updates all UI objects in the manager. */
	void (*draw)(UIObjectManager* manager, WindowManager* window); /**< Renders all UI objects in the manager. */
	void (*destroy)(UIObjectManager** manager); /**< Destroys the UI manager and its objects. */
};

/**
 * @brief Creates a UI object from a rectangle shape.
 * @param shape The rectangle shape.
 * @param name The name of the UI object.
 * @param mouse_button_trigger The mouse button that triggers interaction.
 * @param key_button_trigger The keyboard key that triggers interaction.
 * @return A pointer to the created UI object.
 */
UIObject* CreateUIObjectFromRectangle(sfRectangleShape* shape, char* name, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger);

/**
 * @brief Creates a UI object from a circle shape.
 * @param shape The circle shape.
 * @param name The name of the UI object.
 * @param mouse_button_trigger The mouse button that triggers interaction.
 * @param key_button_trigger The keyboard key that triggers interaction.
 * @return A pointer to the created UI object.
 */
UIObject* CreateUIObjectFromCircleShape(sfCircleShape* shape, char* name, sfMouseButton mouse_button_trigger, sfKeyCode key_button_trigger);

/**
 * @brief Creates a UI object from a sprite.
 * @param shape The sprite shape.
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
