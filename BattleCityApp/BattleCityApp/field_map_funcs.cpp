#include "field.h"
#include "general.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

//init field (parameters: start position, size, color)
void GameField::FillField()
{
	field.setPosition(32, 16);
	field.setSize(sf::Vector2f(208.0, 208.0));
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
				level[index] = value + 6; index++; break;
			case 20: case 21: case 22: case 23: case 24:
				level[index] = value + 21; index++; break;
			default:
				level[index] = cellW * cellH - 1; //last cell
				index++; break;
			}
		}
	}

	//loading tiles from texture
	tmap.load(texture, level);

	//setting the position identical to the field
	tmap.setPosition(sf::Vector2f(field.getPosition()));

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

	(*it).offsetFrame(map.GetValueMap(i, j));
	(*it).loadParamPartsBrick();
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
