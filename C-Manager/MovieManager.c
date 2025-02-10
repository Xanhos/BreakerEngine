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
#include "MovieManager.h"
#include "AudioManager.h"


stdList* global_movie_list, * scene_movie_list;
Movie movie_place_holder;

Movie CreateMovie(const char* path)
{
	Movie tmp;
	Path tmpPath = fs_create_path(path);
	tmp.m_movie = sfeMovie_createFromFile(path);
	tmp.m_path = tmpPath;
	strcpy_s(tmp.m_name, MAX_PATH_SIZE, tmpPath.stem(&tmpPath).path_data.m_path);
	ToLower(tmp.m_name);
	printf_d("Movie {\n\tPath : %s\n\tName: %s\n } loaded\n\n", tmp.m_path.path_data.m_path, tmp.m_name);
	return tmp;
}



void DeleteMovie(Movie* movie)
{
	sfeMovie_destroy(movie->m_movie);
}

void InitMovieManager(void)
{
	NEW_CHAR(resources_path, MAX_PATH_SIZE)
		strcpy_s(resources_path, MAX_PATH_SIZE, resource_directory);
	strcat_s(resources_path, MAX_PATH_SIZE, "/ALL/Movies");
	Path fs_path = fs_create_path(resources_path);
	if (fs_path.exist(&fs_path))
	{
		if (global_movie_list == NULL)
		{
			printf_d("Start Global movie loading\n\n");

			global_movie_list = stdList_Create(sizeof(Movie), 0);
			scene_movie_list = stdList_Create(sizeof(Movie), 0);
			FOR_EACH_TEMP_LIST(filesInfos, FilesInfo, SearchFilesInfos(fs_path.path_data.m_path, "mp4"),
				Movie tmp = CreateMovie(STD_GETDATA(filesInfos, FilesInfo, i)->m_path);
			if (strcmp(tmp.m_name, "placeholder") == 0)
				movie_place_holder = tmp;
			else
				global_movie_list->push_back(global_movie_list, &tmp);
				)
		}
	}
	else
	{
		printf_d("No movie directory found, create a ALL/Movies folder in your resources directory\n\n");
		exit(0);
	}
}

void LoadMovie(const char* path)
{
	Movie tmp = CreateMovie(path);
	scene_movie_list->push_back(scene_movie_list, &tmp);
}


void LoadSceneMovie(const char* scene, float* progressValue)
{
	ClearSceneMovie();
	__LoadScene(scene, "mp4", "Movies", progressValue, &LoadMovie);
}

void ClearSceneMovie(void)
{
	if (scene_movie_list != NULL)
	{
		for (int i = 0; i < scene_movie_list->size(scene_movie_list); i++)
			sfeMovie_destroy(STD_GETDATA(scene_movie_list, Movie, i)->m_movie);
		scene_movie_list->clear(scene_movie_list);
	}
}

sfeMovie* GetMovie(const char* name)
{
	if (global_movie_list != NULL)
		FOR_EACH_LIST(global_movie_list, Movie, it, tmp,
			if (strcmp(tmp->m_name, name) == 0)
				return tmp->m_movie;
				)

		if (scene_movie_list != NULL)
			FOR_EACH_LIST(scene_movie_list, Movie, it, tmp,
				if (strcmp(tmp->m_name, name) == 0)
					return tmp->m_movie;
					)

			if (movie_place_holder.m_movie)
			{
				printf_d("Movie %s not found, placeholder returned", name);
				return movie_place_holder.m_movie;
			}

	printf_d("No Movie placeholder found, put a placeholder.mp4 in your %s/ALL/Movies folder", resource_directory);
	return NULL;
}

void DestroyMoviesManager(void)
{
	ClearSceneMovie();
	if (global_movie_list != NULL)
	{
		for (int i = 0; i < global_movie_list->size(global_movie_list); i++)
			sfeMovie_destroy(STD_GETDATA(global_movie_list, Movie, i)->m_movie);
		global_movie_list->clear(global_movie_list);
	}
	scene_movie_list->destroy(&scene_movie_list);
	global_movie_list->destroy(&global_movie_list);
}
