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
#include "Particles.h"

typedef struct Particle Particle;
struct Particle
{
	sfVector2f m_position;
	sfVector2f m_direction;
	float m_rotation;
	float m_despawn_timer;
	float m_despawn_time;
};


struct Particles_Data
{
	stdList* m_particle_list;

	sfTexture* m_texture;

	sfCircleShape* m_vanilla_rendeder;
	sfRectangleShape* m_texture_renderer;

	sfVector2f m_position;
	float m_angle_direction;
	float m_speed;
	float m_rotation;

	float m_spawn_time;
	float m_spawn_timer;

	float m_despawn_time;

	float m_life_time;
	float m_life_timer;

	ParticlesTypes m_type;
	int m_spawn_count;

	float m_angle_spawn_spread;

};

static Particle* CreateParticle(Particles* particles)
{
	Particle* particle = calloc(1, sizeof(Particle));
	assert(particle);

	particle->m_position = particles->_Data->m_position;
	particle->m_despawn_time = particles->_Data->m_despawn_time;
	particle->m_despawn_timer = 0.f;
	particle->m_direction = GetVectorFromAngle(sfVector2f_Create(0, 0), 1, particles->_Data->m_angle_direction + rand_float(-particles->_Data->m_angle_spawn_spread, particles->_Data->m_angle_spawn_spread));

	return particle;
}


static sfBool UpdateParticle(Particles* particles, Particle* particle, float delta_time)
{
	particle->m_despawn_timer += delta_time;
	if (particle->m_despawn_timer > particle->m_despawn_time)
	{
		return sfTrue;
	}

	particle->m_position = AddVector2f(particle->m_position, MultiplyVector2f(particle->m_direction, particles->_Data->m_speed * delta_time));
	particle->m_rotation += particles->_Data->m_rotation * delta_time;
	return sfFalse;
}

static Particles* CreateParticles(ParticlesTypes type, sfVector2f position, sfVector2f origin, float radius, float angle_direction, float angle_spawn_spread, float speed, float rotation, int point_count, sfColor color, float despawn_time, float spawn_time, int spawn_count, float life_time)
{
	Particles* particles = calloc(1, sizeof(Particles));
	assert(particles);
	Particles_Data* data = calloc(1, sizeof(Particles_Data));
	assert(data);

	data->m_particle_list = STD_LIST_CREATE_POINTER(Particle*, 0);
	data->m_texture = NULL;
	data->m_texture_renderer = NULL;
	data->m_vanilla_rendeder = NULL;
	data->m_position = position;
	data->m_angle_direction = angle_direction;
	data->m_speed = speed;
	data->m_rotation = rotation;
	data->m_spawn_time = spawn_time;
	data->m_spawn_timer = 0;
	data->m_despawn_time = despawn_time;
	data->m_spawn_count = spawn_count;
	data->m_angle_spawn_spread = abs(angle_spawn_spread) / 2;
	data->m_life_time = life_time;
	data->m_life_timer = 0.f;
	data->m_type = type;


	particles->_Data = data;
	return particles;
}



ParticleParam CreateDefaultParam(ParticlesTypes type, sfVector2f position, float direction, float speed)
{
	return (ParticleParam) {
		.type = type,
			.position = position,
			.origin = sfVector2f_Create(0, 0),
			.radius = 25.f,
			.angle_direction = direction,
			.angle_spawn_spread = 0.f,
			.speed = speed,
			.rotation = 0.f,
			.color = CreateColor(255, 255, 255, 255),
			.despawn_time = 5.f,
			.spawn_time = 0.2f,
			.spawn_count = 1.f,
			.life_time = 10.f
	};
}

