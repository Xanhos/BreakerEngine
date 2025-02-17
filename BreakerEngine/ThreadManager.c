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
#include "ThreadManager.h"
#include "MemoryManagement.h"

struct ThreadManager_Data
{
	size_t m_limit;
	size_t m_actual_size;
	stdList* m_thread_list;
};

typedef struct ThreadFunctionInfo ThreadFunctionInfo;
struct ThreadFunctionInfo
{
	void (*func)(void*);
	void* func_data;
	sfThread* m_thread;
	sfBool m_is_finish;
	sfBool m_data_is_copied;
};



static size_t GetThreadCount(const ThreadManager* thread_manager)
{
	return thread_manager->_Data->m_actual_size;
}

static void ThreadFunction(void* data)
{
	ThreadFunctionInfo* thread_info = data;
	thread_info->func(thread_info->func_data);
	thread_info->m_is_finish = sfTrue;
}

static ThreadFunctionInfo* CreateThreadFunction(void (*func)(void*), void* data)
{
	ThreadFunctionInfo* thread_function_info = calloc_d(ThreadFunctionInfo, 1);
	assert(thread_function_info);
	thread_function_info->func = func;
	thread_function_info->func_data = data;
	thread_function_info->m_thread = sfThread_create(&ThreadFunction, thread_function_info);
	thread_function_info->m_is_finish = sfFalse;
	return thread_function_info;
}

static void DestroyThreadFunction(ThreadFunctionInfo** thread_function_info)
{
	ThreadFunctionInfo* abbreviate_thread_function_info = *thread_function_info;
	sfThread_wait(abbreviate_thread_function_info->m_thread);
	sfThread_destroy(abbreviate_thread_function_info->m_thread);
	if (abbreviate_thread_function_info->m_data_is_copied)
		free_d(abbreviate_thread_function_info->func_data);
	free_d(abbreviate_thread_function_info);
	*thread_function_info = NULL;
}

static void UpdateThreadManager(ThreadManager* thread_manager)
{
	stdList* thread_list = thread_manager->_Data->m_thread_list;
	do
	{
		for (int i = 0; i < thread_list->size(thread_list); i++)
		{
			if ((*STD_GETDATA(thread_list, ThreadFunctionInfo*, i))->m_is_finish)
			{
				DestroyThreadFunction(STD_GETDATA(thread_list, ThreadFunctionInfo*, i));
				thread_manager->_Data->m_actual_size--;
				thread_list->erase(thread_list, i);
			}
		}
	} while (thread_manager->_Data->m_limit == thread_manager->_Data->m_actual_size);
}

static void AddNewThread(ThreadManager* thread_manager, void (*func)(void*), void* func_data, sfBool copy_data, size_t data_size)
{
	UpdateThreadManager(thread_manager);
	if (copy_data)
	{
		void* tmp = TrackerCalloc(1, data_size, "C:\\Users\\y.grallan\\Documents\\BreakerEngine\\BreakerEngine\\ThreadManager.c", 101);
		assert(tmp);
		memcpy(tmp, func_data, data_size);
		func_data = tmp;
	}
	ThreadFunctionInfo* newThread = CreateThreadFunction(func, func_data);
	thread_manager->_Data->m_actual_size++;
	newThread->m_data_is_copied = copy_data;
	thread_manager->_Data->m_thread_list->push_back(thread_manager->_Data->m_thread_list, &newThread);
	sfThread_launch(newThread->m_thread);
}

static void DestroyThreadManager(ThreadManager** thread_manager)
{
	while ((*thread_manager)->_Data->m_actual_size)
		UpdateThreadManager(*thread_manager);

	(*thread_manager)->_Data->m_thread_list->destroy(&(*thread_manager)->_Data->m_thread_list);
	free_d((*thread_manager)->_Data);
	free_d(*thread_manager);
	*thread_manager = NULL;
}

ThreadManager* CreateThreadManager(size_t limit)
{
	ThreadManager* tmp = calloc_d(ThreadManager, 1);
	ThreadManager_Data* tmp_data = calloc_d(ThreadManager_Data, 1);
	assert(tmp);
	assert(tmp_data);

	tmp_data->m_limit = limit;
	tmp_data->m_thread_list = STD_LIST_CREATE(ThreadFunctionInfo*, 0);
	tmp_data->m_actual_size = 0;

	tmp->_Data = tmp_data;

	tmp->AddNewThread = &AddNewThread;
	tmp->Update = &UpdateThreadManager;
	tmp->Destroy = &DestroyThreadManager;
	tmp->GetThreadCount = &GetThreadCount;

	return tmp;
}