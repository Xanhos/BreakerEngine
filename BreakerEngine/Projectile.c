#include "Projectile.h"
#include "MemoryManagement.h"
#include "ResourcesManager.h"
#include "Enemy.h"
#include "Pool.h"

sfSprite* projectile_sprite;
stdList* projectiles_list;

void CreateProjectile(sfVector2f position, int damage, sfBool shotByPlayer)
{
    Projectile* projectile = calloc_d(Projectile, 1);
    projectile->transform.position = position;
    projectile->transform.velocity = sfVector2f_Create(shotByPlayer ? 1 : -1, 0);
    projectile->damage = damage;
    projectile->hitbox = (sfFloatRect){ 0,0,26,19 };
    projectile->isShotByPlayer = shotByPlayer;
    
    projectiles_list->push_back(projectiles_list, &projectile);
}

void InitProjectiles()
{
    projectile_sprite = sfSprite_create();
    sfSprite_setTexture(projectile_sprite, GetTexture("ingamep1"), sfFalse);
    sfSprite_setTextureRect(projectile_sprite, (sfIntRect) { 0, 1321, 26, 19 });
    projectiles_list = stdList_Create(sizeof(Projectile*), 0);
}

void UpdateProjectiles(WindowManager* window)
{    

    for (int i = 0; i < projectiles_list->size(projectiles_list); i++) {
        Projectile** it_ = ((Projectile**)projectiles_list->getData(projectiles_list, i)); 
        Projectile* it = *it_;
        it->transform.position = AddVector2f(it->transform.position, MultiplyVector2f(it->transform.velocity, 500.f * DeltaTime)); 
        it->hitbox.left = it->transform.position.x;
        it->hitbox.top = it->transform.position.y; if (it->isShotByPlayer) {
        }
        if (!PointInRectangle(it->transform.position, (sfFloatRect) { 0, 0, window->GetBaseSize(window).x, window->GetBaseSize(window).y}, 0)) 
        {
            free(it);
            projectiles_list->erase(projectiles_list, i);
            i--;
        }
    }


}


void DisplayProjectiles(WindowManager* window)
{
    FOR_EACH_POINTER(projectiles_list, Projectile, i, it,
        sfSprite_setPosition(projectile_sprite, it->transform.position);
    window->DrawSprite(window, projectile_sprite, NULL);

        )
}


void DestroyProjectiles()
{
    for (int i = 0; i < projectiles_list->size(projectiles_list); i++) {
        Projectile** it_ = ((Projectile**)projectiles_list->getData(projectiles_list, i));
        Projectile* it = *it_;
        {
            DetrackerCalloc(it);
            projectiles_list->erase(projectiles_list, i);
            i--;
        }
    }
    projectiles_list->destroy(&projectiles_list);
    sfSprite_destroy(projectile_sprite);
}