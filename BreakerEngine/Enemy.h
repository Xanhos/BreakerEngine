#pragma once
#include "Tools.h"
#include "WindowManager.h"

typedef enum {
	SHOOTER,
	KAMIKAZE
} Enemy_Type;

typedef struct {
	Transform transform;
	sfFloatRect hitbox;
	sfIntRect sprite_rect;
	float life;
	float speed;
	float shooting_timer;
	sfVector2f destination_pos;
	Enemy_Type type;
} Enemy;

void CreateEnemy(sfVector2f pos, Enemy_Type type, float health);
void InitEnemies();
void UpdateEnemies(WindowManager* window);
void DisplayEnemies(WindowManager* window);
void DestroyEnemies();

stdList* GetEnemyList();

void EnemyTakeDamage(Enemy* enemy, float damage);