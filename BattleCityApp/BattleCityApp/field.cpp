#include "field.h"

GameField::GameField(sf::RenderWindow &window, sf::Texture &texture)
	: window(window), texture(texture)
{
	CreateTanks();
}

GameField::~GameField()
{
}

void GameField::CreateTanks()
{
	Tank tankObj(texture);
	tank.push_back(tankObj);
	tank.push_back(tankObj);
	tank.push_back(tankObj);
	tank[0].loadTank(YELLOW, modA, DOWN); tank[0].setPosObj(32.f, 0.f);
	tank[1].loadTank(GREEN, modC, DOWN); tank[1].setPosObj(64.f, 0.f);
	tank[2].loadTank(RED, modF, DOWN); tank[2].setPosObj(96.f, 0.f);
}

void GameField::DrawTank(const int index)
{
	float speed = tank[index].optTank.speed;
	float time = this->time / speed;

	if (tank[index].optTank.dir == UP)
		tank[index].moveObj(0.f, -0.1f*time);
	else if (tank[index].optTank.dir == LEFT)
		tank[index].moveObj(-0.1f*time, 0.f);
	else if (tank[index].optTank.dir == DOWN)
		tank[index].moveObj(0.f, 0.1f*time);
	else if (tank[index].optTank.dir == RIGHT)
		tank[index].moveObj(0.1f*time, 0.f);

	if (coef_reload == tank[index].optTank.coef_reload)
		tank[index].reloadTank();

	window.draw(tank[index].takeObj());
}

void GameField::Monitoring()
{
	//coef_reload
	coef_reload++;
	if (coef_reload > 6)//todo max_coef_reload
		coef_reload = 0;

	return;
}

void GameField::UpdateField()
{
	window.clear();
	time = clock.getElapsedTime().asMicroseconds();

	this->DrawTank(0);
	this->DrawTank(1);
	this->DrawTank(2);

	Monitoring();
	clock.restart();
	window.display();
	return;
}
