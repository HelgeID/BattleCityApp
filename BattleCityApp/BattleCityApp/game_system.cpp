#include "game.h"
#include "general.hpp"

//System functions
bool Game::loadTexture()
{
	sf::Image image;
	if (!image.loadFromFile("data/graphics/texture/texture.png"))
		return EXIT_FAILURE;

	image.createMaskFromColor(image.getPixel(0, 0));
	this->texture->loadFromImage(image, sf::IntRect(0, 0, 400, 256));
	
	return EXIT_SUCCESS;
}

bool Game::loadIcon()
{
	sf::Image icon;
	if (!icon.loadFromFile("data/graphics/icon/icon.png"))
		return EXIT_FAILURE;
	window.setIcon(176, 176, icon.getPixelsPtr());
	return EXIT_SUCCESS;
}


void Game::zoomOn()
{
	if (p_zoom == false)
	{
		sf::Vector2u vec;
		vec.x = static_cast<float>(window.getSize().x);
		vec.y = static_cast<float>(window.getSize().y);
		vec.x *= 2; vec.y *= 2;
		window.setSize(vec);
		p_zoom = true;
	}
	return;
}

void Game::zoomOff()
{
	if (p_zoom == true)
	{
		sf::Vector2u vec;
		vec.x = static_cast<float>(window.getSize().x);
		vec.y = static_cast<float>(window.getSize().y);
		vec.x /= 2; vec.y /= 2;
		window.setSize(vec);
		p_zoom = false;
	}
	return;
}

#include <Windows.h>
void Game::winStyle(sf::Window& window, sf::Uint32 style)
{
	HWND handle = window.getSystemHandle();
	DWORD win32Style = WS_VISIBLE;

	if (style == sf::Style::None) {
		win32Style |= WS_POPUP;
	}
	else {
		if (style & sf::Style::Titlebar) win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
		if (style & sf::Style::Resize)   win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
		if (style & sf::Style::Close)    win32Style |= WS_CAPTION | WS_SYSMENU;
	}

	SetWindowLongPtr(handle, GWL_STYLE, win32Style);

	// force changes to take effect
	SetWindowPos(handle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_DRAWFRAME);
	return;
}

void Game::HideConsole()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	return;
}

void Game::ShowConsole()
{
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	return;
}

bool Game::IsConsoleVisible()
{
	return (IsWindowVisible(GetConsoleWindow()) != FALSE);
}


void Game::TakeConsolePos(int* x, int* y)
{
	HWND consoleWindow = GetConsoleWindow();
	HWND hWndParent = GetParent(consoleWindow);
	POINT point = { 0 };
	MapWindowPoints(consoleWindow, hWndParent, &point, 1);
	(*x) = point.x;
	(*y) = point.y;
	return;
}
