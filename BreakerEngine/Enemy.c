#include "Enemy.h"
#include "ResourcesManager.h"
#include "Player.h"
#include "Projectile.h"

stdList* enemies_list;
sfSprite* enemy_sprite;
float spawn_timer;


void CreateEnemy(sfVector2f pos, Enemy_Type type, float health)
{
	Enemy enemy;
	enemy.transform.position = pos;
	enemy.transform.velocity = sfVector2f_Create(0, 0);
	enemy.transform.angle = 0.f;
	enemy.type = type;
	enemy.life = health;
	enemy.shooting_timer = 0.f;
	switch (type)	
	{
	case KAMIKAZE:
		enemy.sprite_rect = (sfIntRect){ 0,3386 ,119,99 };
		enemy.speed = 700.f;
		break;
	case SHOOTER:
		enemy.sprite_rect = (sfIntRect){ 0,3214 ,173,86 };
		enemy.speed = 400.f;
		enemy.destination_pos = GetRandomPosInRectangle((sfFloatRect) { 960, 0, 1700, 900 });
		break;
	default:
		break;
	}
	enemy.hitbox = (sfFloatRect){ 0,0,enemy.sprite_rect.width,enemy.sprite_rect.height };
	enemies_list->push_back(enemies_list, &enemy);
}

void InitEnemies()
{
	enemies_list = stdList_Create(sizeof(Enemy), 0);
	enemy_sprite = sfSprite_create();
	sfSprite_setTexture(enemy_sprite, GetTexture("ingamep1"), sfTrue);
	spawn_timer = 0.f;
}

void UpdateEnemies(WindowManager* window)
{
	spawn_timer += DeltaTime;

	if (spawn_timer >= 2.5f)
	{
		CreateEnemy(sfVector2f_Create(2100.f, rand_float(0.f, 960.f)), rand_int(0, 1), 50.f);
		spawn_timer = 0.f;
	}

	FOR_EACH(enemies_list, Enemy, i, enemy_it,
		enemy_it->shooting_timer += DeltaTime;
		if (enemy_it->type == KAMIKAZE)
		{
			enemy_it->transform.velocity = NormalizeVector2f(SubVector2f(PlayerPos, enemy_it->transform.position));
		}
		else if (enemy_it->type == SHOOTER)
		{
			enemy_it->shooting_timer += DeltaTime;
			if (GetDistance(enemy_it->transform.position, enemy_it->destination_pos) < 5.f)
			{
				enemy_it->destination_pos = GetRandomPosInRectangle((sfFloatRect) { 960, 0, 1700, 900 });
			}
			if (enemy_it->shooting_timer > 1.f)
			{
				enemy_it->shooting_timer = 0.f;
				CreateProjectile(enemy_it->transform.position, 5, sfFalse);
			}

			enemy_it->transform.velocity = NormalizeVector2f(SubVector2f(enemy_it->destination_pos,enemy_it->transform.position));
		}
		
	enemy_it->transform.position = AddVector2f(enemy_it->transform.position, MultiplyVector2f(enemy_it->transform.velocity, enemy_it->speed * DeltaTime));
	enemy_it->hitbox.left = enemy_it->transform.position.x;
	enemy_it->hitbox.top = enemy_it->transform.position.y;

	)
	
}

void DisplayEnemies(WindowManager* window)
{
	FOR_EACH(enemies_list, Enemy, i, it,
		sfSprite_setTextureRect(enemy_sprite, it->sprite_rect);
		sfSprite_setPosition(enemy_sprite, it->transform.position);
		window->DrawSprite(window, enemy_sprite, NULL);
		)
}

void DestroyEnemies()
{
	enemies_list->destroy(&enemies_list);
	sfSprite_destroy(enemy_sprite);
}

stdList* GetEnemyList()
{
	return enemies_list;
}

void EnemyTakeDamage(Enemy* enemy, float damage)
{
	enemy->life -= damage;
}
