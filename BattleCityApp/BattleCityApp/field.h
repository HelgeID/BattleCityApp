#ifndef SCREEN_H
#define SCREEN_H

#include <SFML\Graphics.hpp>
#include <vector>
#include "tank.hpp"
#include "map.h"

//test
#include <iostream>
using namespace std;

class GameField
{
	sf::RenderWindow &window;
	sf::Texture &texture;
	sf::Clock clock;
	float time;
	int coef_reload;

	Map map;
	void FillField();

	std::vector<Tank> tank;
	void CreateTanks();
	void DrawTank(Tank&);
	void MoveTank(Tank&, float);

	void Monitoring();
	void Collision(Tank&);
	void Collision(Tank&, Tank&);
	void Logic(Tank&);

public:
	explicit GameField(sf::RenderWindow &, sf::Texture &);
	~GameField();

	void UpdateField();
};

#endif
