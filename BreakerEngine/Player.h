#pragma once
#include "Tools.h"
#include "SpriteManager.h"
#include "WindowManager.h"
#include "Animation.h"

sfVector2f PlayerPos;

void InitPlayer();
void UpdatePlayer(WindowManager* window);
void DisplayPlayer(WindowManager* window);
void DestroyPlayer();

void PlayerTakeDamage(int damage);

sfFloatRect GetPlayerHitbox();