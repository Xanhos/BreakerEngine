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
#include "ResourcesManager.h"

float __TextureProgressBar, __MovieProgressBar, __SoundProgressBar,__MusicProgressBar ,__FontProgressBar;

void InitResourcesManager(const char* resource_directory_)
{
	strcpy_s(resource_directory, MAX_PATH_SIZE, resource_directory_);
	InitTextureManager();
	InitFontManager();
	InitSoundManager();
	InitMovieManager();
}

void LoadScene(const char* scene_name)
{
	printf_d("--------------------Starting loading the %s scene--------------------\n\n", scene_name);
	LoadSceneTexture(scene_name,&__TextureProgressBar);
	LoadSceneFont(scene_name, &__FontProgressBar);
	LoadSceneSound(scene_name, &__SoundProgressBar, &__MusicProgressBar);
	LoadSceneMovie(scene_name, &__MovieProgressBar);
	printf_d("--------------------Finish loading the %s scene--------------------\n\n", scene_name);
}

void DestroyResourcesManager(void)
{
	DestroyTexturesManager();
	DestroyFontsManager();
	DestroySoundsManager();
	DestroyMoviesManager();
}

float GetLoadingValue()
{
	return (__TextureProgressBar + __FontProgressBar + __MusicProgressBar + __SoundProgressBar + __MovieProgressBar) / 5.f;
}
