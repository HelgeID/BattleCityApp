#include <SFML\Graphics.hpp>
#include "game.h"
#define TITLE "BattleCity"

int main()
{
	// Declare and create a new render-window
	sf::RenderWindow window(sf::VideoMode(272, 240), TITLE, sf::Style::Close);

	// Create a new object of the game
	Game game(window);

	// Launch of the game
	game.GameLaunch();

	return 0;
}
