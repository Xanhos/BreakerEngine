#include "Projectile.h"
#include "MemoryManagement.h"
#include "ResourcesManager.h"
#include "Enemy.h"
#include "Player.h"
#include "Pool.h"

sfSprite* projectile_sprite;
stdList* projectiles_list;

void CreateProjectile(sfVector2f position, int damage, sfBool shotByPlayer)
{
	Projectile projectile;
	projectile.transform.position = position;
	projectile.transform.velocity = sfVector2f_Create(shotByPlayer ? 1.f : -1.f, 0.f);
	projectile.damage = damage;
	projectile.isShotByPlayer = shotByPlayer;
	projectile.texture_rect = shotByPlayer ? (sfIntRect) { 0, 1321, 26, 19 } : (sfIntRect) { 0, 2011, 33, 31 };
	projectile.hitbox = (sfFloatRect){ 0, 0, (float)projectile.texture_rect.width, (float)projectile.texture_rect.height };

	projectiles_list->push_back(projectiles_list, &projectile);
}

void InitProjectiles()
{
	projectile_sprite = sfSprite_create();
	sfSprite_setTexture(projectile_sprite, GetTexture("ingamep1"), sfFalse);
	projectiles_list = stdList_Create(sizeof(Projectile), 0);
}

void UpdateProjectiles(WindowManager* window)
{

	for (int i = 0; i < projectiles_list->size(projectiles_list); i++)
	{
		Projectile* it = ((Projectile*)projectiles_list->getData(projectiles_list, i));
		it->transform.position = AddVector2f(it->transform.position, MultiplyVector2f(it->transform.velocity, 1000.f * DeltaTime));
		it->hitbox.left = it->transform.position.x;
		it->hitbox.top = it->transform.position.y;
		if (!PointInRectangle(it->transform.position, (sfFloatRect) { 0, 0, (float)window->GetBaseSize(window).x, (float)window->GetBaseSize(window).y }, 0))
		{
			projectiles_list->erase(projectiles_list, i);
			i--;
		}

		if (!it->isShotByPlayer && Rectangle_Collision(it->hitbox, GetPlayerHitbox(), sfFalse))
		{
			PlayerTakeDamage(1);
			projectiles_list->erase(projectiles_list, i);
			i--;
		}
		else if(it->isShotByPlayer)
		{
			FOR_EACH(GetEnemyList(),Enemy,j,enemy,
				if (Rectangle_Collision(it->hitbox, enemy->hitbox, sfFalse))
				{
					EnemyTakeDamage(enemy, (float)it->damage, AddVector2f(it->transform.position, sfVector2f_Create((float)it->hitbox.width, (float)it->hitbox.height * .5f)));
					projectiles_list->erase(projectiles_list, i);
					i--;
				})
		}

	}


}


void DisplayProjectiles(WindowManager* window)
{
	FOR_EACH(projectiles_list, Projectile, i, it,
		sfSprite_setTextureRect(projectile_sprite, it->texture_rect);
		sfSprite_setPosition(projectile_sprite, it->transform.position);
	window->DrawSprite(window, projectile_sprite, NULL);

		)
}


void DestroyProjectiles()
{
	for (int i = 0; i < projectiles_list->size(projectiles_list); i++) {
		Projectile* it = ((Projectile*)projectiles_list->getData(projectiles_list, i));
		{
			projectiles_list->erase(projectiles_list, i);
			i--;
		}
	}
	projectiles_list->destroy(&projectiles_list);
	sfSprite_destroy(projectile_sprite);
}