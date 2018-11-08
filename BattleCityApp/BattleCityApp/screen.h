#ifndef SCREEN_H
#define SCREEN_H

#include <SFML\Graphics.hpp>

class GameScreen
{
	sf::RenderWindow &window;

public:
	explicit GameScreen(sf::RenderWindow &);
	~GameScreen();

	void UpdateScreen();
};

#endif
