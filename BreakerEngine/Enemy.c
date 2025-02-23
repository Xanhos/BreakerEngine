#include "Enemy.h"
#include "ResourcesManager.h"
#include "Player.h"
#include "Projectile.h"
#include "Particles.h"
#include "MemoryManagement.h"

stdList* enemies_list;
stdList* particles_list;
ParticleParam particle_parameters;
sfSprite* enemy_sprite;
float spawn_timer;



void CreateEnemy(sfVector2f pos, float health)
{
	Enemy enemy;
	enemy.transform.position = pos;
	enemy.transform.velocity = sfVector2f_Create(0, 0);
	enemy.transform.angle = 0.f;
	enemy.life = health;
	enemy.shooting_timer = 0.f;
	enemy.sprite_rect = (sfIntRect){ 0,3214 ,173,86 };
	enemy.speed = 400.f;
	enemy.destination_pos = GetRandomPosInRectangle((sfFloatRect) { 960, 0, 1700, 900 });
	enemy.hitbox = (sfFloatRect){ 0,0,(float)enemy.sprite_rect.width, (float)enemy.sprite_rect.height };
	enemies_list->push_back(enemies_list, &enemy);
}

void InitEnemies()
{
	enemies_list = stdList_Create(sizeof(Enemy), 0);
	particles_list = stdList_Create(sizeof(Particles*), 0);
	enemy_sprite = sfSprite_create();
	sfSprite_setTexture(enemy_sprite, GetTexture("ingamep1"), sfTrue);
	spawn_timer = 0.f;

	particle_parameters = CreateDefaultParam(ONE_TIME, sfVector2f_Create(0, 0), 0, 300.f);
	particle_parameters.spawn_count = 30;
	particle_parameters.color = CreateColor(255, 0, 0, 255);
	particle_parameters.angle_spawn_spread = 60.f;
	particle_parameters.despawn_time = .75f;
	particle_parameters.random_spawn_rotation = 360.f;
	particle_parameters.radius = 10.f;
	particle_parameters.random_speed_boost = 300.f;
	particle_parameters.fading_flags = FADING_BY_COLOR | FADING_BY_SIZE;
	particle_parameters.fading_color = CreateColor(255, 0, 255, 0);
	particle_parameters.fading_start_time = 0.f;
}


void UpdateEnemies(WindowManager* window)
{
	spawn_timer += DeltaTime;

	if (spawn_timer >= 2.5f)
	{
		CreateEnemy(sfVector2f_Create(2100.f, (float)rand_float(0.f, 960.f)), 50.f);
		spawn_timer = 0.f;
	}

	FOR_EACH(enemies_list, Enemy, i, enemy_it,
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

	enemy_it->transform.velocity = NormalizeVector2f(SubVector2f(enemy_it->destination_pos, enemy_it->transform.position));


	if (enemy_it->life <= 0)
	{
		PlayerScore += 50;
		enemies_list->erase(enemies_list, i);
		i--;
	}

	enemy_it->transform.position = AddVector2f(enemy_it->transform.position, MultiplyVector2f(enemy_it->transform.velocity, enemy_it->speed * DeltaTime));
	enemy_it->hitbox.left = enemy_it->transform.position.x;
	enemy_it->hitbox.top = enemy_it->transform.position.y;);



	FOR_EACH_POINTER(particles_list, Particles, i, it,
		it->Update(it, DeltaTime);
	if (it->HasFinish(it))
	{
		it->Destroy(&it);
		particles_list->erase(particles_list, i);
		i--;
	}
		);


}

void DisplayEnemies(WindowManager* window)
{
	FOR_EACH(enemies_list, Enemy, i, it,
		sfSprite_setTextureRect(enemy_sprite, it->sprite_rect);
	sfSprite_setPosition(enemy_sprite, it->transform.position);
	window->DrawSprite(window, enemy_sprite, NULL);
		)

		FOR_EACH_POINTER(particles_list, Particles, i, it,
			window->DrawParticles(window, it, NULL);
			)
}

void DestroyEnemies()
{
	enemies_list->destroy(&enemies_list);
	particles_list->destroy(&particles_list);
	sfSprite_destroy(enemy_sprite);
}

stdList* GetEnemyList()
{
	return enemies_list;
}

void EnemyTakeDamage(Enemy* enemy, float damage, sfVector2f hit_point)
{
	enemy->life -= damage;
	if (enemy->life <= 0)
	{
		particle_parameters.position = hit_point;
		Particles* death_particles = CreateVanillaParticles(particle_parameters, 3);
		particles_list->push_back(particles_list, &death_particles);
	}
}
