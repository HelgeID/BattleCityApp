#ifndef SCREEN_H
#define SCREEN_H

#include <SFML\Graphics.hpp>
#include "actor.hpp"
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
	void ControlTank_onFrame(Tank&);

	Bullet *bulletArr[4] = { nullptr, nullptr, nullptr, nullptr };
	void CreateBullet(Tank&, sf::Vector2f);
	void CreateBullet(Tank&);
	void DrawBullets();

	//actors
	Actor *firstPlayer;
	Actor *secondPlayer;
	void CreateActors();
	void DrawActors();
	void MonitoringKeys();

	//monitoring tanks
	class TankCollision
	{
	public:
		void MonitoringCollision(GameField&);
	private:
		void CollisionFrame(GameField&);
		void CollisionBlocks(GameField&);
		void CollisionTanks(GameField&);
	} objTankCollision;

	//monitoring bullets
	class BulletCollision
	{
	public:
		void MonitoringCollision(GameField&);
	private:
		void CollisionFrame(GameField&);
		void CollisionBlocks(GameField&);
		void CollisionTanks(GameField&);
		void CollisionBullets(GameField&);
		void CollisionActor1(GameField&);
		void CollisionActor2(GameField&);
	} objBulletCollision;

	//monitoring shooting bullets
	class ShootingBullets
	{
	public:
		void MonitoringShootingBullets(GameField&);
	} objShootingBullets;

	//destroy objects
	template<class T, class I, class = typename std::enable_if<std::is_integral<I>::value>::type>
	void RemovalObj(std::vector<T> &obj, I index)
	{
		//std::vector<T> used to create objects
		const auto &iter = obj.cbegin() + index;
		obj.erase(iter);
		return;
	}

public:
	explicit GameField(sf::RenderWindow&, sf::Texture&);
	~GameField();

	void UpdateField();
};

#endif
