﻿#include "game.h"
#include "general.hpp"
#include "events.h"
#include "field.h"

std::mutex mtx;

Game::Game(sf::RenderWindow &window)
	: window(window), texture(new sf::Texture())
{
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	loadTexture();
	loadIcon();
}

Game::~Game()
{
	delete texture;
}

//main loader game
void Game::GameLaunch()
{
	// initialization zoom
	zoomOn();

	// show frame
	//p_showframe = true;

	// parameters
	//undying_enemy = true;
	//undying_players = true;
	//undying_hit_on_player = true;
	//blocking_firing = true;

	// initialization of objects
	GameEvent gEvent(window);
	GameField gField(window, *texture);

	// update
	while (window.isOpen())
	{
		//Update Event
		gEvent.UpdateEvent();

		//Update Field
		{
			std::lock_guard<std::mutex> lg(mtx);
			gField.UpdateField();
		}
	}

	return;
}
