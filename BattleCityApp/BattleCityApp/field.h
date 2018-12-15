#ifndef SCREEN_H
#define SCREEN_H

#include <SFML\Graphics.hpp>
#include <vector>
#include "tank.hpp"
#include "block.hpp"
#include "bullet.hpp"
#include "map.h"
#include "tile_map.h"
#include "part_bricks_map.h"

//test
#include <iostream>

class GameField
{
	sf::RenderWindow &window;
	sf::Texture &texture;
	sf::Clock clock;
	float time;
	int coef_reload;

	sf::RectangleShape field;
	Map map;
	TileMap tmap;
	Part_Bricks_Map pbmap;
	void FillField();
	void FillMap();
	void LoadMap();
	void ReadMap(std::vector<Block>::iterator&, const int, const int);
	void DrawField();
	void DrawMap();
	void DrawBrickDamage();

	std::vector<Block> block;
	std::vector<sf::RectangleShape> partsBrickVec;
	void CreateBlocks();
	void DrawBlocks();

	std::vector<Tank> tank;
	void CreateTanks();
	void DrawTank(Tank&);
	void MoveTank(Tank&, float);

	Bullet *bulletArr[4] = { nullptr, nullptr, nullptr, nullptr };
	void CreateBullet(Tank&, sf::Vector2f);
	void CreateBullet(Tank&);
	void DrawBullets();

	void Monitoring();
	void CollisionFrame(Tank&);
	void CollisionBlocks(Tank&);
	void CollisionTanks(Tank&, Tank&);
	void CollisionBullets();

public:
	explicit GameField(sf::RenderWindow&, sf::Texture&);
	~GameField();

	void UpdateField();
};

#endif
