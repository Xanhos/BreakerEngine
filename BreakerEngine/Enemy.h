#pragma once
#include "Tools.h"
#include "WindowManager.h"

typedef struct {
	Transform transform;
	float life;
}Enemy;

void CreateEnemy(sfVector2f pos, float health);
void InitEnemies();
void UpdateEnemies();
void RenderEnemies(WindowManager* window);
void DestroyEnemies();