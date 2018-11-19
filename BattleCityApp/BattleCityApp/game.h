#ifndef GAME_H
#define GAME_H

#include <SFML\Graphics.hpp>

class Game
{
	sf::RenderWindow &window;
	sf::Texture* texture;

public:
	explicit Game(sf::RenderWindow &);
	~Game();

	void GameLaunch();

private:
	bool loadTexture();
	bool loadIcon();
};

#endif
