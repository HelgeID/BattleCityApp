#include <SFML\Graphics.hpp>
#include "game.h"
#define TITLE "BattleCity"

//test
#include "tank.hpp"
#include <iostream>

int main()
{
	// Declare and create a new render-window
	//sf::RenderWindow window(sf::VideoMode(200, 200), TITLE, sf::Style::Close);

	// Create a new object of the game
	//Game game(window);

	// Launch of the game
	//game.GameLaunch();

	//test
	sf::RenderWindow window(sf::VideoMode(200, 200), TITLE, sf::Style::Close);
	window.setFramerateLimit(30);
	sf::Texture texture;
	texture.loadFromFile("data/graphics/texture/texture.png", sf::IntRect(0, 0, 400, 256));

	Tank tank(texture);
	tank.loadTank(YELLOW, modF, UP);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		tank.reloadTank();
		window.clear();
		window.draw(tank.takeObj());
		window.display();
		sf::sleep(sf::milliseconds(50));

		tank.reloadTank();
		window.clear();
		window.draw(tank.takeObj());
		window.display();
		sf::sleep(sf::milliseconds(50));
	}
	return 0;
}
