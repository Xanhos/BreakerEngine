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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dirent.h"
#include <List.h>


/**
 * @file filesystem.h
 * @brief This file contains a extension tools for directory and files management.
*/

#define MAX_PATH_SIZE 512 /**< Maximum size for file paths. */

/**
 * @brief Macro to declare a new character array of a specified size and initialize it to zero.
 */
#define NEW_CHAR(name, size) char name[size] = {0};

typedef unsigned char fsBool; /**< Boolean type for the file system functions. */
#define TRUE 1  /**< Boolean TRUE value. */
#define FALSE 0 /**< Boolean FALSE value. */

/**
 * @brief Macro to iterate over all files and directories recursively in a given path.
 * @param path The base path to start iteration.
 * @param name The variable name for the iterator list.
 * @param func The operation to perform for each item.
 */
#define FOR_EACH_RECURSIVE_ITERATOR(path, name, func) \
stdList* name = fs_recursive_iterator_directory(&path); \
for (int i = 0; i < name->size(name); i++) \
{func} \
name->destroy(&name);

 /**
  * @brief Macro to iterate over all files and directories in a given path (non-recursive).
  * @param path The base path to start iteration.
  * @param name The variable name for the iterator list.
  * @param func The operation to perform for each item.
  */
#define FOR_EACH_ITERATOR(path, name, func) \
stdList* name = fs_iterator_directory(&(path)); \
for (int i = 0; i < (name)->size(name); i++) \
{func} \
name->destroy(&(name));

  /**
   * @struct fs_path
   * @brief Internal representation of a file system path.
   */
typedef struct fs_path fs_path;
struct fs_path
{
    char m_path[MAX_PATH_SIZE]; /**< The file path as a string. */
    size_t length;              /**< Length of the file path string. */
};

/**
 * @struct Path
 * @brief High-level representation of a file system path with utility functions.
 */
typedef struct Path Path;
struct Path
{
    fs_path path_data; /**< Internal file path data. */

    /**
     * @brief Checks if the path is a directory.
     * @param path Pointer to the Path object.
     * @return TRUE if it is a directory, FALSE otherwise.
     */
    fsBool(*is_directory)(Path* path);

    /**
     * @brief Checks if the path is a file.
     * @param path Pointer to the Path object.
     * @return TRUE if it is a file, FALSE otherwise.
     */
    fsBool(*is_file)(Path* path);

    /**
     * @brief Checks if the path exists.
     * @param path Pointer to the Path object.
     * @return TRUE if the path exists, FALSE otherwise.
     */
    fsBool(*exist)(Path* path);

    /**
     * @brief Retrieves the parent directory of the path.
     * @param path Pointer to the Path object.
     * @return A new Path object representing the parent directory.
     */
    Path(*parent)(Path* path);

    /**
     * @brief Retrieves the file extension of the path.
     * @param path Pointer to the Path object.
     * @return A new Path object representing the file extension.
     */
    Path(*extension)(Path* path);

    /**
     * @brief Retrieves the stem (base name without extension) of the path.
     * @param path Pointer to the Path object.
     * @return A new Path object representing the stem.
     */
    Path(*stem)(Path* path);

    /**
     * @brief Retrieves the file name (with extension) of the path.
     * @param path Pointer to the Path object.
     * @return A new Path object representing the file name.
     */
    Path(*filename)(Path* path);
};

/**
 * @brief Creates a Path object from a given string.
 * @param path String representing the file system path.
 * @return A Path object initialized with the provided path.
 */
Path fs_create_path(const char* path);

/**
 * @brief Creates a new directory at the specified path.
 * @param path String representing the directory path to create.
 * @return TRUE if the directory was created successfully, FALSE otherwise.
 */
fsBool fs_create_directory(const char* path);

/**
 * @brief Removes a file or directory at the specified path.
 * @param path String representing the file or directory path to remove.
 * @return TRUE if the removal was successful, FALSE otherwise.
 */
fsBool fs_remove(const char* path);

/**
 * @brief Retrieves a list of files and directories in the specified path (non-recursive).
 * @param path Pointer to the Path object representing the directory to iterate.
 * @return A pointer to a stdList containing the files and directories.
 */
stdList* fs_iterator_directory(Path* path);

/**
 * @brief Recursively retrieves a list of files and directories in the specified path.
 * @param path Pointer to the Path object representing the directory to iterate.
 * @return A pointer to a stdList containing the files and directories.
 */
stdList* fs_recursive_iterator_directory(Path* path);

/**
 * @brief Checks if the given Path represents a directory.
 * @param path Pointer to the Path object.
 * @return TRUE if it is a directory, FALSE otherwise.
 */
fsBool is_directory(Path* path);

/**
 * @brief Checks if the given Path represents a file.
 * @param path Pointer to the Path object.
 * @return TRUE if it is a file, FALSE otherwise.
 */
fsBool is_file(Path* path);

/**
 * @brief Checks if the given Path exists in the file system.
 * @param path Pointer to the Path object.
 * @return TRUE if the Path exists, FALSE otherwise.
 */
fsBool exist(Path* path);

/**
 * @brief Retrieves the parent directory of the given Path.
 * @param path Pointer to the Path object.
 * @return A new Path object representing the parent directory.
 */
Path parent(Path* path);

/**
 * @brief Retrieves the file extension from the given Path.
 * @param path Pointer to the Path object.
 * @return A new Path object representing the file extension.
 */
Path extension(Path* path);
