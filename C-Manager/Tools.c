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
#include "Tools.h"


DECLARE_ALL_BASICS_OPERATION_VECTOR2_IN_C(sfVector2f, f, float)
DECLARE_ALL_BASICS_OPERATION_VECTOR2_IN_C(sfVector2i, i, int)
DECLARE_ALL_BASICS_OPERATION_VECTOR2_IN_C(sfVector2u, u, unsigned int)


struct clock_data
{
	sfClock* m_clock;
	sfTime m_time;
};

struct key_data
{
	sfBool m_down;
	sfBool m_up;
};

struct key_data key_info[sfKeyCount - 1];

static void Restart_clock(Clock* clock)
{
	clock->_Data->m_time = sfClock_restart(clock->_Data->m_clock);
}

static float Get_Delta_Time(Clock* clock)
{
	return sfTime_asSeconds(clock->_Data->m_time);
}

static float Frame_Rate(Clock* clock)
{
	return 1.f / Get_Delta_Time(clock);
}

static void Clock_Destroy(Clock** clock)
{
	sfClock_destroy((*clock)->_Data->m_clock);
	free((*clock)->_Data);
	free(*clock);
	*clock = NULL;
}


Clock* CreateClock()
{
	Clock* tmp = (Clock*)calloc(1, sizeof(Clock));
	assert(tmp);
	tmp->_Data = (clock_data*)calloc(1, sizeof(clock_data));
	assert(tmp->_Data);

	tmp->_Data->m_clock = sfClock_create();
	tmp->_Data->m_time = sfTime_Zero;

	tmp->restartClock = &Restart_clock;
	tmp->getDeltaTime = &Get_Delta_Time;
	tmp->frameRate = &Frame_Rate;
	tmp->destroy = &Clock_Destroy;

	return tmp;
}

void ToLower(char* sentence)
{
	int i = 0;
	while (sentence[i] != '\0')
	{
		sentence[i] = tolower(sentence[i]);
		i++;
	}
}

void ToUpper(char* sentence)
{
	int i = 0;
	while (sentence[i] != '\0')
	{
		sentence[i] = toupper(sentence[i]);
		i++;
	}
}

char* StrAllocNCopy(const char* name)
{
	return _strdup(name);	
}

stdList* SearchFilesInfos(const char* path, const char* extension)
{
	stdList* filesList = stdList_Create(sizeof(FilesInfo), 0);
	Path Converted_Path = fs_create_path(path);
	FOR_EACH_RECURSIVE_ITERATOR(Converted_Path, filesIterator,
		Path tmpPath = *STD_GETDATA(filesIterator, Path, i);
	Path tmpExtension = tmpPath.extension(&tmpPath);
	if (strcmp(tmpExtension.path_data.m_path, extension) == 0)
	{
		FilesInfo tmpFilesInfos;
		strcpy_s(tmpFilesInfos.m_name, MAX_PATH_SIZE, tmpPath.stem(&tmpPath).path_data.m_path);
		strcpy_s(tmpFilesInfos.m_path, MAX_PATH_SIZE, tmpPath.path_data.m_path);
		filesList->push_back(filesList, &tmpFilesInfos);
	}
		)
		return filesList;
}

void __LoadWithThread(void* thread_infos)
{
	thread_info* infos = thread_infos;
	for (int it = infos->start; it < infos->end; it++)
	{
		infos->func(STD_GETDATA(infos->files_info, FilesInfo, it)->m_path);
	}
}

void __LoadScene(const char* scene, const char* extension, const char* type, void(*func)(const char*))
{
	NEW_CHAR(path, MAX_PATH_SIZE)
		strcpy_s(path, MAX_PATH_SIZE, resource_directory);
	strcat_s(path, MAX_PATH_SIZE, "/");
	strcat_s(path, MAX_PATH_SIZE, scene);
	strcat_s(path, MAX_PATH_SIZE, "/");
	strcat_s(path, MAX_PATH_SIZE, type);

	Path tmp_path = fs_create_path(path);
	if (tmp_path.exist(&tmp_path))
	{
		stdList* files_infos = SearchFilesInfos(tmp_path.path_data.m_path, extension);
		if (files_infos->size(files_infos) == 0)
		{
			printf_d("%s folder is empty\n", path);
			files_infos->destroy(&files_infos);
			return;
		}

		int nbrThread = files_infos->size(files_infos) < MAX_THREAD ? files_infos->size(files_infos) : MAX_THREAD;
		int block_size = files_infos->size(files_infos) / nbrThread;


		stdList* thread_list = STD_LIST_CREATE(sfThread*, 0);
		stdList* thread_infos = STD_LIST_CREATE(thread_info, 0);

		for (int i = 0; i < nbrThread; i++)
		{
			int start = i * block_size;
			int end = (i == nbrThread - 1) ? files_infos->size(files_infos) : (i + 1) * block_size;

			thread_info tmp_thread_info;
			tmp_thread_info.end = end;
			tmp_thread_info.start = start;
			tmp_thread_info.files_info = files_infos;
			tmp_thread_info.func = func;

			thread_infos->push_back(thread_infos, &tmp_thread_info);
			sfThread* tmp_thread = NULL;
			thread_list->push_back(thread_list, &tmp_thread);
			*STD_GETDATA(thread_list, sfThread*, i) = sfThread_create(&__LoadWithThread, STD_GETDATA(thread_infos, thread_info, i));
			sfThread_launch(*STD_GETDATA(thread_list, sfThread*, i));

		}

		FOR_EACH_LIST(thread_list, sfThread*, i, tmp,
			sfThread_wait(*tmp);
		sfThread_destroy(*tmp);
			)

			thread_list->destroy(&thread_list);
		thread_infos->destroy(&thread_infos);
		files_infos->destroy(&files_infos);
	}
	else
	{
		printf_d("No %s directory found", path);
	}
}


