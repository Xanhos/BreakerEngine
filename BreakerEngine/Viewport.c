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
#include "Viewport.h"
#include "MemoryManagement.h"

struct Viewport_Data {
	sfVector2u window_size;
	sfView* view;
};

static sfFloatRect CalculateNewPort(sfVector2u windowSize ,sfFloatRect port)
{
	sfFloatRect rect = {
		.left = port.left / windowSize.x,
		.top = port.top / windowSize.y,
		.width = port.width / windowSize.x,
		.height = port.height / windowSize.y
	};
	
	return rect;
}

static void UpdatePort(Viewport* viewport,sfFloatRect port)
{
	sfView_setViewport(viewport->_Data->view, CalculateNewPort(viewport->_Data->window_size, port));
}

static void UpdateWindowSize(Viewport* viewport, sfVector2u size)
{
	viewport->_Data->window_size = size;
}

static sfView* GetViewFromViewPort(Viewport* viewport)
{
	return viewport->_Data->view; 
}

static void Zoom(Viewport* viewport ,float factor)
{
	sfView_zoom(viewport->_Data->view, 1 + factor / 100.f);
}

static void Dezoom(Viewport* viewport, float factor)
{
	sfView_zoom(viewport->_Data->view, 1 - factor / 100.f);
}


Viewport* CreateViewport(sfVector2u windowSize, sfVector2f size, sfFloatRect port)
{
	Viewport* viewport = calloc_d(Viewport, 1);
	assert(viewport);
	Viewport_Data* viewport_data = calloc_d(Viewport_Data, 1);;
	assert(viewport_data);

	viewport_data->window_size;
	sfFloatRect rect = CalculateNewPort(windowSize, port);
	sfView* view = sfView_createFromRect(rect);
	sfView_setSize(view, size);
	viewport_data->view = view;
	viewport->_Data = viewport_data;

	viewport->UpdatePort = &UpdatePort;
	viewport->UpdateWindowSize = &UpdateWindowSize;
	viewport->Dezoom = &Dezoom;
	viewport->Zoom = &Zoom;
	viewport->GetView = &GetViewFromViewPort;

	return viewport;
}
