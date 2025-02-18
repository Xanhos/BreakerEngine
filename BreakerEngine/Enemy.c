#include "Enemy.h"

stdList* enemies_pool;

void CreateEnemy(sfVector2f pos, float health)
{

}

void InitEnemies()
{
	enemies_pool = stdPool_Create(sizeof(Enemy*), 0);
}

void UpdateEnemies()
{

}

void RenderEnemies(WindowManager* window)
{
}

void DestroyEnemies()
{
	enemies_pool->destroy(&enemies_pool);
}

stdList* GetEnemyList()
{
	return enemies_pool;
}
