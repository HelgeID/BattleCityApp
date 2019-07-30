//FINAL
#include "game.h"
#include "general.hpp"
#include "events.h"
#include "field.h"

std::mutex mtx;

#include "ui_msg.hpp" //for Stage MSG
#include <conio.h> //for Console Menu

auto Console_Note = []() {
	std::cerr << " ___ ___ ___        ___ _    _ ___ ___ _  _ ___ ___ _  _ _          _ ___ ___ _ " << std::endl;
	std::cerr << "|___|___|___|_    _|___|_|_ |_|___|___|_||_|___|___|_||_|_|        |_|___|___|_|" << std::endl;
	std::cerr << "|_|_|     |___| _|___| |_|_|_   |___|        |___|    |___|        |___|        " << std::endl;
	std::cerr << "|___|_ ___|_|_||___|     |_|_|  |_|_|        |_|_|    |_|_|        |_|_|___ _   " << std::endl;
	std::cerr << "|_|___|___|_|_ |_|_|___ _|___|  |___|        |___|    |___|        |___|___|_|  " << std::endl;
	std::cerr << "|___|     |_|_||___|___|___|_|  |_|_|        |_|_|    |_|_|        |_|_|        " << std::endl;
	std::cerr << "|_|_|___ _|___||_|_|     |___|  |___|        |___|    |___|_ ___ _ |___|_ ___ _ " << std::endl;
	std::cerr << "|___|___|___|  |___|     |_|_|  |_|_|        |_|_|    |_|___|___|_||_|___|___|_|" << std::endl;
	std::cerr << "                                                                                " << std::endl;
	std::cerr << "                 _ ___ _     ___ ___ ___   ___ ___ ___   ___     ___            " << std::endl;
	std::cerr << "               _|_|___|_|_  |___|___|___| |___|___|___| |___|   |___|           " << std::endl;
	std::cerr << "             _|_|_|   |_|_|     |_|_|         |_|_|     |_|_|   |_|_|           " << std::endl;
	std::cerr << "            |_|_|               |___|         |___|     |___|_ _|___|           " << std::endl;
	std::cerr << "            |___|               |_|_|         |_|_|       |___|___|             " << std::endl;
	std::cerr << "            |_|_|_     _ _      |___|         |___|         |___|               " << std::endl;
	std::cerr << "              |_|_|___|_|_|  ___|_|_|___      |_|_|         |_|_|               " << std::endl;
	std::cerr << "                |_|___|_|   |___|___|___|     |___|         |___|               " << std::endl;
};

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

void Game::InitParamsGame()
{
	gameover = false; //default =>FALSE
	level_finish = false; //default =>FALSE
	level_exit = false; //default =>FALSE
	pause = false; //default =>FALSE
	no_close = true; //default =>TRUE

	f_f = false;

	//show frame
	//p_showframe = true;

	//show fps
	//p_showfps = true;

	return;
}

void Game::InitParamsWin(bool restart)
{
	winStyle(window, sf::Style::Close);//Call WinStyle
	window.setTitle(TITLE); //Title

	if (restart == false)
	{
		sf::Vector2i pos{ 0, 0 };
		TakeConsolePos(&pos.x, &pos.y);
		window.setPosition(pos);

		//initialization zoom
		zoomOn();
	}

	return;
}

//loader "console menu"
void Game::GameMenu()
{
	system("mode con cols=80 lines=40");
	system("color F4");
	system("title BattleCity");

	auto BattleCity_Console_Choice_Number_Players = []() {
		system("cls");//screen cleaning
		Console_Note();//BattleCity
		std::cerr << "Please press keys: 1 or 2" << std::endl;
		std::cerr << "1 - for one player" << std::endl;
		std::cerr << "2 - for two players" << std::endl;
		std::cerr << std::endl << ">>";

		bool repeat_flag(true);
		while (repeat_flag) {
			sf::sleep(sf::milliseconds(200));
			if (_kbhit()) {
				switch (_getch())
				{
				case '1': std::cerr << '1' << std::endl; repeat_flag = false; p_player = 1; break;
				case '2': std::cerr << '2' << std::endl; repeat_flag = false; p_player = 2; break;
				}
			}
		}
	};

	auto BattleCity_Console_Choice_Level = []() {
		system("cls");//screen cleaning

		char buff[BUFSIZ];
		char temp;
		int number;
		bool repeat_flag = true;

		while (repeat_flag) {
			system("cls");//screen cleaning
			Console_Note();//BattleCity
			fputs("Please Enter Level: 1 - 35\n\n>>", stdout);
			fflush(stdout);
			bool input_control_flag(false);

			if (fgets(buff, BUFSIZ, stdin) == NULL) {
				if (!ferror(stdin)) {
					//fputs("No input available\n", stderr);
					exit(EXIT_FAILURE);
				}
				else {
					perror("stdin");
					clearerr(stdin);
					continue;
				}
			}
			//std::cerr << "input buff" << buff << std::endl;

			for (char ch : buff) {
				if (ch == '\0' || ch == '\n')
					break;

				if (!isdigit(ch))
					input_control_flag = true;
			}

			if (sscanf_s(buff, "%d %c\n", &number, &temp) != 1 || input_control_flag || number > 35) {
				//fputs("Incorrect input\n", stderr);
				continue;
			}

			repeat_flag = !repeat_flag;
			_flushall();
		}

		p_level = number;
	};

	//-------
	//start--
	//-------
	BattleCity_Console_Choice_Number_Players();
	BattleCity_Console_Choice_Level();
	if (IsConsoleVisible())
		HideConsole();
	return;
}

//loader "stage screen"
void Game::GameStage()
{
	UIStageMSG uiStageMSG(*texture);

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

		window.draw(uiStageMSG);
		window.display();
	}

	sf::sleep(sf::milliseconds(2500));
	return;
}

//loader "game"
void Game::GameLaunch()
{
	// parameters \\
	----------------
	//undying_enemy = true;
	//undying_players = true;
	//undying_hit_on_player = true;
	//blocking_firing = true;

	//undying_emblem_hit_by_tank = true;
	//undying_emblem_boom_by_tank = true;
	//undying_emblem_absence_players = true;

	// Game PTRS
	GameEvent *gEvent(nullptr);
	GameField *gField(nullptr);
	GameFPS *gFPS(nullptr);


	// initialization objects
	{
		gEvent = new GameEvent(window);
		gField = new GameField(window, *texture);
		gFPS = new GameFPS();
	}

	//winStyle(window, sf::Style::Close); //Call WinStyle
	//window.setTitle(TITLE);

	// update
	while (window.isOpen() && !level_exit)
	{
		//Update Event
		gEvent->UpdateEvent<Game>(this);

		///////////////////////////////////////////////
		gFPS->StartFrame(); //for FPS
		///////////////////////////////////////////////

		//Update Field
		{
			std::lock_guard<std::mutex> lg(mtx);
			gField->UpdateField();
		}

		///////////////////////////////////////////////
		gFPS->FinishFrame(); //for FPS
		///////////////////////////////////////////////

		///////////////////////////////////////////////
		gFPS->ProcessingFPS(window); //for FPS
		///////////////////////////////////////////////
		
		//show
		window.display();
	}

	//clear display
	//{
		//window.clear(sf::Color(0, 0, 0));
		//window.display();
		//sf::sleep(sf::milliseconds(3000));
		//std::cerr << "\a";
	//}

	// dell objects
	{
		delete gEvent;
		delete gField;
		delete gFPS;
	}
	return;
}
