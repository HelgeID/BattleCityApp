#include "game.h"
#include "general.hpp"
#include "events.h"
#include "field.h"

std::mutex mtx;

Game::Game(sf::RenderWindow &window)
	: window(window), texture(new sf::Texture())
{
	window.setFramerateLimit(80);
	window.setVerticalSyncEnabled(true);
	loadTexture();
	loadIcon();
}

Game::~Game()
{
	delete texture;
}

void Game::InitParams()
{
	gameover = false; //default =>FALSE
	pause = false; //default =>FALSE
	no_close = true; //default =>TRUE
	return;
}

//loader "menu"
void Game::GameMenu()
{

	return;
}

//loader "stage screen"
void Game::GameStage()
{
	bool exit(false);
	while (window.isOpen() && !exit)
	{
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				exit = true;
		}

		window.clear(sf::Color(99, 99, 99));
		window.display();
	}
	return;
}

//loader "game"
void Game::GameLaunch()
{
	// initialization zoom
	zoomOn();

	// show frame
	//p_showframe = true;

	// show fps
	p_showfps = true;

	// parameters \\
	----------------
	//undying_enemy = true;
	//undying_players = true;
	//undying_hit_on_player = true;
	//blocking_firing = true;

	//undying_emblem_hit_by_tank = true;
	//undying_emblem_boom_by_tank = true;
	//undying_emblem_absence_players = true;

	// initialization of objects
	GameEvent gEvent(window);
	GameField gField(window, *texture);

	GameFPS gFPS;

	// update
	while (window.isOpen() && no_close)
	{
		//Update Event
		gEvent.UpdateEvent();

		///////////////////////////////////////////////
		gFPS.StartFrame(); //for FPS
		///////////////////////////////////////////////

		//Update Field
		{
			std::lock_guard<std::mutex> lg(mtx);
			gField.UpdateField();
		}

		///////////////////////////////////////////////
		gFPS.FinishFrame(); //for FPS
		///////////////////////////////////////////////

		///////////////////////////////////////////////
		gFPS.ProcessingFPS(window); //for FPS
		///////////////////////////////////////////////
		
		//std::cerr << "FPS: "<< gFPS.fps.getFPS() << std::endl;

		//show
		window.display();
	}

	{
		//exit
		window.clear(sf::Color(0, 0, 0));
		window.display();
		sf::sleep(sf::milliseconds(10000));
		std::cerr << "\a";
	}

	return;
}
