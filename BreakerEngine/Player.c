#include "Player.h"
#include "Projectile.h"

SimpleAnim* PlayerTrail;
Animation* Player;
sfText* PlayerScoreText;



void InitPlayer()
{
	PlayerTrail = CreateSimpleAnim(GetTexture("ingamep1"), (sfIntRect) { 0, 0, 95, 51 }, 1, 3, 3, .1f);
	Player = CreateAnimation("Player", GetTexture("ingamep2"));
	Player->AddAnimationKey(Player, CreateAnimationKey("Idle", (sfIntRect) { 0, 5995, 228, 130 }, 1, 1, 1, 1));
	Player->AddAnimationKey(Player, CreateAnimationKey("Damage", (sfIntRect) { 0, 6125, 228, 130 }, 1, 3, 3, .5f / 6.f));
	Player->SelectAnimationKey(Player, "Damage");
	PlayerScoreText = sfText_create();
	sfText_setFont(PlayerScoreText, GetFont("placeholder"));
	sfText_setPosition(PlayerScoreText, sfVector2f_Create(50, 50));
	PlayerScore = 0;
	
}

void UpdatePlayer(WindowManager* window)
{
	Player->Update(Player, DeltaTime);

	sfVector2f velocity = sfVector2f_Create(0, 0);
	sfVector2f pos = sfRectangleShape_getPosition(Player->GetRenderer(Player));

	if (KEY(D) && pos.x < window->GetBaseSize(window).x - sfRectangleShape_getSize(Player->GetRenderer(Player)).x)
	{
		velocity.x = 10;
	}
	else if (KEY(Q) && pos.x > 0)
	{
		velocity.x = -10;
	}
	if (KEY(Z) && pos.y > 0)
	{
		velocity.y = -10;
	}
	else if (KEY(S) && pos.y < window->GetBaseSize(window).y - sfRectangleShape_getSize(Player->GetRenderer(Player)).y)
	{
		velocity.y = 10;
	}
	velocity = NormalizeVector2f(velocity);

	if (KEY_DOWN(Space))
	{
		CreateProjectile(AddVector2f(pos, sfVector2f_Create(120, 85)), 100, sfTrue);
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
	PlayerPos = AddVector2f(sfRectangleShape_getPosition(Player->GetRenderer(Player)), MultiplyVector2f(velocity, DeltaTime * 400.f));
	sfRectangleShape_setPosition(Player->GetRenderer(Player), PlayerPos);

	stdString* score = stdStringCreate("Score : ");
	score->append(score, IntToString(PlayerScore));
	sfText_setString(PlayerScoreText, score->getData(score));
	score->destroy(&score);
}

void DisplayPlayer(WindowManager* window)
{
	window->DrawAnimation(window, Player, NULL);
}

void DisplayPlayerUI(WindowManager* window)
{
	window->DrawText(window, PlayerScoreText, NULL);
}

void DestroyPlayer()
{
	PlayerTrail->Destroy(&PlayerTrail);
	Player->Destroy(&Player);
}

void PlayerTakeDamage(int damage)
{
	Animation_Key* anim_key = Player->SelectAnimationKey(Player, "Damage");
	anim_key->SetCurrentFrame(anim_key, 0);
}

sfFloatRect GetPlayerHitbox()
{
	sfVector2f pos = sfRectangleShape_getPosition(Player->GetRenderer(Player));
	sfVector2f size = {
		.x = (float)Player->GetCurrentAnimationKey(Player)->GetCurrentRect(Player->GetCurrentAnimationKey(Player)).width,
		.y = (float)Player->GetCurrentAnimationKey(Player)->GetCurrentRect(Player->GetCurrentAnimationKey(Player)).height
	};


	return (sfFloatRect) { pos.x, pos.y, size.x, size.y };
}
