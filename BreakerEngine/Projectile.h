#pragma once
#include "Tools.h"
#include "WindowManager.h"

typedef struct
{
	Transform transform;
	int damage;
	sfFloatRect hitbox;
	sfIntRect texture_rect;
	sfBool isShotByPlayer;
} Projectile;

void CreateProjectile(sfVector2f position, int damage, sfBool shotByPlayer);

void InitProjectiles();

void UpdateProjectiles(WindowManager* window);

void DisplayProjectiles(WindowManager* window);

void DestroyProjectiles();