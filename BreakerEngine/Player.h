#pragma once
#include "Tools.h"
#include "SpriteManager.h"
#include "WindowManager.h"
#include "Animation.h"

void InitPlayer();
void UpdatePlayer(WindowManager* window);
void DisplayPlayer(WindowManager* window);
void DestroyPlayer();