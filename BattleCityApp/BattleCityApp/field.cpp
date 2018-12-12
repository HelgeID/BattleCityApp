#include "field.h"
#include "general.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

GameField::GameField(sf::RenderWindow &window, sf::Texture &texture)
	: window(window), texture(texture)
{
	window.clear(sf::Color(127, 127, 127));
	FillField();
	FillMap();
	CreateBlocks();
	CreateTanks();

	//test "draw line"
	//sf::Vector2f *posA(new sf::Vector2f()), *posB(new sf::Vector2f());
	//sf::VertexArray line(sf::Lines, 2);
	//posA->x = 0.f; posA->y = 0.f;
	//posB->x = (float)window.getSize().x;
	//posB->y = (float)window.getSize().y;
	//line[0].position = *posA; line[0].color = sf::Color::Blue;
	//line[1].position = *posB; line[1].color = sf::Color::Blue;
	//window.draw(line);
	//delete posA; delete posB;
}

GameField::~GameField()
{
}

void GameField::FillField()
{
	field.setPosition(32, 16);
	field.setSize(sf::Vector2f(208.0, 208.0));
	field.setFillColor(sf::Color::Black);
	return;
}

void GameField::FillMap()
{
	std::string FILENAME("");
	switch (p_level) {
		case 1: FILENAME = "data/levels/level1"; break;
	}

	std::ifstream infile(FILENAME.c_str());
	char s_level_name[16];
	char s_index[8]; char s_tape[8]; char s_value[8];
	int h_value, indI, indJ;
	infile >> s_level_name;
	while (infile >> s_index >> s_tape >> s_value)
	{
		std::istringstream issV(s_value);
		issV >> h_value;
		issV.clear();
		char* ptr = s_index;
		char dash(' ');
		std::string s_indI{ "" }, s_indJ{ "" };
		while (*ptr != 0) {
			if (*ptr == '-') {
				dash = *ptr++;
				continue;
			}
			dash == '-' ? s_indJ += *ptr : s_indI += *ptr;
			ptr++;
		}

		std::istringstream issI(s_indI); issI >> indI;
		std::istringstream issJ(s_indJ); issJ >> indJ;
		map.SetValueMap(h_value, indI, indJ);
	}

	return;
}

void GameField::ReadMap(std::vector<Block>::iterator& it, const int i, const int j)
{
	sf::Vector2f pos = map.TakeCoord(i, j);
	(*it).loadBlock(map.GetValueMap(i, j));
	(*it).setPosObj(pos.x, pos.y);
	(*it).setPosFrame(pos.x, pos.y);
	return;
}

void GameField::DrawField()
{
	window.draw(field);
	return;
}

void GameField::CreateBlocks()
{
	Block blockObj(texture);
	std::vector<Block>::iterator it;
	for (int i(0); i < sizeMap; i++) {
		for (int j(0); j < sizeMap; j++) {
			if (map.GetValueMap(i, j) == 0)
				continue;
			else {
				block.push_back(blockObj);
				it = block.end() - 1;
				ReadMap(it, i, j);
			}
		}
	}
	return;
}

void GameField::DrawBlocks()
{
	int index(0);
	for (int i(0); i < sizeMap; i++) {
		for (int j(0); j < sizeMap; j++) {
			if (map.GetValueMap(i, j) == 0)
				continue;
			else {
				if (index < block.size()) {
					if (p_showframe)
						window.draw(block[index++].frame);
					else
						window.draw(block[index++].takeObj());
				}
			}
		}
	}
	return;
}

void GameField::CreateTanks()
{
	Tank tankObj(texture);
	tank.push_back(tankObj);

	sf::Vector2f pos;
	tank[0].loadTank(YELLOW, modA, DOWN); pos = { 64.f, 16.f };
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

	tank.setPosFrame(tank.takeObj().getPosition().x, tank.takeObj().getPosition().y);

	if (p_showframe)
		window.draw(tank.frame);
	else
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
		CollisionFrame(*it1);
		CollisionBlocks(*it1);
		for (auto it2(it1); it2 != tank.end(); ++it2) {
			CollisionTanks(*it1, *it2);
		}
	}
	return;
}

void GameField::UpdateField()
{
	time = (float)clock.getElapsedTime().asMicroseconds();

	window.clear(sf::Color(127, 127, 127));
	DrawField();
	DrawBlocks();

	std::for_each(tank.begin(), tank.end(), [&](Tank &tank) { Logic(tank); }); //DrawTank

	Monitoring();
	clock.restart();
	window.display();
	return;
}
