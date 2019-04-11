#include "field.h"
#include "general.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

void FixedValue(int& value, const int i, const int j)
{
	(i == 12 && j == 5) || (i == 11 && j == 5) || (i == 11 && j == 6) || (i == 12 && j == 7) || (i == 11 && j == 7) ? value = 0 : 0;
	//todo spawn point
	//todo flag point
	return;
}

auto TakeTape = [&](std::string tape)
{
	if (tape == "Empty")
		return 0;
	if (tape == "Brick")
		return 10;
	if (tape == "Brick_Right")
		return 11;
	if (tape == "Brick_Down")
		return 12;
	if (tape == "Brick_Left")
		return 13;
	if (tape == "Brick_Up")
		return 14;
	if (tape == "Steel")
		return 20;
	if (tape == "Steel_Right")
		return 21;
	if (tape == "Steel_Down")
		return 22;
	if (tape == "Steel_Left")
		return 23;
	if (tape == "Steel_Up")
		return 24;
	if (tape == "Trees")
		return 30;
	if (tape == "Water")
		return 40;
	if (tape == "Ice")
		return 50;

	//add
	if (tape == "Brick_Right_Up_Mini")
		return 1114;
	if (tape == "Brick_Right_Down_Mini")
		return 1112;
	if (tape == "Brick_Left_Up_Mini")
		return 1314;
	if (tape == "Brick_Left_Down_Mini")
		return 1312;

	if (tape == "Steel_Right_Up_Mini")
		return 2124;
	if (tape == "Steel_Right_Down_Mini")
		return 2122;
	if (tape == "Steel_Left_Up_Mini")
		return 2324;
	if (tape == "Steel_Left_Down_Mini")
		return 2322;

	return 0;
};

//init field (parameters: start position, size, color)
void GameField::FillField()
{
	field.setPosition(32, 16);
	field.setSize(sf::Vector2f(SizeField, SizeField));
	field.setFillColor(sf::Color::Black);
	return;
}

//fill the array with data from the level file
void GameField::FillMap()
{
	std::string FILENAME("");

	switch (p_level) {
		case 1: FILENAME = "data/levels/level1"; break;
	}

	std::ifstream infile(FILENAME.c_str());
	char s_level_name[16];
	char s_index[8]; char s_tape[16];
	int h_value, indI, indJ;
	infile >> s_level_name;
	while (infile >> s_index >> s_tape)
	{
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
		h_value = TakeTape(std::string(s_tape));

		FixedValue(h_value, indI, indJ);

		map.SetValueMap(h_value, indI, indJ);
	}

	return;
}

//loading map from array
void GameField::LoadMap()
{
	//generate level
	int level[] =
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0
	};

	//find the number of cells by the width and height of the map
	const sf::Vector2u tileSize = sf::Vector2u(16, 16);
	unsigned int cellW = texture.getSize().x / tileSize.x;
	unsigned int cellH = texture.getSize().y / tileSize.y;

	//convert the value on the map according to the texture
	//write value to level[]
	int value, index(0);
	for (int i(0); i < sizeMap; i++) {
		for (int j(0); j < sizeMap; j++) {
			value = map.GetValueMap(i, j);
			switch (value)
			{
			case 10: case 11: case 12: case 13: case 14:
				level[index] = value + BrickConst; index++; break;
			case 20: case 21: case 22: case 23: case 24:
				level[index] = value + SteelConst; index++; break;
			case 30:
				level[index] = value + TreesConst; index++; break;
			case 40:
				level[index] = value + WaterConst; index++; break;
			case 50:
				level[index] = value + IceConst; index++; break;
			default:
				level[index] = cellW * cellH - 1; //last cell
				index++; break;
			}
		}
	}

	//loading tiles from texture
	tMap.load(texture, level);

	//setting the position identical to the field
	tMap.setPosition(sf::Vector2f(field.getPosition()));

	return;
}

//creating blocks
void GameField::CreateBlocks()
{
	Block blockObj(texture);
	std::vector<Block>::iterator it;
	for (int i(0); i < sizeMap; i++) {
		for (int j(0); j < sizeMap; j++) {
			block.push_back(blockObj);
			if (map.GetValueMap(i, j) == 0)
				continue;
			else {
				it = block.end() - 1;
				ReadMap(it, i, j);
			}
		}
	}
	return;
}

