/*
    Author: GRALLAN Yann

    Description: An advanced game engine for CSFML

    Date: 2025/01/22

    MIT License

    Copyright (c) 2025 GRALLAN Yann


    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once
#include "Tools.h"
#include "WindowManager.h"

typedef enum {
	SHOOTER,
	KAMIKAZE
} Enemy_Type;

typedef struct {
	Transform transform;
	sfFloatRect hitbox;
	sfIntRect sprite_rect;
	float life;
	float speed;
	float shooting_timer;
	sfVector2f destination_pos;
} Enemy;

void CreateEnemy(sfVector2f pos, float health);
void InitEnemies();
void UpdateEnemies(WindowManager* window);
void DisplayEnemies(WindowManager* window);
void DestroyEnemies();

stdList* GetEnemyList();

void EnemyTakeDamage(Enemy* enemy, float damage, sfVector2f hit_point);