sfBool Circle_Collision(sfVector2f _pos1, sfVector2f _pos2, float _rayon1, float _rayon2)
{
	if ((_pos1.x - _pos2.x) * (_pos1.x - _pos2.x) + (_pos1.y - _pos2.y) * (_pos1.y - _pos2.y) < (_rayon1 + _rayon2) * (_rayon1 + _rayon2))
	{
		return sfTrue;
	}
	else return sfFalse;
}

sfBool Rectangle_Collision(sfFloatRect _box1, sfFloatRect _box2)
{
	if ((_box2.left >= _box1.left + _box1.width)      // trop à droite
		|| (_box2.left + _box2.width <= _box1.left) // trop à gauche
		|| (_box2.top >= _box1.top + _box1.height) // trop en bas
		|| (_box2.top + _box2.height <= _box1.top))  // trop en haut
		return sfFalse;
	else
		return sfTrue;
}

int iRand(int _min, int _max)
{
	if (_max > _min)
		return rand() % (_max - _min + 1) + _min;
	else
		return _min;
}

sfBool PointInCircle(sfVector2f _pos, sfVector2f _circle_pos, float _rayon)
{
	return ((_pos.x - _circle_pos.x) * (_pos.x - _circle_pos.x) + (_pos.y - _circle_pos.y) * (_pos.y - _circle_pos.y) < _rayon * _rayon);
}

sfBool PointInRectangle(sfVector2f _pos, sfFloatRect _box)
{
	if (_pos.x >= _box.left &&         
		_pos.x <= _box.left + _box.width &&    
		_pos.y >= _box.top &&         
		_pos.y <= _box.top + _box.height) {   
		return sfTrue;
	}
	return sfFalse;
}

int rand_int(int _min, int _max)
{
	return rand() % (_max - _min) + _min;
}

double rand_float(float _min, float _max)
{
	return ((rand() / (double)RAND_MAX) * ((double)_max - (double)_min) + (double)_min);
}


float GetDistance(sfVector2f a_, sfVector2f b_)
{
	return (float)sqrt((b_.x - a_.x) * (b_.x - a_.x) + (b_.y - a_.y) * (b_.y - a_.y));
}


float GetNorme(sfVector2f _vec)
{
	return sqrtf(_vec.x * _vec.x + _vec.y * _vec.y);
}


sfVector2f NormalizeVector2f(sfVector2f _vec)
{
	if (_vec.x == 0 && _vec.y == 0)
		return _vec;
	return DivideVector2f(_vec, GetNorme(_vec));
}

sfVector2f GetVectorFromAngle(sfVector2f pos, float lenght, float angle)
{
	angle *= DEG2RAD;

	float x = pos.x + lenght * cos(angle);
	float y = pos.y + lenght * sin(angle);

	return sfVector2f_Create(x, y);
}

sfColor CreateColor(int r, int g, int b, int alpha)
{
	return sfColor_fromRGBA(r, g, b, alpha);
}


int iClamp(int value, int min, int max)
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
	return value;
}

float fClamp(float value, float min, float max)
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
	return value;
}



sfBool sfKeyboard_isKeyDown(sfKeyCode key)
{
	if (sfKeyboard_isKeyPressed(key) && key_info[key].m_down == sfFalse)
	{
		key_info[key].m_down = sfTrue;
		return sfTrue;
	}
	else if (!sfKeyboard_isKeyPressed(key) && key_info[key].m_down == sfTrue)
	{
		key_info[key].m_down = sfFalse;
		return sfFalse;
	}
	return sfFalse;
}

sfBool sfKeyboard_isKeyUp(sfKeyCode key)
{
	if (!sfKeyboard_isKeyPressed(key) && key_info[key].m_up == sfTrue)
	{
		key_info[key].m_up = sfFalse;
		return sfTrue;
	}
	else if (sfKeyboard_isKeyPressed(key) && key_info[key].m_up == sfFalse)
	{
		key_info[key].m_up = sfTrue;
		return sfFalse;
	}
	return sfFalse;
}

void DebugPrint(const char* const string, ...)
{
#ifdef _DEBUG
	va_list args;
	va_start(args, string);
	vprintf(string, args);
	va_end(args);
#endif // _DEBUG
}
