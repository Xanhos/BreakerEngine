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

/**
 * @file memorymanagement.h
 * @brief This file contains a extension tools for the memory management of the engine.
*/


/**
 * @brief Allocates memory for an array of elements, with leak tracking enabled.
 * This macro wraps the `TrackerCalloc` function for easier usage.
 * @param type The data type of the elements.
 * @param count The number of elements to allocate.
 * @return A pointer to the allocated memory, cast to the specified type.
 */
#define calloc_d(type, count) (type*)TrackerCalloc(count, sizeof(type), __FILE__, __LINE__)

 /**
  * @brief Frees memory allocated by `calloc_d`, with leak tracking enabled.
  * This macro wraps the `DetrackerCalloc` function for easier usage.
  * @param ptr Pointer to the memory to free.
  */
#define free_d(ptr) DetrackerCalloc(ptr)

  /**
   * @brief Allocates memory for an array of elements and tracks the allocation.
   * @param count The number of elements to allocate.
   * @param size The size of each element in bytes.
   * @param file The source file where the allocation is made.
   * @param line The line number in the source file where the allocation is made.
   * @return A pointer to the allocated memory.
   */
void* TrackerCalloc(size_t count, size_t size, const char* file, unsigned int line);

/**
 * @brief Frees memory allocated by `TrackerCalloc` and removes it from tracking.
 * @param ptr Pointer to the memory to free.
 */
void DetrackerCalloc(void* ptr);

/**
 * @brief Reports all memory leaks detected by the tracker.
 * This function outputs information about unfreed memory allocations.
 */
void ReportLeaks(void);
