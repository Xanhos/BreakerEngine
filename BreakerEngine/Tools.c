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
#include "WindowManager.h"
#include "MemoryManagement.h"

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
	sfBool m_hold;
	sfBool m_up;
};

struct key_data key_info[sfKeyCount - 1];
struct key_data mouse_info[sfMouseButtonCount - 1];

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
	free_d((*clock)->_Data);
	free_d(*clock);
	*clock = NULL;
}


Clock* CreateClock()
{
	Clock* tmp = calloc_d(Clock, 1);
	assert(tmp);
	tmp->_Data = calloc_d(clock_data, 1);
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
		*infos->currentSize += 1;
		*infos->progressValue = *infos->currentSize / *infos->totalSize;
	}
}

void __LoadScene(const char* scene, const char* extension, const char* type, float* progressValue, void(*func)(const char*))
{
	NEW_CHAR(path, MAX_PATH_SIZE)
		strcpy_s(path, MAX_PATH_SIZE, resource_directory);
	strcat_s(path, MAX_PATH_SIZE, "/");
	strcat_s(path, MAX_PATH_SIZE, scene);
	strcat_s(path, MAX_PATH_SIZE, "/");
	strcat_s(path, MAX_PATH_SIZE, type);

	*progressValue = 0.0f;
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

		float totalSize = 0.f, currentSize = 0.f;

		FOR_EACH_LIST(files_infos, FilesInfo, i, it,
			totalSize ++;
		);

		int nbrThread = files_infos->size(files_infos) < MAX_THREAD ? files_infos->size(files_infos) : MAX_THREAD;
		int block_size = files_infos->size(files_infos) / nbrThread;


		stdList* thread_list = STD_LIST_CREATE(sfThread*, 0);
		stdList* thread_infos = STD_LIST_CREATE(thread_info, 0);

		for (int i = 0; i < nbrThread; i++)
		{
			int start = i * block_size;
			int end = (i == nbrThread - 1) ? files_infos->size(files_infos) : (i + 1) * block_size;

			thread_info tmp_thread_info = {
			.end = end,
			.start = start,
			.files_info = files_infos,
			.func = func,
			.progressValue = progressValue,
			.currentSize = &currentSize,
			.totalSize = &totalSize
			};

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
		printf_d("No %s directory found\n\n", path);
	}
	*progressValue = 1.0f;
}

void UpdateKeyAndMouseState(void)
{
	for (sfKeyCode i = 0; i < sfKeyCount - 1; i++)
	{
		if (sfKeyboard_isKeyPressed(((sfKeyCode)i)) && key_info[i].m_down == sfFalse && key_info[i].m_hold == sfFalse)
		{
			key_info[i].m_down = sfTrue;
			key_info[i].m_hold = sfTrue;
		}
		else if (sfKeyboard_isKeyPressed(((sfKeyCode)i)) && key_info[i].m_hold == sfTrue)
		{
			key_info[i].m_down = sfFalse;
			key_info[i].m_up = sfFalse;
		}
		else if (!sfKeyboard_isKeyPressed(((sfKeyCode)i)) && key_info[i].m_hold == sfTrue)
		{
			key_info[i].m_hold = sfFalse;
			key_info[i].m_up = sfTrue;
		}
		else if (!sfKeyboard_isKeyPressed(((sfKeyCode)i)) && key_info[i].m_hold == sfFalse)
		{
			key_info[i].m_down = sfFalse;
			key_info[i].m_up = sfFalse;
		}
	}
	for (sfMouseButton i = 0; i < sfMouseButtonCount - 1; i++)
	{
		if (sfMouse_isButtonPressed(((sfMouseButton)i)) && mouse_info[i].m_down == sfFalse && mouse_info[i].m_hold == sfFalse)
		{
			mouse_info[i].m_down = sfTrue;
			mouse_info[i].m_hold = sfTrue;
		}
		else if (sfMouse_isButtonPressed(((sfMouseButton)i)) && mouse_info[i].m_hold == sfTrue)
		{
			mouse_info[i].m_down = sfFalse;
			mouse_info[i].m_up = sfFalse;
		}
		else if (!sfMouse_isButtonPressed(((sfMouseButton)i)) && mouse_info[i].m_hold == sfTrue)
		{
			mouse_info[i].m_hold = sfFalse;
			mouse_info[i].m_up = sfTrue;
		}
		else if (!sfMouse_isButtonPressed(((sfMouseButton)i)) && mouse_info[i].m_hold == sfFalse)
		{
			mouse_info[i].m_down = sfFalse;
			mouse_info[i].m_up = sfFalse;
		}
	}
}

