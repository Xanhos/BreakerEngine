#pragma once
#include "WindowManager.h"

/**
 * @file State.h
 * @brief This file contains the function prototypes for managing states in the application.
 * It allows for dynamic management of different states in the application, with a lifecycle that includes initialization, updates, rendering, and destruction.
 */


 /**
  * @brief Declares the function prototypes for a state.
  *
  * This macro generates function declarations for a given state, including functions for initialization, updating events, updating logic,
  * rendering, UI rendering, and destruction.
  *
  * @param name The name of the state.
  */
#define DECLARE_HEADER_STATE(name)   \
void Init##name(WindowManager*);                                  /**< Initializes the state. */ \
void UpdateEvent##name(WindowManager*, sfEvent*);                           /**< Updates events for the state. */ \
void Update##name(WindowManager*);                                /**< Updates the logic of the state. */ \
void Render##name(WindowManager*);                                /**< Renders the state. */ \
void UIRender##name(WindowManager*);                              /**< Renders the UI for the state. */ \
void Destroy##name(WindowManager*);                               /**< Destroys the state. */ \

  /**
   * @brief Declares a pragma section for the CRT (C runtime).
   *
   * This macro is used for creating a section in the CRT to add the state functions.
   * The section will ensure that the state functions are included in the program at the right point during initialization.
   */
#define DECLARE_SECTION_PRAGMA __pragma(section(".CRT$XCU", read))

   /**
    * @brief Registers a state with the state manager.
    *
    * This macro registers a state by creating a function that adds the state information
    * to the global state list. It will ensure the proper handling of states in the application lifecycle.
    *
    * @param stateName The name of the state to register.
    */
#define REGISTER_STATE(stateName)                                            \
    static void AddState##stateName##ToStateList()                             \
    {                                                                          \
        printf("Adding state %s\n", #stateName);                              /**< Prints the state being added. */ \
                                                                               \
        StateInfo info = {.name = #stateName,                                  \
                          .Init = &Init##stateName,                            /**< Pointer to the initialization function. */ \
                          .UpdateEvent = &UpdateEvent##stateName,              /**< Pointer to the update event function. */ \
                          .Update = &Update##stateName,                        /**< Pointer to the update function. */ \
                          .Render = &Render##stateName,                        /**< Pointer to the render function. */ \
                          .UIRender = &UIRender##stateName,                    /**< Pointer to the UI render function. */ \
                          .Destroy = &Destroy##stateName};                     /**< Pointer to the destroy function. */ \
        __RegisterState(info);                                                 /**< Registers the state information. */ \
    }                                                                          \
    DECLARE_SECTION_PRAGMA                                                     \
     __declspec(allocate(".CRT$XCU")) void (*p_register_##stateName##_function)() = AddState##stateName##ToStateList; /**< Declares the registration function. */ \

    /**
     * @brief Declares a blank state with empty function definitions.
     *
     * This macro generates empty function definitions for a given state. Useful when defining placeholder states
     * or when you want to create a state with no actual logic implemented yet.
     *
     * @param stateName The name of the state.
     */
#define DECLARE_BLANK_STATE(stateName) \
void Init##stateName(WindowManager* windowManager)          /**< Initializes the state. */ \
{                                                           \
                                                            \
}                                                           \
void UpdateEvent##stateName(WindowManager* windowManager, sfEvent* evt)   /**< Updates events for the state. */ \
{                                                           \
                                                            \
}                                                           \
void Update##stateName(WindowManager* windowManager)        /**< Updates the logic of the state. */ \
{                                                           \
                                                            \
}                                                           \
void Render##stateName(WindowManager* windowManager)        /**< Renders the state. */ \
{                                                           \
                                                            \
}                                                           \
void UIRender##stateName(WindowManager* windowManager)      /**< Renders the UI for the state. */ \
{                                                           \
                                                            \
}                                                           \
void Destroy##stateName(WindowManager* windowManager)       /**< Destroys the state. */ \
{                                                           \
                                                            \
}                                                           \

     /**
      * @struct StateInfo
      * @brief Contains function pointers for managing the lifecycle of a state in the application.
      * This structure allows the state manager to store the necessary functions for each state and execute them at the appropriate points in the application's lifecycle.
      */
typedef struct StateInfo StateInfo;
struct StateInfo
{
    char name[256]; /**< The name of the state. */

    /**
     * @brief Initializes the state.
     * @param window_manager Pointer to the WindowManager object for managing the application window.
     * This function is called once when the state is first created, and it should set up any necessary resources for the state.
     */
    void (*Init)(WindowManager*);

    /**
     * @brief Handles events for the state.
     * @param window_manager Pointer to the WindowManager object for event handling.
     * @param event Pointer to the SFML event structure.
     * This function handles input events like key presses or mouse clicks specific to this state.
     */
    void (*UpdateEvent)(WindowManager*, sfEvent*);

    /**
     * @brief Updates the state logic.
     * @param window_manager Pointer to the WindowManager object for state updates.
     * This function is called on every frame update to process the logic of the state, such as moving objects or updating variables.
     */
    void (*Update)(WindowManager*);

    /**
     * @brief Renders the state visuals.
     * @param window_manager Pointer to the WindowManager object for rendering.
     * This function is called every frame to render the state’s visuals to the screen, such as sprites, backgrounds, etc.
     */
    void (*Render)(WindowManager*);

    /**
     * @brief Renders the user interface of the state.
     * @param window_manager Pointer to the WindowManager object for UI rendering.
     * This function is called to render any UI elements specific to the state, like buttons, labels, etc.
     */
    void (*UIRender)(WindowManager*);

    /**
     * @brief Destroys and cleans up the state.
     * @param window_manager Pointer to the WindowManager object for cleanup.
     * This function is called when the state is being destroyed to release any resources and cleanup.
     */
    void (*Destroy)(WindowManager*);
};

/**
 * @brief Registers a state with the global state manager.
 *
 * This function is called to register a state using the information provided in the StateInfo structure.
 * It ensures that the state can be accessed and managed by the global state manager.
 *
 * @param stateInfo The StateInfo structure containing the state details.
 */
void __RegisterState(StateInfo stateInfo);

/**
 * @brief Retrieves a state by its name.
 *
 * This function returns the state information associated with a given state name.
 * The returned structure can then be used to perform operations on the state.
 *
 * @param name The name of the state.
 * @return The StateInfo structure associated with the specified state name.
 */
StateInfo GetState(const char* name);
