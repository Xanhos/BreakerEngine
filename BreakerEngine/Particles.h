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
 * @file particles.h
 * @brief This file defines the Particles system structure and functions for managing a particles.
 */

 /**
  * @typedef Particles_Data
  * @brief Opaque structure that holds the internal data of the particles system.
  */
typedef struct Particles_Data Particles_Data;

/**
 * @typedef Particles
 * @brief Represents the particle system and its associated data.
 */
typedef struct Particles Particles;

/**
 * @typedef ParticleParam
 * @brief Parameters for configuring the behavior and properties of particles.
 */
typedef struct ParticleParam ParticleParam;

/**
 * @enum ParticlesTypes
 * @brief Enumerates the different types of particle behaviors.
 */
typedef enum ParticlesTypes ParticlesTypes;

/**
 * @enum ParticlesTypes
 * @brief Enumerates the different types of particle behaviors.
 */
enum ParticlesTypes
{
	NONE = -1,        /**< No particle behavior. */
	ONE_TIME,         /**< Particles that exist once and do not loop. */
	LIFE_TIME,        /**< Particles that have a lifetime and despawn after a set time. */
	ALWAYS            /**< Particles that continue indefinitely. */
};

/**
 * @enum ParticlesTypes
 * @brief Enumerates the different types of particle behaviors.
 */
typedef enum FadingTypes FadingTypes;

/**
 * @enum FadingTypes
 * @brief Enumerates the different types of fading behaviors.
 */
enum FadingTypes
{
	NO_FADING = 0,        /**< No particle behavior. */
	FADING_BY_SIZE = 1,         /**< Particles that exist once and do not loop. */
	FADING_BY_COLOR = 2,        /**< Particles that have a lifetime and despawn after a set time. */
};


/**
 * @struct ParticleParam
 * @brief Contains the parameters to configure individual particles' behavior.
 */
struct ParticleParam
{
	ParticlesTypes type;              /**< Type of particle behavior (e.g., ONE_TIME, LIFE_TIME, ALWAYS). */
	FadingTypes fading_flags;
	sfVector2f position;              /**< Initial position of the particle. */
	sfVector2f origin;                /**< The origin point for the particle spawn (for rotation). */
	float radius;                     /**< Radius used for particle dispersion or area of effect. */
	float angle_direction;            /**< The initial direction angle of the particle. */
	float angle_spawn_spread;         /**< Spread of the particle spawn in terms of angle. */
	float speed;                      /**< Speed of the particle's movement. */
	float random_speed_boost;         /**< Random boost to the speed of the particle. */
	float rotation;                   /**< Rotation of the particle. */
	float random_spawn_rotation;      /**< Random rotation at the spawn of the particle. */
	float life_time;                  /**< Total lifetime of the particle. */
	float despawn_time;               /**< Time until the particle despawns. */
	float spawn_time;                 /**< Time for the particle to spawn. */
	float fading_start_time;
	sfColor color;                    /**< Color of the particle. */
	sfColor fading_color;
	int spawn_count;                  /**< Number of particles to spawn at once. */
};

/**
 * @struct Particles
 * @brief Represents a particle system containing multiple particles.
 */
struct Particles
{
	Particles_Data* _Data;  /**< Internal data for managing particles. */

	/**
	 * @brief Updates the particle system by advancing the particles based on the elapsed time.
	 * @param particles Pointer to the Particles object to update.
	 * @param deltaTime The time elapsed since the last update (in seconds).
	 */
	void (*Update)(Particles* particles, float deltaTime);

	/**
	 * @brief Checks if the particle system has finished its lifecycle.
	 * @param particles Pointer to the Particles object.
	 * @return sfTrue if the system has finished, sfFalse otherwise.
	 */
	sfBool(*HasFinish)(Particles* particles);

	void (*SetEmitterPosition)(Particles* particles, sfVector2f pos);

	/**
	 * @brief Destroys the particle system and frees allocated memory.
	 * @param particles Pointer to the pointer of the Particles object to be destroyed.
	 */
	void (*Destroy)(Particles** particles);
};

/**
 * @brief Creates default particle parameters for a given type and position.
 * @param type The type of particles to create (e.g., ONE_TIME, LIFE_TIME, etc.).
 * @param position The initial position of the particles.
 * @param direction The direction of the particles in degrees.
 * @param speed The speed of the particles.
 * @return A ParticleParam structure initialized with the given parameters.
 */
ParticleParam CreateDefaultParam(ParticlesTypes type, sfVector2f position, float direction, float speed);

/**
 * @brief Creates a vanilla particle system with default settings.
 * @param parameters The parameters used to configure the particles.
 * @param point_count The number of particles to create.
 * @return A pointer to a Particles object representing the created particle system.
 */
Particles* CreateVanillaParticles(ParticleParam parameters, int point_count);

/**
 * @brief Creates a particle system that uses a texture for rendering particles.
 * @param parameters The parameters used to configure the particles.
 * @param texture The texture used for rendering each particle.
 * @param texture_rect The section of the texture to use for each particle.
 * @return A pointer to a Particles object representing the created particle system.
 */
Particles* CreateTextureParticles(ParticleParam parameters, sfTexture* texture, sfIntRect texture_rect);


Particles* LoadParticlesFromFile(const char* path, sfVector2f position, sfTexture* texture);

void RestartParticles(Particles* particles);

/**
 * @brief Renders the particles to the window.
 * @param render_window Pointer to the SFML render window.
 * @param particles Pointer to the Particles object to render.
 * @param state Render states to apply while drawing the particles.
 */
void sfRenderWindow_drawParticles(sfRenderWindow* render_window, Particles* particles, sfRenderStates* state);

/**
 * @brief Renders the particles into a render texture.
 * @param render_texture Pointer to the SFML render texture.
 * @param particles Pointer to the Particles object to render.
 * @param state Render states to apply while drawing the particles.
 */
void sfRenderTexture_drawParticles(sfRenderTexture* render_texture, Particles* particles, sfRenderStates* state);
