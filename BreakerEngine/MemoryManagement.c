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
#include "MemoryManagement.h"

typedef struct AllocInfo AllocInfo;
struct AllocInfo
{
	void* ptr;
	size_t size;
	const char* file;
	unsigned int line;
};

static stdList* allocations = NULL;
static size_t totalAllocated = 0;
static size_t totalFreed = 0;

static void allocationTracker(void* ptr, size_t size, const char* file, unsigned int line)
{
	if (!allocations)
		allocations = stdList_Create(sizeof(AllocInfo), 0, NULL);
	allocations->push_back(allocations, STD_CONVERT(AllocInfo,  ptr,size,file,line ));
	totalAllocated += size;
}

void* TrackerCalloc(size_t count, size_t size, const char* file, unsigned int line)
{
	void* ptr = calloc(count, size);
	allocationTracker(ptr, size, file, line);
	return ptr;
}

void DetrackerCalloc(void* ptr)
{
	FOR_EACH_LIST(allocations, AllocInfo, it, tmp,
		if (tmp->ptr == ptr)
		{
			free(tmp->ptr);
			totalFreed += tmp->size;
			allocations->erase(allocations, it);
			return;
		}
			)


	free(ptr);
	
}


void ReportLeaks(void)
{
	if (allocations)
	{
		if(allocations->size(allocations) > 0)
		printf_d("Memory leaks detected:\n");
		FOR_EACH_LIST(allocations, AllocInfo, it, tmp,
			printf_d("Leaked %i bytes at %p (allocated in %s : %i\n", (int)tmp->size, tmp->ptr, tmp->file, tmp->line);
		)
			printf_d("Total allocated: %i\n", (int)totalAllocated);
		printf_d("Total freed: %i\n", (int)totalFreed);
	}
}

