#include "field.h"
#include "general.hpp"
#include <random>

void GameField::Checking::CheckingShootingBullets(GameField& gField)
{
	if (blocking_firing)
		return;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(4, 8); //results between 4 and 8 inclusive
	int random(dist(gen)); //random generation

	std::for_each(gField.tank.begin(), gField.tank.end(), [&](Tank &tank)
	{
		if (!tank.isTank() || tank.sleepTank())
			;
		else {
			if (!tank.optTankShooting.bulletActivFlag && tank.optTankShooting.clockTank.getElapsedTime().asSeconds() > random) {
				gField.CreateBullet(tank, sf::Vector2f(0.f, 0.f));

				//{
				//	for (int i(0); i < 8; i++)
				//		gField.bulletArr[i] != nullptr ? std::cerr << gField.bulletArr[i]->indexTank << "-" : std::cerr << "X" << "-";
				//	std::cerr << std::endl;
				//}

				tank.optTankShooting.timeShooting = tank.optTankShooting.clockTank.restart(); //starting the timer
				random = dist(gen); //random generation
			}
		}
	});
	return;
}

void GameField::Checking::CheckingSkin(GameField& gField)
{
	auto Checking = [&](AnimPlayer& playerAnim, Player* player)
	{
		if (player->GetSkin() && playerAnim.playerSkin == nullptr) {
			sf::Vector2f posPlayer = player->getPosObj();
			std::unique_ptr<AnimSkin> anim(new AnimSkin(gField.texture, posPlayer));
			playerAnim.playerSkin = std::move(anim);
		}
	};

	Checking(gField.firstPlayerAnim, gField.firstPlayer);
	Checking(gField.secondPlayerAnim, gField.secondPlayer);

	return;
}
