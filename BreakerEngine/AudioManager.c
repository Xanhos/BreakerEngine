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
#include "AudioManager.h"


stdList* global_sound_list, * scene_sound_list, * global_music_list, * scene_music_list;
Sound sound_place_holder;
Music music_place_holder;

Sound CreateSound(const char* path)
{
	Sound sound;
	Path tmpPath = fs_create_path(path);
	sound.m_sound = KSound_createSoundFromFile(path);
	sound.m_path = tmpPath;
	strcpy_s(sound.m_name, MAX_PATH_SIZE, tmpPath.stem(&tmpPath).path_data.m_path);
	ToLower(sound.m_name);
	printf_d("Sound {\n\tPath : %s\n\tName: %s\n } loaded\n\n", sound.m_path.path_data.m_path, sound.m_name);
	return sound;
}

Music CreateMusic(const char* path)
{
	Music music;
	Path tmpPath = fs_create_path(path);
	music.m_music = KSound_createSoundFromFile(path);
	music.m_path = tmpPath;
	strcpy_s(music.m_name, MAX_PATH_SIZE, tmpPath.stem(&tmpPath).path_data.m_path);
	ToLower(music.m_name);
	printf_d("Music {\n\tPath : %s\n\tName: %s\n } loaded\n\n", music.m_path.path_data.m_path, music.m_name);
	return music;
}

void DeleteSound(Sound* sound)
{
	KSound_destroy(sound->m_sound);
}

void DeleteMusic(Music* music)
{
	KSound_destroy(music->m_music);
}

void InitSoundManager(void)
{
	{
		NEW_CHAR(resources_path, MAX_PATH_SIZE)
			strcpy_s(resources_path, MAX_PATH_SIZE, resource_directory);
		strcat_s(resources_path, MAX_PATH_SIZE, "/ALL/Sounds");
		Path fs_path = fs_create_path(resources_path);
		if (fs_path.exist(&fs_path))
		{
			if (global_sound_list == NULL)
			{
				printf_d("Start Global sound loading\n\n");

				global_sound_list = stdList_Create(sizeof(Sound), 0);
				scene_sound_list = stdList_Create(sizeof(Sound), 0);
				FOR_EACH_TEMP_LIST(filesInfos, FilesInfo, SearchFilesInfos(fs_path.path_data.m_path, "wav"),
					Sound tmp = CreateSound(STD_GETDATA(filesInfos, FilesInfo, i)->m_path);
				if (strcmp(tmp.m_name, "placeholder") == 0)
					sound_place_holder = tmp;
				else
					global_sound_list->push_back(global_sound_list, &tmp);
					)
			}
		}
		else
		{
			printf_d("No sound directory found, create a ALL/Sounds folder in your resources directory\n\n");
			exit(0);
		}
	}
	NEW_CHAR(resources_path, MAX_PATH_SIZE)
		strcpy_s(resources_path, MAX_PATH_SIZE, resource_directory);
	strcat_s(resources_path, MAX_PATH_SIZE, "/ALL/Musics");
	Path fs_path = fs_create_path(resources_path);
	if (fs_path.exist(&fs_path))
	{
		if (global_music_list == NULL)
		{
			printf_d("Start Global music loading\n\n");

			global_music_list = stdList_Create(sizeof(Music), 0);
			scene_music_list = stdList_Create(sizeof(Music), 0);
			FOR_EACH_TEMP_LIST(filesInfos, FilesInfo, SearchFilesInfos(fs_path.path_data.m_path, "ogg"),
				Music tmp = CreateMusic(STD_GETDATA(filesInfos, FilesInfo, i)->m_path);
			if (strcmp(tmp.m_name, "placeholder") == 0)
				music_place_holder = tmp;
			else
				global_music_list->push_back(global_music_list, &tmp);
				)
		}
	}
	else
	{
		printf_d("No music directory found, create a ALL/Musics folder in your resources directory\n\n");
		exit(0);
	}
}


void LoadSound(const char* path)
{
	Sound tmp = CreateSound(path);
	scene_sound_list->push_back(scene_sound_list, &tmp);
}

void LoadMusic(const char* path)
{
	Music tmp = CreateMusic(path);
	scene_music_list->push_back(scene_music_list, &tmp);
}



void LoadSceneSound(const char* scene, float* soundProgressValue, float* musicProgressValue)
{
	ClearSceneSound();
	__LoadScene(scene, "wav", "Sounds",soundProgressValue ,&LoadSound);
	__LoadScene(scene, "ogg", "Musics",musicProgressValue ,&LoadMusic);
}

void ClearSceneSound(void)
{
	if (scene_sound_list != NULL)
	{
		for (int i = 0; i < scene_sound_list->size(scene_sound_list); i++)
		{
			DeleteSound(STD_GETDATA(scene_sound_list, Sound, i));
		}
		scene_sound_list->clear(scene_sound_list);
	}
	if (scene_music_list != NULL)
	{
		for (int i = 0; i < scene_music_list->size(scene_music_list); i++)
		{
			DeleteMusic(STD_GETDATA(scene_music_list, Music, i));
		}
		scene_music_list->clear(scene_music_list);
	}
}

KSound* GetSound(const char* name)
{
	if (global_sound_list != NULL)
		FOR_EACH_LIST(global_sound_list, Sound, it, tmp,
			if (strcmp(tmp->m_name, name) == 0)
				return tmp->m_sound;
				)

		if (scene_sound_list != NULL)
			FOR_EACH_LIST(scene_sound_list, Sound, it, tmp,
				if (strcmp(tmp->m_name, name) == 0)
					return tmp->m_sound;
					)

			if (sound_place_holder.m_sound)
			{
				printf_d("Sound %s not found, placeholder returned", name);
				return sound_place_holder.m_sound;
			}

	printf_d("No Sound placeholder found, put a placeholder.wav in your %s/ALL/Sounds folder", resource_directory);
	return NULL;
}

KSound* GetMusic(const char* name)
{
	if (global_music_list != NULL)
		FOR_EACH_LIST(global_music_list, Music, it, tmp,
			if (strcmp(tmp->m_name, name) == 0)
				return tmp->m_music;
				)

		if (scene_music_list != NULL)
			FOR_EACH_LIST(scene_music_list, Music, it, tmp,
				if (strcmp(tmp->m_name, name) == 0)
					return tmp->m_music;
					)

			if (music_place_holder.m_music)
			{
				printf_d("Music %s not found, placeholder returned", name);
				return music_place_holder.m_music;
			}

	printf_d("No Music placeholder found, put a placeholder.ogg in your %s/ALL/Musics folder", resource_directory);
	return NULL;
}

void DestroySoundsManager(void)
{
	ClearSceneSound();
	if (global_sound_list != NULL)
	{
		for (int i = 0; i < global_sound_list->size(global_sound_list); i++)
		{
			DeleteSound(STD_GETDATA(global_sound_list, Sound, i));
		}
		global_sound_list->clear(global_sound_list);
		global_sound_list->destroy(&global_sound_list);
	}
	scene_sound_list->destroy(&scene_sound_list);
	if (global_music_list != NULL)
	{
		for (int i = 0; i < global_music_list->size(global_music_list); i++)
		{
			DeleteMusic(STD_GETDATA(global_music_list, Music, i));
		}
		global_music_list->clear(global_music_list);
		global_music_list->destroy(&global_music_list);
	}
	scene_music_list->destroy(&scene_music_list);
}
