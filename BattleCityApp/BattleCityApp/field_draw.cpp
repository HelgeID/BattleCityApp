#include "field.h"
#include "general.hpp"

//----------------------------------------------------
//-- Update the game loop
//----------------------------------------------------

//drawing tDynamic
void GameField::DrawDynamicElements()
{
	if (p_showframe == true)
		return;

	window.draw(tDynamic);
	return;
}

//drawing a field, a black square the size of 208x208 px
void GameField::DrawField()
{
	window.draw(field);
	return;
}

//drawing a loaded map (13x13 cells), "13x16=208"
void GameField::DrawMap()
{
	(!p_showframe) ? window.draw(tMap) : DrawBlocks();
	return;
}

//drawing the destruction of blocks
void GameField::DrawBrickDamage()
{
	window.draw(pbmap);
	return;
}

//drawing game blocks
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
		(p_showframe) ? window.draw(block.frame) : block.drawBlock(window);
	return;
}

//drawing players
void GameField::DrawActors()
{
	if (firstPlayer->Presence())
		(p_showframe) ? window.draw(firstPlayer->frame) : window.draw(firstPlayer->takeObj());
	if (secondPlayer->Presence())
		(p_showframe) ? window.draw(secondPlayer->frame) : window.draw(secondPlayer->takeObj());
	return;
}

//drawing a tank(enemy)
void GameField::DrawTank(Tank &tank)
{
	if (!tank.isTank())
		return;

	if (p_showframe)
		window.draw(tank.frame);
	else
	{
		window.draw(tank.takeObj());
	}
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
	const size_t bulletArrSize = this->bulletArr.size();
	for (size_t indxBullet(0); indxBullet < bulletArrSize; ++indxBullet) {
		if (this->bulletArr[indxBullet] != nullptr) {
			if (p_showframe)
				window.draw(this->bulletArr[indxBullet]->frame);
			else {
				window.draw(this->bulletArr[indxBullet]->takeObj());
			}
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
	for (size_t indexBulletBoom(0); indexBulletBoom < bulletBoomSize; ++indexBulletBoom) {
		if (bulletBoom[indexBulletBoom] != nullptr)
			window.draw(bulletBoom[indexBulletBoom]->TakeAnim());
	}

	const size_t tankBoomSize = sizeof(this->tankBoom) / sizeof(*this->tankBoom);
	for (size_t indexTankBoom(0); indexTankBoom < tankBoomSize; ++indexTankBoom) {
		if (tankBoom[indexTankBoom] != nullptr)
			window.draw(tankBoom[indexTankBoom]->TakeAnim());
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
