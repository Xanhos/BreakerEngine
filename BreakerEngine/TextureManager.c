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
#include "TextureManager.h"

stdList* global_texture_list, * scene_texture_list;
Texture texture_place_holder;


Texture CreateTexture(const char* path)
{
	Texture tmp;
	Path tmpPath = fs_create_path(path);
	tmp.m_texture = sfTexture_createFromFile(path, NULL);
	sfVector2f size = { (float)sfTexture_getSize(tmp.m_texture).x, (float)sfTexture_getSize(tmp.m_texture).y };
	tmp.m_path = tmpPath;
	strcpy_s(tmp.m_name, MAX_PATH_SIZE, tmpPath.stem(&tmpPath).path_data.m_path);
	ToLower(tmp.m_name);
	printf_d("Texture {\n\tPath : %s\n\tName: %s\n } loaded\n\n", tmp.m_path.path_data.m_path, tmp.m_name);
	return tmp;
}

void InitTextureManager(void)
{
	NEW_CHAR(resources_path, MAX_PATH_SIZE)
		strcpy_s(resources_path, MAX_PATH_SIZE, resource_directory);
	strcat_s(resources_path, MAX_PATH_SIZE, "/ALL/Textures");
	Path fs_path = fs_create_path(resources_path);
	if (fs_path.exist(&fs_path))
	{
		if (global_texture_list == NULL)
		{
			printf_d("Start Global texture loading\n\n");

			global_texture_list = stdList_Create(sizeof(Texture), 0);
			scene_texture_list = stdList_Create(sizeof(Texture), 0);
			stdList* filesInfos = SearchFilesInfos(fs_path.path_data.m_path, "png"); 
			for (int i = 0; i < filesInfos->size(filesInfos); i++) 
			{
				Texture tmp = CreateTexture(((FilesInfo*)filesInfos->getData(filesInfos, i))->m_path);
				if (strcmp(tmp.m_name, "placeholder") == 0) 
					texture_place_holder = tmp;
				else global_texture_list->push_back(global_texture_list, &tmp);
			} 
			filesInfos->destroy(&filesInfos);
		}
	}
	else
	{
		printf_d("No texture directory found, create a ALL/Texture folder in your resources directory\n\n");
		exit(0);
	}
}


void Load_Texture(const char* path)
{
	Texture tmp = CreateTexture(path);
	scene_texture_list->push_back(scene_texture_list, &tmp);
}

void LoadSceneTexture(const char* scene, float* progressValue)
{
	ClearSceneTexture();
	__LoadScene(scene, "png", "Textures",progressValue, &Load_Texture);
}



void ClearSceneTexture(void)
{
	if (scene_texture_list != NULL)
	{
		for (int i = 0; i < scene_texture_list->size(scene_texture_list); i++)
			sfTexture_destroy(STD_GETDATA(scene_texture_list, Texture, i)->m_texture);
		scene_texture_list->clear(scene_texture_list);
	}
}

sfTexture* GetTexture(const char* name)
{
	if (global_texture_list != NULL)
		FOR_EACH_LIST(global_texture_list, Texture, it, tmp,
			if (strcmp(tmp->m_name, name) == 0)
				return tmp->m_texture;
				)

		if (scene_texture_list != NULL)
			FOR_EACH_LIST(scene_texture_list, Texture, it, tmp,
				if (strcmp(tmp->m_name, name) == 0)
					return tmp->m_texture;
					)

			if (texture_place_holder.m_texture)
			{
				printf_d("Texture %s not found, placeholder returned", name);
				return texture_place_holder.m_texture;
			}

	printf_d("No texture placeholder found, put a placeholder.png in your %s/ALL/Textures folder", resource_directory);
	return NULL;
}

void DestroyTexturesManager(void)
{
	ClearSceneTexture();
	if (global_texture_list != NULL)
	{
		for (int i = 0; i < global_texture_list->size(global_texture_list); i++)
			sfTexture_destroy(STD_GETDATA(global_texture_list, Texture, i)->m_texture);
		global_texture_list->clear(global_texture_list);
	}
	scene_texture_list->destroy(&scene_texture_list);
	assert(global_texture_list);
	global_texture_list->destroy(&global_texture_list);
}

void DeleteTexture(Texture* texture)
{
	sfTexture_destroy(texture->m_texture);
}
