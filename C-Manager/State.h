#pragma once
#include "WindowManager.h"

/**
 * @file State.h
 * @brief This file contains the function prototypes for managing states in the application.
 */


/**
 * @brief Declares the function prototypes for a state.
 *
 * This macro generates function declarations for a given state, including functions for initialization, updating events, updating logic,
 * rendering, UI rendering, and destruction.
 *
 * @param name The name of the state.
 */
#define DECLARE_HEADER_STATE(name)                               \
void Init##name(WindowManager*);                                  /**< Initializes the state. */ \
void UpdateEvent##name(WindowManager*);                           /**< Updates events for the state. */ \
void Update##name(WindowManager*);                                /**< Updates the logic of the state. */ \
void Render##name(WindowManager*);                                /**< Renders the state. */ \
void UIRender##name(WindowManager*);                              /**< Renders the UI for the state. */ \
void Destroy##name(WindowManager*);                               /**< Destroys the state. */ \

 /**
  * @brief Declares a pragma section for the CRT (C runtime).
  *
  * This macro is used for creating a section in the CRT to add the state functions.
  */
#define DECLARE_SECTION_PRAGMA __pragma(section(".CRT$XCU", read))

  /**
   * @brief Registers a state with the state manager.
   *
   * This macro registers a state by creating a function that adds the state information
   * to the global state list.
   *
   * @param stateName The name of the state to register.
   */
#define REGISTER_C_STATE(stateName)                                            \
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
    * This macro generates empty function definitions for a given state. Useful when defining placeholder states.
    *
    * @param stateName The name of the state.
    */
#define DECLARE_BLANK_STATE(stateName)                                      \
void Init##stateName(WindowManager* windowManager)          /**< Initializes the state. */ \
{                                                           \
                                                            \
}                                                           \
void UpdateEvent##stateName(WindowManager* windowManager)   /**< Updates events for the state. */ \
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
 */
typedef struct StateInfo StateInfo;
struct StateInfo
{
    char name[256]; /**< The name of the state. */

    /**
     * @brief Initializes the state.
     * @param window_manager Pointer to the WindowManager object for managing the application window.
     */
    void (*Init)(WindowManager*);

    /**
     * @brief Handles events for the state.
     * @param window_manager Pointer to the WindowManager object for event handling.
     * @param event Pointer to the SFML event structure.
     */
    void (*UpdateEvent)(WindowManager*, sfEvent*);

    /**
     * @brief Updates the state logic.
     * @param window_manager Pointer to the WindowManager object for state updates.
     */
    void (*Update)(WindowManager*);

    /**
     * @brief Renders the state visuals.
     * @param window_manager Pointer to the WindowManager object for rendering.
     */
    void (*Render)(WindowManager*);

    /**
     * @brief Renders the user interface of the state.
     * @param window_manager Pointer to the WindowManager object for UI rendering.
     */
    void (*UIRender)(WindowManager*);

    /**
     * @brief Destroys and cleans up the state.
     * @param window_manager Pointer to the WindowManager object for cleanup.
     */
    void (*Destroy)(WindowManager*);
};

/**
 * @brief Registers a state with the global state manager.
 *
 * This function is called to register a state using the information provided in the StateInfo structure.
 *
 * @param stateInfo The StateInfo structure containing the state details.
 */
void __RegisterState(StateInfo stateInfo);

/**
 * @brief Retrieves a state by its name.
 *
 * This function returns the state information associated with a given state name.
 *
 * @param name The name of the state.
 * @return The StateInfo structure associated with the specified state name.
 */
StateInfo GetState(char* name);
