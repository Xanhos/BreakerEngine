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
#include "FontManager.h"

stdList* global_font_list, * scene_font_list;
Font font_place_holder;

Font CreateFont(const char* path)
{
	Font tmp;
	Path tmpPath = fs_create_path(path);
	tmp.m_font = sfFont_createFromFile(path);
	tmp.m_path = tmpPath;
	strcpy_s(tmp.m_name, MAX_PATH_SIZE, tmpPath.stem(&tmpPath).path_data.m_path);
	ToLower(tmp.m_name);
	printf_d("Font {\n\tPath : %s\n\tName: %s\n } loaded\n\n", tmp.m_path.path_data.m_path, tmp.m_name);

	return tmp;
}

void DeleteFont(Font* font)
{
	sfFont_destroy(font->m_font);
}

void InitFontManager(void)
{
	NEW_CHAR(resources_path, MAX_PATH_SIZE)
		strcpy_s(resources_path, MAX_PATH_SIZE, resource_directory);
	strcat_s(resources_path, MAX_PATH_SIZE, "/ALL/Fonts");
	Path fs_path = fs_create_path(resources_path);
	if (fs_path.exist(&fs_path))
	{
		if (global_font_list == NULL)
		{
			printf_d("Start Global Font loading\n\n");

			global_font_list = stdList_Create(sizeof(Font), 0);
			scene_font_list = stdList_Create(sizeof(Font), 0);
			
			FOR_EACH_TEMP_LIST(filesInfos, FilesInfo, SearchFilesInfos(fs_path.path_data.m_path, "ttf"),
				Font tmp = CreateFont(STD_GETDATA(filesInfos, FilesInfo, i)->m_path);
			if (strcmp(tmp.m_name, "placeholder") == 0)
				font_place_holder = tmp;
			else
				global_font_list->push_back(global_font_list, &tmp);
				)
		}
	}
	else
	{
		printf_d("No font directory found, create a ALL/Fonts folder in your resources directory\n\n");
		exit(0);
	}
}

void Load_Font(const char* path)
{
	Font tmp = CreateFont(path);
	scene_font_list->push_back(scene_font_list, &tmp);
}

void LoadSceneFont(const char* scene, float* progressValue)
{
	ClearSceneFont();
	__LoadScene(scene, "ttf", "Fonts",progressValue, &Load_Font);
}

void ClearSceneFont(void)
{
	if (scene_font_list != NULL)
	{
		for (int i = 0; i < scene_font_list->size(scene_font_list); i++)
			sfFont_destroy(STD_GETDATA(scene_font_list, Font, i)->m_font);
		scene_font_list->clear(scene_font_list);
	}
}

sfFont* GetFont(const char* name)
{
	if (global_font_list != NULL)
		FOR_EACH_LIST(global_font_list, Font, it, tmp,
			if (strcmp(tmp->m_name, name) == 0)
				return tmp->m_font;
				)

		if (scene_font_list != NULL)
			FOR_EACH_LIST(scene_font_list, Font, it, tmp,
				if (strcmp(tmp->m_name, name) == 0)
					return tmp->m_font;
					)

			if (font_place_holder.m_font)
			{
				printf_d("Font %s not found, placeholder returned\n\n", name);
				return font_place_holder.m_font;
			}

	printf_d("No Font placeholder found, put a placeholder.ttf in your %s/ALL/Fonts folder\n\n", resource_directory);
	return NULL;
}

void DestroyFontsManager(void)
{
	ClearSceneFont();
	if (global_font_list != NULL)
	{
		for (int i = 0; i < global_font_list->size(global_font_list); i++)
			sfFont_destroy(STD_GETDATA(global_font_list, Font, i)->m_font);
		global_font_list->clear(global_font_list);
	}
	scene_font_list->destroy(&scene_font_list);
	global_font_list->destroy(&global_font_list);

}