sfVector2f ScaleVectorIfNotMouse(sfVector2f vec)
{
	return vec.x == sfMouse_getPositionRenderWindow(GameWindow->GetWindow(GameWindow)).x && vec.y == sfMouse_getPositionRenderWindow(GameWindow->GetWindow(GameWindow)).y ? vec : sfVector2f_Create(vec.x * ScreenScaleFactorX, vec.y * ScreenScaleFactorY);
}

sfFloatRect ScaleFloatRect(sfFloatRect rect)
{
	return (sfFloatRect) { rect.left* ScreenScaleFactorX, rect.top* ScreenScaleFactorY, rect.width* ScreenScaleFactorX, rect.height* ScreenScaleFactorY };
}

sfBool Circle_Collision(sfVector2f _pos1, sfVector2f _pos2, float _rayon1, float _rayon2, sfBool shouldScale)
{
	if (shouldScale)
	{
		_pos1 = ScaleVectorIfNotMouse(_pos1);
		_pos2 = ScaleVectorIfNotMouse(_pos2);
		_rayon1 *= MIN(ScreenScaleFactorX, ScreenScaleFactorY);
		_rayon2 *= MIN(ScreenScaleFactorX, ScreenScaleFactorY);
	}
	if ((_pos1.x - _pos2.x) * (_pos1.x - _pos2.x) + (_pos1.y - _pos2.y) * (_pos1.y - _pos2.y) < (_rayon1 + _rayon2) * (_rayon1 + _rayon2))
	{
		return sfTrue;
	}
	else return sfFalse;
}

sfBool Rectangle_Collision(sfFloatRect _box1, sfFloatRect _box2, sfBool shouldScale)
{
	if (shouldScale)
	{
		_box1 = ScaleFloatRect(_box1);
		_box2 = ScaleFloatRect(_box2);
	}
	if ((_box2.left >= _box1.left + _box1.width)
		|| (_box2.left + _box2.width <= _box1.left)
		|| (_box2.top >= _box1.top + _box1.height)
		|| (_box2.top + _box2.height <= _box1.top))
		return sfFalse;
	else
		return sfTrue;
}

sfBool CircleRect_Collision(sfVector2f pos, float radius, sfFloatRect _rect, sfBool shouldScale)
{
	if (shouldScale)
	{
		pos = ScaleVectorIfNotMouse(pos);
		radius *= MIN(ScreenScaleFactorX, ScreenScaleFactorY);
		_rect = ScaleFloatRect(_rect);
	}
	float testX = pos.x;
	float testY = pos.y;

	if (pos.x < _rect.left)         testX = _rect.left;
	else if (pos.x > _rect.left + _rect.width) testX = _rect.left + _rect.width;
	if (pos.y < _rect.top)         testY = _rect.top;
	else if (pos.y > _rect.top + _rect.height) testY = _rect.top + _rect.height;

	float distX = pos.x - testX;
	float distY = pos.y - testY;
	float distance = (float)sqrt((distX * distX) + (distY * distY));

	if (distance <= radius) {
		return sfTrue;
	}
	return sfFalse;
}

