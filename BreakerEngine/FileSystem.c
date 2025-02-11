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
#include "FileSystem.h"


static void reverseString(char* str, int len, int i, int temp)
{
	if (i < len) {
		temp = str[i];
		str[i] = str[len - 1];
		str[len - 1] = temp;
		i++;
		len--;
		reverseString(str, len, i, temp);
	}
}

static void convertToW_Char(wchar_t* output, const char* copied)
{
	size_t outsize, size = strlen(copied);
	mbstowcs_s(&outsize, output, size + 1, copied, size);
}

static void convertToChar(char* output, wchar_t* copied)
{
	size_t outsize, size = wcslen(copied);
	wcstombs_s(&outsize, output, size + 1, copied, size);
}



fsBool exist(Path* path)
{
	fsBool result = is_file(path);
	if (!result)
		result = is_directory(path);

	return result;
}

fsBool is_directory(Path* path)
{
	DIR* dir = opendir(path->path_data.m_path);
	if (dir)
	{
		closedir(dir);
		return TRUE;
	}
	return FALSE;
}

fsBool is_file(Path* path)
{
	FILE* file = NULL;
	errno_t error_code = fopen_s(&file, path->path_data.m_path, "r");
	if (error_code == 0)
	{
		fclose(file);
		return TRUE;
	}
	return FALSE;
}

Path extension(Path* path)
{
	NEW_CHAR(extension, MAX_PATH_SIZE)
		if (is_file(path))
		{
			int index_extension = 0, index_path = (int)path->path_data.length - 1;
			while (path->path_data.m_path[index_path] != '.')
			{
				extension[index_extension] = path->path_data.m_path[index_path];
				index_path--;
				index_extension++;
			}
			reverseString(extension, (int)strlen(extension), 0, 0);
		}
	return fs_create_path(extension);
}


Path parent(Path* path)
{
	NEW_CHAR(tmp, MAX_PATH_SIZE)
		for (int i = (int)path->path_data.length; i > 0; i--)
		{
			if (path->path_data.m_path[i] == '/' || path->path_data.m_path[i] == '\\')
			{
				i--;
				int index = 0;
				while (i >= 0)
				{
					tmp[index] = path->path_data.m_path[i];
					i--;
					index++;
				}
				reverseString(tmp, (int)strlen(tmp), 0, 0);
				break;
			}
		}

	Path parent = fs_create_path(tmp);
	if (exist(&parent))
		return parent;
	return *path;
}


stdList* fs_iterator_directory(Path* path)
{
	stdList* list = STD_LIST_CREATE(Path, 0);

	DIR* d;
	struct dirent* dir;
	d = opendir(path->path_data.m_path);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			NEW_CHAR(new_entry_path, MAX_PATH_SIZE)
				strcpy_s(new_entry_path, path->path_data.length + 1, path->path_data.m_path);
			strcat_s(new_entry_path, strlen(new_entry_path) + 2, "\\");
			strcat_s(new_entry_path, strlen(new_entry_path) + strlen(dir->d_name) + 1, dir->d_name);

			Path new_entry = fs_create_path(new_entry_path);
			if (is_file(&new_entry))
				list->push_back(list, &new_entry);
		}
		closedir(d);
	}

	return list;
}


static stdList* recursive_iterator_directory__(Path* path, stdList* list)
{
	DIR* d;
	struct dirent* dir;
	d = opendir(path->path_data.m_path);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
				continue;
			NEW_CHAR(new_entry_path, MAX_PATH_SIZE)
				strcpy_s(new_entry_path, path->path_data.length + 1, path->path_data.m_path);
			strcat_s(new_entry_path, strlen(new_entry_path) + 2, "\\");
			strcat_s(new_entry_path, strlen(new_entry_path) + strlen(dir->d_name) + 1, dir->d_name);

			Path new_entry = fs_create_path(new_entry_path);
			if (is_file(&new_entry))
				list->push_back(list, &new_entry);
			else if (is_directory(&new_entry))
				recursive_iterator_directory__(&new_entry, list);

		}
		closedir(d);
	}
	return NULL;
}



stdList* fs_recursive_iterator_directory(Path* path)
{
	stdList* list = STD_LIST_CREATE(Path, 0);
	recursive_iterator_directory__(path, list);
	return list;
}

static Path filename(Path* path)
{
	NEW_CHAR(tmp_filename, MAX_PATH_SIZE)
		size_t index = path->path_data.length - 1;
	size_t index_new_path = 0;
	while (path->path_data.m_path[index] != '\\' && path->path_data.m_path[index] != '/')
	{
		tmp_filename[index_new_path] = path->path_data.m_path[index];
		index_new_path++;
		index--;
	}
	return fs_create_path(_strrev(tmp_filename));
}