Particles* CreateVanillaParticles(ParticleParam parameters, int point_count)
{
	Particles* particles = CreateParticles(parameters.type, parameters.position, parameters.origin, parameters.radius, parameters.angle_direction, parameters.angle_spawn_spread, parameters.speed, parameters.rotation, point_count, parameters.color, parameters.despawn_time, parameters.spawn_time, parameters.spawn_count, parameters.life_time);


	sfCircleShape* renderer = sfCircleShape_create();
	sfCircleShape_setPosition(renderer, parameters.position);
	sfCircleShape_setOrigin(renderer, parameters.origin);
	sfCircleShape_setRadius(renderer, parameters.radius);
	sfCircleShape_setFillColor(renderer, parameters.color);
	sfCircleShape_setPointCount(renderer, point_count);

	particles->_Data->m_vanilla_rendeder = renderer;
	return particles;
}



Particles* CreateTextureParticles(ParticleParam parameters, sfTexture* texture, sfIntRect texture_rect)
{
	Particles* particles = CreateParticles(parameters.type, parameters.position, parameters.origin, parameters.radius, parameters.angle_direction, parameters.angle_spawn_spread, parameters.speed, parameters.rotation, 0, parameters.color, parameters.despawn_time, parameters.spawn_time, parameters.spawn_count, parameters.life_time);

	if (texture_rect.width == 0 && texture_rect.height == 0)
	{
		texture_rect.width = sfTexture_getSize(texture).x;
		texture_rect.height = sfTexture_getSize(texture).y;
	}
	particles->_Data->m_texture = texture;
	sfRectangleShape* renderer = sfRectangleShape_create();
	sfRectangleShape_setSize(renderer, sfVector2f_Create(texture_rect.width, texture_rect.height));
	sfRectangleShape_setOrigin(renderer, parameters.origin);
	sfRectangleShape_setFillColor(renderer, parameters.color);
	sfRectangleShape_setTexture(renderer, texture, sfFalse);
	sfRectangleShape_setTextureRect(renderer, texture_rect);

	particles->_Data->m_texture_renderer = renderer;

	return particles;
}

void UpdateParticles(Particles* particles, float deltaTime)
{
	if (deltaTime == 0.f)
		deltaTime = DeltaTime;

	Particles_Data* data = particles->_Data;

	data->m_life_timer += deltaTime;
	data->m_spawn_timer += deltaTime;
	if ((data->m_spawn_timer > data->m_spawn_time && (data->m_type == ALWAYS || data->m_type == LIFE_TIME && data->m_life_timer < data->m_life_time)) || data->m_type == ONE_TIME)
	{
		if (data->m_type == ONE_TIME)
			data->m_type = NONE;

		for (int i = 0; i < data->m_spawn_count; i++)
		{
			Particle* tmp = CreateParticle(particles);
			data->m_particle_list->push_back(&data->m_particle_list, &tmp);
		}
		data->m_spawn_timer = 0;
	}

	FOR_EACH_LIST_POINTER(data->m_particle_list, Particle*, i, it,
		if (UpdateParticle(particles, it, deltaTime))
		{
			free(it);
			data->m_particle_list->erase(&data->m_particle_list, i);
			i--;
		}
			);
}

void sfRenderWindow_drawParticles(sfRenderWindow* render_window, Particles* particles, sfRenderStates* state)
{
	Particles_Data* data = particles->_Data;
	if (data->m_vanilla_rendeder)
	{
		FOR_EACH_LIST_POINTER(data->m_particle_list, Particle*, i, it,
			{
				sfCircleShape_setPosition(data->m_vanilla_rendeder, it->m_position);
				sfCircleShape_setRotation(data->m_vanilla_rendeder, it->m_rotation);
				sfRenderWindow_drawCircleShape(render_window, data->m_vanilla_rendeder, state);
			}
		);
	}
	else if (data->m_texture_renderer && data->m_texture)
	{
		FOR_EACH_LIST_POINTER(data->m_particle_list, Particle*, i, it,
			{
				sfRectangleShape_setPosition(data->m_texture_renderer, it->m_position);
				sfRectangleShape_setRotation(data->m_texture_renderer, it->m_rotation);
				sfRenderWindow_drawRectangleShape(render_window, data->m_texture_renderer, state);
			}
		);
	}

}