sfBool LineCircle_Collision(sfVector2f start_line, sfVector2f end_line, sfVector2f circle_pos, float r, sfBool shouldScale)
{
	if (shouldScale)
	{
		start_line = ScaleVectorIfNotMouse(start_line);
		end_line = ScaleVectorIfNotMouse(end_line);
		circle_pos = ScaleVectorIfNotMouse(circle_pos);
		r *= MIN(ScreenScaleFactorX, ScreenScaleFactorY);
	}
	sfBool inside1 = PointInCircle(start_line, circle_pos, r, sfFalse);
	sfBool inside2 = PointInCircle(end_line, circle_pos, r, sfFalse);
	if (inside1 || inside2)
		return sfTrue;

	float distX = start_line.x - end_line.x;
	float distY = start_line.y - end_line.y;
	float len = (float)sqrt((distX * distX) + (distY * distY));

	float dot = (((circle_pos.x - start_line.x) * (end_line.x - start_line.x)) + ((circle_pos.x - start_line.y) * (end_line.y - start_line.y))) / (float)pow(len, 2);

	float closestX = start_line.x + (dot * (end_line.x - start_line.x));
	float closestY = end_line.y + (dot * (end_line.y - start_line.y));

	sfBool onSegment = LinePoint_Collision(start_line, end_line, sfVector2f_Create(closestX, closestY), 0.1f, sfFalse);
	if (!onSegment)
		return sfFalse;

	distX = closestX - circle_pos.x;
	distY = closestY - circle_pos.y;
	float distance = (float)sqrt((distX * distX) + (distY * distY));

	if (distance <= r) {
		return sfTrue;
	}
	return sfFalse;
}

sfBool LinePoint_Collision(sfVector2f start_line, sfVector2f end_line, sfVector2f point, float offset, sfBool shouldScale)
{
	if (shouldScale)
	{
		start_line = ScaleVectorIfNotMouse(start_line);
		end_line = ScaleVectorIfNotMouse(end_line);
		point = ScaleVectorIfNotMouse(point);
		offset *= MIN(ScreenScaleFactorX, ScreenScaleFactorY);
	}

	float d1 = GetDistance(point, start_line);
	float d2 = GetDistance(point, end_line);
	float lineLen = GetDistance(start_line, end_line);
	float buffer = offset;

	return (d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer) ? sfTrue : sfFalse;
}

sfBool LineLine_Collision(sfVector2f start_line_one, sfVector2f end_line_one, sfVector2f start_line_two, sfVector2f end_line_two, sfBool shouldScale)
{
	if (shouldScale)
	{
		start_line_one = ScaleVectorIfNotMouse(start_line_one);
		end_line_one = ScaleVectorIfNotMouse(end_line_one);
		start_line_two = ScaleVectorIfNotMouse(start_line_two);
		end_line_two = ScaleVectorIfNotMouse(end_line_two);
	}

	float denominator = ((end_line_two.y - start_line_two.y) * (end_line_one.x - start_line_one.x)) -
		((end_line_two.x - start_line_two.x) * (end_line_one.y - start_line_one.y));

	if (denominator == 0)
		return sfFalse;

	float uA = (((end_line_two.x - start_line_two.x) * (start_line_one.y - start_line_two.y)) -
		((end_line_two.y - start_line_two.y) * (start_line_one.x - start_line_two.x))) / denominator;

	float uB = (((end_line_one.x - start_line_one.x) * (start_line_one.y - start_line_two.y)) -
		((end_line_one.y - start_line_one.y) * (start_line_one.x - start_line_two.x))) / denominator;

	return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) ? sfTrue : sfFalse;
}

