﻿#include "field.h"
#include "general.hpp"

//----------------------------------------------------
//-- Update the game loop
//----------------------------------------------------

//drawing a field, a black square the size of 208x208 px
void GameField::DrawField()
{
	window.draw(field);
	return;
}

//drawing a loaded map (13x13 px), "13x16=208"
void GameField::DrawMap()
{
	(!p_showframe) ? window.draw(tmap) : DrawBlocks();
	return;
}

//drawing the destruction of blocks
void GameField::DrawBrickDamage()
{
	window.draw(pbmap);
	return;
}

//drawing game blocks
/*
!!!
not used, because the draw function eats a lot of CPU time in a loop
an alternative - to draw once with vertex ARR
!!!
*/
void GameField::DrawBlocks()
{
	int index(0);
	for (int i(0); i < sizeMap; i++) {
		for (int j(0); j < sizeMap; j++) {
			if (map.GetValueMap(i, j) == 0) {
				index++;
				continue;
			}
			else {
				if (index < block.size()) {
					if (p_showframe)
						window.draw(block[index++].frame);
					else
						block[index++].drawBlock(window);
				}
			}
		}
	}
	return;
}

//drawing moore
void GameField::DrawMoore()
{
	if (moore.size() > 0)
		;
	else
		return;

	for each(Block block in moore)
		p_showframe ? window.draw(block.frame) : block.drawBlock(window);
	return;
}

//drawing players
void GameField::DrawActors()
{
	bool *keyArray(nullptr);

	auto fun = [=](Player& player, bool keyArray[])
	{
		if (
			(cr.cr_aPlayer == player.optTank.coef_reload && player.optTank.mod == playerModA && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_bPlayer == player.optTank.coef_reload && player.optTank.mod == playerModB && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_cPlayer == player.optTank.coef_reload && player.optTank.mod == playerModC && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_dPlayer == player.optTank.coef_reload && player.optTank.mod == playerModD && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_aEnemy == player.optTank.coef_reload && player.optTank.mod == enemyModA && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_bEnemy == player.optTank.coef_reload && player.optTank.mod == enemyModB && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_cEnemy == player.optTank.coef_reload && player.optTank.mod == enemyModC && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_dEnemy == player.optTank.coef_reload && player.optTank.mod == enemyModD && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])))
				player.reloadTank();

		//frames position
		player.setPosFrame(player.takeObj().getPosition().x, player.takeObj().getPosition().y);

		//draw
		p_showframe ? window.draw(player.frame) : window.draw(player.takeObj());

		delete[] keyArray;
		keyArray = nullptr;
	};

	if (firstPlayer->Presence()) {
		keyArray = new bool[4] {Key_A, Key_D, Key_W, Key_S};
		fun(*firstPlayer, keyArray);
	}

	if (secondPlayer->Presence()) {
		keyArray = new bool[4] {Key_Left, Key_Right, Key_Up, Key_Down};
		fun(*secondPlayer, keyArray);
	}

	return;
}

//drawing a tank(enemy)
void GameField::DrawTank(Tank &tank)
{
	if (!tank.isTank())
		return;

	const float step_speed = tank.optTank.step_speed;

	if (tank.sleepTank() || step_speed == 0.f) {
		p_showframe ? window.draw(tank.frame) : window.draw(tank.takeObj());
		return;
	}
	
	if (tank.optTank.dir == UP)
		tank.moveObj(0.f, -step_speed);
	else if (tank.optTank.dir == LEFT)
		tank.moveObj(-step_speed, 0.f);
	else if (tank.optTank.dir == DOWN)
		tank.moveObj(0.f, step_speed);
	else if (tank.optTank.dir == RIGHT)
		tank.moveObj(step_speed, 0.f);

	if (
		(cr.cr_aPlayer == tank.optTank.coef_reload && tank.optTank.mod == playerModA) ||
		(cr.cr_bPlayer == tank.optTank.coef_reload && tank.optTank.mod == playerModB) ||
		(cr.cr_cPlayer == tank.optTank.coef_reload && tank.optTank.mod == playerModC) ||
		(cr.cr_dPlayer == tank.optTank.coef_reload && tank.optTank.mod == playerModD) ||
		(cr.cr_aEnemy == tank.optTank.coef_reload && tank.optTank.mod == enemyModA) ||
		(cr.cr_bEnemy == tank.optTank.coef_reload && tank.optTank.mod == enemyModB) ||
		(cr.cr_cEnemy == tank.optTank.coef_reload && tank.optTank.mod == enemyModC) ||
		(cr.cr_dEnemy == tank.optTank.coef_reload && tank.optTank.mod == enemyModD))
			tank.reloadTank();
	
	//frames position
	tank.setPosFrame(tank.takeObj().getPosition().x, tank.takeObj().getPosition().y);

	if (p_showframe)
		window.draw(tank.frame);
	else
		window.draw(tank.takeObj());
	return;
}

