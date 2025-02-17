#include "Player.h"
#include "Projectile.h"

SimpleAnim* PlayerTrail;
Animation* Player;

void InitPlayer()
{
	PlayerTrail = CreateSimpleAnim(GetTexture("ingamep1"), (sfIntRect) { 0, 0, 95, 51 }, 1, 3, 3, .1);
	Player = CreateAnimation("Player", GetTexture("ingamep2"));
	Player->AddAnimationKey(Player, CreateAnimationKey("Idle", (sfIntRect) { 0, 5995, 228, 130 }, 1, 1, 1, 1));
	Player->AddAnimationKey(Player, CreateAnimationKey("Damage", (sfIntRect) { 0, 6125, 228, 130 }, 1, 3, 3, .5f / 6.f));
	Player->SelectAnimationKey(Player, "Idle");
}

void UpdatePlayer(WindowManager* window)
{
	Player->Update(Player, DeltaTime);

	sfVector2f velocity = sfVector2f_Create(0, 0);
	if (KEY(D))
	{
		velocity.x = 10;
	}
	else if (KEY(Q))
	{
		velocity.x = -10;
	}
	if (KEY(S))
	{
		velocity.y = 10;
	}
	else if (KEY(Z))
	{
		velocity.y = -10;
	}
	velocity = NormalizeVector2f(velocity);

	if (KEY_DOWN(Space))
	{
		CreateProjectile(sfRectangleShape_getPosition(Player->GetRenderer(Player)), 10, sfTrue);
	}


	Animation_Key* anim_key = Player->GetCurrentAnimationKey(Player);
	if (strcmp(anim_key->GetAnimationKeyName(anim_key), "Damage") == 0)
	{
		if (anim_key->HasFinishAnim(anim_key) && Player->IsRevert(Player))
		{
			Player->SelectAnimationKey(Player, "Idle");
			Player->SetAnimationParameters(Player, sfFalse, sfFalse, sfFalse);
		}
		else if (anim_key->HasFinishAnim(anim_key))
		{
			Player->SetAnimationParameters(Player, sfFalse, sfTrue, sfFalse);
			anim_key->SetCurrentFrame(anim_key, anim_key->GetCurrentFrame(anim_key) - 1);
		}

	}

	sfRectangleShape_setPosition(Player->GetRenderer(Player), AddVector2f(sfRectangleShape_getPosition(Player->GetRenderer(Player)), MultiplyVector2f(velocity, DeltaTime * 400.f)));
}

void DisplayPlayer(WindowManager* window)
{
	window->DrawAnimation(window, Player, NULL);
}

void DestroyPlayer()
{
	PlayerTrail->Destroy(&PlayerTrail);
	Player->Destroy(&Player);
}