void GameField::ReadMap(std::vector<Block>::iterator& it, const int i, const int j)
{
	sf::Vector2f pos = map.TakeCoord(i, j);
	(*it).loadBlock(map.GetValueMap(i, j));
	(*it).setPosObj(pos.x, pos.y);
	(*it).setPosFrame(pos.x, pos.y);

	(*it).loadParamPartsBrick(partsBrickVec, pbmap);
	return;
}

void GameField::InitOutside()
{
	const float X1(field.getPosition().x);
	const float Y1(field.getPosition().y);
	const float X2(field.getPosition().x + field.getSize().x);
	const float Y2(field.getPosition().y + field.getSize().y);

	outsideUP.setPosition(sf::Vector2f(X1 - 32, Y1 - 16));
	outsideUP.setSize(sf::Vector2f(X2 + 32, Y1 - 1));
	outsideDOWN.setPosition(sf::Vector2f(X1 - 32, Y2 + 1));
	outsideDOWN.setSize(sf::Vector2f(X2 + 32, Y2 + 16));
	outsideLEFT.setPosition(sf::Vector2f(X1 - 32, Y1 - 16));
	outsideLEFT.setSize(sf::Vector2f(X1 - 1, Y2 + 16));
	outsideRIGHT.setPosition(sf::Vector2f(X2 + 1, Y1 - 16));
	outsideRIGHT.setSize(sf::Vector2f(X2 + 32, Y2 + 16));

	//set color
	outsideUP.setFillColor(sf::Color(124, 124, 124, 50));
	outsideUP.setOutlineThickness(0.5f);
	outsideUP.setOutlineColor(sf::Color::Red);
	outsideDOWN.setFillColor(sf::Color(124, 124, 124, 50));
	outsideDOWN.setOutlineThickness(0.5f);
	outsideDOWN.setOutlineColor(sf::Color::Red);
	outsideLEFT.setFillColor(sf::Color(124, 124, 124, 50));
	outsideLEFT.setOutlineThickness(0.5f);
	outsideLEFT.setOutlineColor(sf::Color::Red);
	outsideRIGHT.setFillColor(sf::Color(124, 124, 124, 50));
	outsideRIGHT.setOutlineThickness(0.5f);
	outsideRIGHT.setOutlineColor(sf::Color::Red);

	return;
}

//moore
void GameField::CreateMoore(const char* block)
{
	moore.size() > 0 ? moore.clear() : 0;

	char* blockName[5];
	if (block == "steel") {
		blockName[0] = "Steel_Right"; blockName[1] = "Steel_Right_Down_Mini"; blockName[2] = "Steel_Down"; blockName[3] = "Steel_Left"; blockName[4] = "Steel_Left_Down_Mini";
	}
	else {
		blockName[0] = "Brick_Right"; blockName[1] = "Brick_Right_Down_Mini"; blockName[2] = "Brick_Down"; blockName[3] = "Brick_Left"; blockName[4] = "Brick_Left_Down_Mini";
	}

	struct Pos
	{
		int value;
		int i, j;
	} pos[]{
		{ TakeTape(blockName[0]), 12, 5 },
		{ TakeTape(blockName[1]), 11, 5 },
		{ TakeTape(blockName[2]), 11, 6 },
		{ TakeTape(blockName[3]), 12, 7 },
		{ TakeTape(blockName[4]), 11, 7 }
	};

	Block blockObj(texture);
	moore.push_back(blockObj); //lower left, 0
	moore.push_back(blockObj); //upper left, 1
	moore.push_back(blockObj); //central, 2
	moore.push_back(blockObj); //lower right, 3
	moore.push_back(blockObj); //upper right, 4

	int element(0);
	for (auto& moore : moore)
	{
		InitMoore(moore, pos[element].i, pos[element].j, pos[element].value);
		element++;
	}

	return;
}

void GameField::InitMoore(Block& block, const int i, const int j, const int value)
{
	sf::Vector2f pos = map.TakeCoord(i, j);
	block.loadBlock(value);
	block.setPosObj(pos.x, pos.y);
	block.setPosFrame(pos.x, pos.y);

	block.loadParamPartsBrick(partsBrickVec, pbmap);

	return;
}
