#ifndef SCREEN_H
#define SCREEN_H

#include <SFML\Graphics.hpp>
#include <vector>
#include "tank.hpp"

class GameField
{
	sf::RenderWindow &window;
	sf::Texture &texture;
	sf::Clock clock;
	float time;
	int coef_reload;

	std::vector<Tank> tank;
	void CreateTanks();
	void DrawTank(Tank&);

	void Monitoring();
	void Collision(Tank&);
	void Collision(Tank&, Tank&);

public:
	explicit GameField(sf::RenderWindow &, sf::Texture &);
	~GameField();

	void UpdateField();
};

#endif
