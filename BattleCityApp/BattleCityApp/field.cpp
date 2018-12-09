#include "field.h"
#include "general.hpp"
#include <algorithm>

GameField::GameField(sf::RenderWindow &window, sf::Texture &texture)
	: window(window), texture(texture)
{
	window.clear(sf::Color(127, 127, 127));
	FillField();
	CreateTanks();
}

GameField::~GameField()
{
}

void GameField::FillField()
{
	sf::RectangleShape field;
	field.setPosition(16, 8);
	field.setSize(sf::Vector2f(208.0, 208.0));
	field.setFillColor(sf::Color::Black);
	window.draw(field);
	return;
}

void GameField::CreateTanks()
{
	Tank tankObj(texture);
	tank.push_back(tankObj);

	sf::Vector2f pos;
	tank[0].loadTank(YELLOW, modA, RIGHT); pos = { 16.f, 8.f };
	tank[0].setPosObj(pos.x, pos.y); map.SetValueMap(100, pos);
	tank[0].mapPos.i = map.TakeIndex(pos, 'i');
	tank[0].mapPos.j = map.TakeIndex(pos, 'j');
	return;
}

void GameField::DrawTank(Tank &tank)
{
	float speed = tank.optTank.speed;
	float time = this->time / speed;

	if (tank.optTank.dir == UP)
		tank.moveObj(0.f, -0.1f*time);
	else if (tank.optTank.dir == LEFT)
		tank.moveObj(-0.1f*time, 0.f);
	else if (tank.optTank.dir == DOWN)
		tank.moveObj(0.f, 0.1f*time);
	else if (tank.optTank.dir == RIGHT)
		tank.moveObj(0.1f*time, 0.f);

	if (coef_reload == tank.optTank.coef_reload)
		tank.reloadTank();

	window.draw(tank.takeObj());
	return;
}

void GameField::MoveTank(Tank& tank, float position)
{
	if (tank.optTank.dir == UP)
		tank.moveObj(0.f, -position);
	else if (tank.optTank.dir == LEFT)
		tank.moveObj(-position, 0.f);
	else if (tank.optTank.dir == DOWN)
		tank.moveObj(0.f, position);
	else if (tank.optTank.dir == RIGHT)
		tank.moveObj(position, 0.f);
	return;
}

void GameField::Monitoring()
{
	//coef_reload
	coef_reload++;
	if (coef_reload > 6)//todo max_coef_reload
		coef_reload = 0;

	for (auto it1 = tank.begin(); it1 != tank.end(); ++it1) {
		Collision(*it1);
		for (auto it2(it1); it2 != tank.end(); ++it2) {
			Collision(*it1, *it2);
		}
	}
	return;
}

void GameField::UpdateField()
{
	time = (float)clock.getElapsedTime().asMicroseconds();

	std::for_each(tank.begin(), tank.end(), [&](Tank &tank) { Logic(tank); });

	Monitoring();
	clock.restart();
	window.display();
	return;
}