sfBool LineRect_Collision(sfVector2f start_line, sfVector2f end_line, sfFloatRect rect, sfBool shouldScale)
{
	if (shouldScale)
	{
		start_line = ScaleVectorIfNotMouse(start_line);
		end_line = ScaleVectorIfNotMouse(end_line);
		rect = ScaleFloatRect(rect);
	}

	sfBool left = LineLine_Collision(start_line, end_line, sfVector2f_Create(rect.left, rect.top), sfVector2f_Create(rect.left, rect.top + rect.height), sfFalse);
	sfBool right = LineLine_Collision(start_line, end_line, sfVector2f_Create(rect.left + rect.width, rect.top), sfVector2f_Create(rect.left + rect.width, rect.top + rect.height), sfFalse);
	sfBool top = LineLine_Collision(start_line, end_line, sfVector2f_Create(rect.left, rect.top), sfVector2f_Create(rect.left + rect.width, rect.top), sfFalse);
	sfBool bottom = LineLine_Collision(start_line, end_line, sfVector2f_Create(rect.left, rect.top + rect.height), sfVector2f_Create(rect.left + rect.width, rect.top + rect.height), sfFalse);

	return (left || right || top || bottom) ? sfTrue : sfFalse;
}

sfBool PointInCircle(sfVector2f _pos, sfVector2f _circle_pos, float _rayon, sfBool shouldScale)
{
	if (shouldScale)
	{
		_pos = ScaleVectorIfNotMouse(_pos);
		_circle_pos = ScaleVectorIfNotMouse(_circle_pos);
		_rayon *= MIN(ScreenScaleFactorX, ScreenScaleFactorY);
	}

	return ((_pos.x - _circle_pos.x) * (_pos.x - _circle_pos.x) + (_pos.y - _circle_pos.y) * (_pos.y - _circle_pos.y) < _rayon * _rayon);
}

sfBool PointInRectangle(sfVector2f _pos, sfFloatRect _box, sfBool shouldScale)
{
	if (shouldScale)
	{
		_pos = ScaleVectorIfNotMouse(_pos);
		_box = ScaleFloatRect(_box);
	}

	return (_pos.x >= _box.left &&
		_pos.x <= _box.left + _box.width &&
		_pos.y >= _box.top &&
		_pos.y <= _box.top + _box.height);
}

int iRand(int _min, int _max)
{
	if (_max > _min)
		return rand() % (_max - _min + 1) + _min;
	else
		return _min;
}

int rand_int(int _min, int _max)
{
	if (_max > _min)
		return rand() % (_max - _min + 1) + _min;
	else
		return _min;
}

double rand_float(float _min, float _max)
{
	return ((rand() / (double)RAND_MAX) * ((double)_max - (double)_min) + (double)_min);
}

sfVector2f GetRandomPosInRectangle(sfFloatRect rect)
{
	return sfVector2f_Create((float)rand_float(rect.left, rect.width), (float)rand_float(rect.top, rect.height));
}

sfVector2f GetRandomPosInCircle(sfVector2f pos, float radius)
{
	return GetVectorFromAngle(pos, (float)rand_float(0, radius), (float)rand_float(0, 360));
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

	float x = pos.x + lenght * (float)cos(angle);
	float y = pos.y + lenght * (float)sin(angle);

	return sfVector2f_Create(x, y);
}

sfColor CreateColor(int r, int g, int b, int alpha)

{
	return sfColor_fromRGBA(iClamp(r, 0, 255), iClamp(g, 0, 255), iClamp(b, 0, 255), iClamp(alpha, 0, 255));
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
	return key_info[key].m_down;
}

sfBool sfKeyboard_isKeyUp(sfKeyCode key)
{
	return key_info[key].m_up;
}

sfBool sfMouse_isButtonDown(sfMouseButton button)
{
	return mouse_info[button].m_down;
}

sfBool sfMouse_isButtonUp(sfMouseButton button)
{
	return mouse_info[button].m_up;
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

float GetFileSizeCustom(const char* filePath)
{
	FILE* file = fopen(filePath, "r");
	if (fseek(file, 0, SEEK_END) < 0)
	{
		fclose(file);
		return 0.f;
	}
	long size = ftell(file);
	fclose(file);
	return (float)size;
}
