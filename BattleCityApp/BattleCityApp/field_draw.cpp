#include "field.h"
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

//drawing players
void GameField::DrawActors()
{
	bool *keyArray(nullptr);

	auto fun = [=](Player& player, bool keyArray[])
	{
		if (coef_reload == player.optTank.coef_reload && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3]))
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

	std::for_each(tank.begin(), tank.end(), [&](Tank &tank) { DrawTank(tank); });
	return;
}

//drawings a bullets
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

//drawings a birth anim
void GameField::DrawAnimBirth()
{
	if (firstPlayerBirth != nullptr && !firstPlayerBirth->FinishTime())
		window.draw(firstPlayerBirth->TakeAnim());
	if (secondPlayerBirth != nullptr && !secondPlayerBirth->FinishTime())
		window.draw(secondPlayerBirth->TakeAnim());
	return;
}

//drawings a skin anim
void GameField::DrawAnimSkin()
{

	return;
}

//drawings a bullets boom anim
void GameField::DrawAnimBoom()
{
	const size_t bulletBoomSize = sizeof(this->bulletBoom) / sizeof(*this->bulletBoom);
	for (int i(0); i < bulletBoomSize; ++i) {
		if (bulletBoom[i] != nullptr)
			window.draw(bulletBoom[i]->TakeAnim());
	}
	return;
}