static Path stem(Path* path)
{
	if (path->is_file(path))
	{
		Path filename = path->filename(path);
		NEW_CHAR(tmp_file_path, MAX_PATH_SIZE)
			size_t index = filename.path_data.length;
		while (filename.path_data.m_path[index] != '.')
		{
			index--;
		}
		index--;
		size_t index_new_path = 0;
		while (index != 0)
		{
			tmp_file_path[index_new_path] = filename.path_data.m_path[index];
			index_new_path++;
			index--;
		}
		tmp_file_path[index_new_path] = filename.path_data.m_path[index];
		return fs_create_path(_strrev(tmp_file_path));
	}
	return *path;
}

static const char* fs_c_srt(Path* path)
{
	return path->path_data.m_path;
}


Path fs_create_path(const char* path)
{
	Path tmp;
	fs_path tmp_fs_path;
	const size_t path_length = strlen(path) + 1;
	tmp_fs_path.m_path[0] = '\0';
	tmp_fs_path.length = path_length - 1;
	strcpy_s(tmp_fs_path.m_path, path_length, path);

	tmp.path_data = tmp_fs_path;

	tmp.exist = &exist;
	tmp.extension = &extension;
	tmp.is_directory = &is_directory;
	tmp.is_file = &is_file;
	tmp.parent = &parent;
	tmp.stem = &stem;
	tmp.filename = &filename;

	return tmp;
}

fsBool fs_create_directory(const char* path)
{
	wchar_t path_converted[MAX_PATH_SIZE];
	convertToW_Char(path_converted, path);
	return !_wmkdir(path_converted);
}



static fsBool remove_directory_content(wchar_t* path)
{
	WIN32_FIND_DATAW findFileData;
	NEW_CHAR(tmp_first_file, MAX_PATH_SIZE)
		convertToChar(tmp_first_file, path);
	strcat_s(tmp_first_file, MAX_PATH_SIZE, "\\*");
	wchar_t complete_tmp_first_file[MAX_PATH_SIZE];
	convertToW_Char(complete_tmp_first_file, tmp_first_file);
	HANDLE hFind = FindFirstFileW(complete_tmp_first_file, &findFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return FALSE; // Impossible d'accéder au répertoire
	}

	do {
		if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
			// Construction du chemin complet du fichier ou du sous-répertoire
			NEW_CHAR(completed_path, MAX_PATH_SIZE);
			NEW_CHAR(c_file_name_char, MAX_PATH_SIZE);
			convertToChar(completed_path, path);
			convertToChar(c_file_name_char, findFileData.cFileName);
			strcat_s(completed_path, MAX_PATH_SIZE, "/");
			strcat_s(completed_path, MAX_PATH_SIZE, c_file_name_char);

			wchar_t completed_path_wchar[MAX_PATH_SIZE];
			convertToW_Char(completed_path_wchar, completed_path);

			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				// Si c'est un sous-répertoire, supprimez-le récursivement
				if (!remove_directory_content(completed_path_wchar)) {
					FindClose(hFind);
					return FALSE;
				}
			}
			else {
				// Si c'est un fichier, supprimez-le
				if (DeleteFileW(completed_path_wchar) == FALSE) {
					FindClose(hFind);
					return FALSE;
				}
			}
		}
	} while (FindNextFileW(hFind, &findFileData) != 0);

	FindClose(hFind);

	// Une fois que tout le contenu est supprimé, supprimez le répertoire lui-même
	if (RemoveDirectoryW(path) == FALSE) {
		return FALSE;
	}

	return TRUE;
}



static fsBool remove_directory(const char* path)
{
	wchar_t path_wchar[MAX_PATH_SIZE];
	convertToW_Char(path_wchar, path);
	return remove_directory_content(path_wchar);
}


static fsBool remove_file(const char* path)
{
	Path tmp = fs_create_path(path);
	if (is_file(&tmp))
		return (fsBool)remove(path);
	return FALSE;
}


fsBool fs_remove(const char* path)
{
	Path tmp = fs_create_path(path);
	if (tmp.exist(&tmp))
	{
		if (tmp.is_directory(&tmp))
			return remove_directory(tmp.path_data.m_path);
		if (tmp.is_file(&tmp))
			return remove_file(tmp.path_data.m_path);
	}
	return FALSE;
}






