#include "field.h"
#include "general.hpp"
#include <random>

void GameField::ShootingBullets::MonitoringShootingBullets(GameField& gField)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(4, 8); //results between 4 and 8 inclusive
	int random(dist(gen)); //random generation

	std::for_each(gField.tank.begin(), gField.tank.end(), [&](Tank &tank)
	{
		if (!tank.optTankShooting.bulletActivFlag && tank.optTankShooting.clockTank.getElapsedTime().asSeconds() > random) {
			gField.CreateBullet(tank, sf::Vector2f(0.f, 0.f));
			for (int i(0); i < 6; i++)
				gField.bulletArr[i] != nullptr ? std::cout << gField.bulletArr[i]->indexTank << "-": std::cout << "X" << "-";
			std::cout << std::endl;
			tank.optTankShooting.timeShooting = tank.optTankShooting.clockTank.restart(); //starting the timer
			random = dist(gen); //random generation
		}
	});
	return;
}