//drawing all tanks(enemy)
void GameField::DrawTanks()
{
	if (tank.size() == 0)
		return;

	std::for_each(tank.begin(), tank.end(), [&](Tank &tank) { tank.isTank() ? DrawTank(tank) : NULL; });
	return;
}

//drawing a bullets
void GameField::DrawBullets()
{
	float speed = BulletSpeed;
	float time = this->time / speed;

	const size_t bulletArrSize = this->bulletArr.size();
	for (int i(0); i < bulletArrSize; ++i) {
		if (this->bulletArr[i] != nullptr) {
			this->bulletArr[i]->move(time);
			this->bulletArr[i]->setPosFrame(
				this->bulletArr[i]->takeObj().getPosition().x, 
				this->bulletArr[i]->takeObj().getPosition().y
			);
			if (p_showframe)
				window.draw(this->bulletArr[i]->frame);
			else
				window.draw(this->bulletArr[i]->takeObj());
		}
	}
	return;
}

//drawing a birth anim
void GameField::DrawAnimBirth()
{
	if (firstPlayerAnim.playerBirth != nullptr)
		if (!firstPlayerAnim.playerBirth->FinishTime())
			window.draw(firstPlayerAnim.playerBirth->TakeAnim());
	if (secondPlayerAnim.playerBirth != nullptr)
		if (!secondPlayerAnim.playerBirth->FinishTime())
			window.draw(secondPlayerAnim.playerBirth->TakeAnim());

	tankAnimArr[0].tankBirth == nullptr ? NULL : window.draw(tankAnimArr[0].tankBirth->TakeAnim());
	tankAnimArr[1].tankBirth == nullptr ? NULL : window.draw(tankAnimArr[1].tankBirth->TakeAnim());
	tankAnimArr[2].tankBirth == nullptr ? NULL : window.draw(tankAnimArr[2].tankBirth->TakeAnim());
	tankAnimArr[3].tankBirth == nullptr ? NULL : window.draw(tankAnimArr[3].tankBirth->TakeAnim());
	tankAnimArr[4].tankBirth == nullptr ? NULL : window.draw(tankAnimArr[4].tankBirth->TakeAnim());
	tankAnimArr[5].tankBirth == nullptr ? NULL : window.draw(tankAnimArr[5].tankBirth->TakeAnim());
	return;
}

//drawing a skin anim
void GameField::DrawAnimSkin()
{
	if (firstPlayerAnim.playerSkin != nullptr)
		if (!firstPlayerAnim.playerSkin->FinishTime())
			window.draw(firstPlayerAnim.playerSkin->TakeAnim());
	if (secondPlayerAnim.playerSkin != nullptr)
		if (!secondPlayerAnim.playerSkin->FinishTime())
			window.draw(secondPlayerAnim.playerSkin->TakeAnim());

	tankAnimArr[0].tankSkin == nullptr ? NULL : window.draw(tankAnimArr[0].tankSkin->TakeAnim());
	tankAnimArr[1].tankSkin == nullptr ? NULL : window.draw(tankAnimArr[1].tankSkin->TakeAnim());
	tankAnimArr[2].tankSkin == nullptr ? NULL : window.draw(tankAnimArr[2].tankSkin->TakeAnim());
	tankAnimArr[3].tankSkin == nullptr ? NULL : window.draw(tankAnimArr[3].tankSkin->TakeAnim());
	tankAnimArr[4].tankSkin == nullptr ? NULL : window.draw(tankAnimArr[4].tankSkin->TakeAnim());
	tankAnimArr[5].tankSkin == nullptr ? NULL : window.draw(tankAnimArr[5].tankSkin->TakeAnim());
	return;
}

//drawing a bullets boom anim
void GameField::DrawAnimBoom()
{
	const size_t bulletBoomSize = sizeof(this->bulletBoom) / sizeof(*this->bulletBoom);
	for (int i(0); i < bulletBoomSize; ++i) {
		if (bulletBoom[i] != nullptr)
			window.draw(bulletBoom[i]->TakeAnim());
	}

	const size_t tankBoomSize = sizeof(this->tankBoom) / sizeof(*this->tankBoom);
	for (int i(0); i < tankBoomSize; ++i) {
		if (tankBoom[i] != nullptr)
			window.draw(tankBoom[i]->TakeAnim());
	}
	return;
}

//drawing a bonus
void GameField::DrawBonus()
{
	if (bonus == nullptr)
		return;

	window.draw(bonus->takeObj());
	return;
}
