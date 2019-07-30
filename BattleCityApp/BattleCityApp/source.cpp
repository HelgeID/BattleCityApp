//FINAL
#include <SFML\Graphics.hpp>
#include "game.h"
#include "general.hpp"

int main()
{
	// Declare and create a new render-window
	sf::RenderWindow window(sf::VideoMode(272, 240), TITLE, sf::Style::Close);

	// Create a new object of the game
	Game game(window);

	// Start menu
	{
		window.setVisible(false);
		game.GameMenu();
		window.setVisible(true);
	}
	
	// Init Params Win
	game.InitParamsWin(); //load all

	while (true) {
		// Init Params Win
		game.InitParamsWin(true); //load not all

		// Level screensavers
		game.GameStage();

		// Check window close
		if (!window.isOpen())
			break;

		// Init Params Game
		game.InitParamsGame();

		// Launch of the game
		game.GameLaunch();
	}

	return 0;
}
