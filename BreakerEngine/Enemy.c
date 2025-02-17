#include "Enemy.h"
#include "Pool.h"

stdPool* enemies_pool;

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
}
