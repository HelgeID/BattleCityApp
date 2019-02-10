#ifndef SCREEN_H
#define SCREEN_H

#include <SFML\Graphics.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <mutex>
#include "actor.hpp"
#include "tank.hpp"
#include "block.hpp"
#include "block_spawn.hpp"
#include "bullet.hpp"
#include "map.h"
#include "tile_map.h"
#include "part_bricks_map.h"
#include "animation_game.h"

//test
#include <iostream>

extern std::mutex mtx;

class GameField
{
	sf::RenderWindow &window;
	sf::Texture &texture;
	sf::Clock clock;
	float time;
	struct CR
	{
		int cr_a{ 0 }, cr_b{ 0 }, cr_c{ 0 }, cr_d{ 0 }, cr_e{ 0 }, cr_f{ 0 }, cr_g{ 0 }, cr_h{ 0 };

		void operator+=(const int value)
		{
			cr_a += value;
			cr_b += value;
			cr_c += value;
			cr_d += value;
			cr_e += value;
			cr_f += value;
			cr_g += value;
			cr_h += value;
		}

	} cr; //coef_reload

	sf::Clock clock_firstPlayer, clock_secondPlayer;
	sf::Time time_firstPlayer, time_secondPlayer;

	void UpdateTime();
	void UpdateCoefReload();
	void UpdateDirectionTanks();

	sf::RectangleShape field, outsideUP, outsideDOWN, outsideLEFT, outsideRIGHT;
	Map map;
	TileMap tmap;
	Part_Bricks_Map pbmap;
	void FillField();
	void FillMap();
	void LoadMap();
	void ReadMap(std::vector<Block>::iterator&, const int, const int);
	void InitOutside();
	void DrawField();
	void DrawMap();
	void DrawBrickDamage();

	std::vector<Block> block;
	std::vector<sf::RectangleShape> partsBrickVec;
	void CreateBlocks();
	void DrawBlocks();

	std::vector<Block> moore;
	void CreateMoore(const char* block = "brick");
	void InitMoore(Block&, const int, const int, const int);
	void DrawMoore();

	std::vector<Tank> tank;
	void CreateTanks();
	void CreateTank(const sf::Vector2f);
	void ReloadTank(Tank&, const sf::Vector2f);
	void DrawTanks();
	void DrawTank(Tank&);
	void MoveTank(Tank&, float);
	void MoveTank(const Direction, Tank&, float);
	void ControlTank_onFrame(Tank&);
	void CheckTankBang(const int);
	friend void LAUNCHING_TANKS(GameField*);
	friend void ON_TANK(GameField*);
	friend bool CONTROL_CollisionTanksBS(GameField*);
	friend bool CONTROL_CollisionTanksTank(GameField*, Tank&);
	friend bool CONTROL_CheckFinishTimeAnim(GameField*, const int);
	friend bool CONTROL_OffAllAnim(GameField*);

	struct AnimTank
	{
		std::unique_ptr<AnimBirth> tankBirth{ nullptr };
		std::unique_ptr<AnimSkin> tankSkin{ nullptr };
	} tankAnimArr[4];
	void CreateAnimBirth(const sf::Vector2f, const int);

	std::array<Bullet*, 6> bulletArr = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	std::shared_ptr<AnimBoom> bulletBoom[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	std::shared_ptr<AnimBoom> tankBoom[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	void CreateAnimBoom(const sf::Vector2f, const char*);

	template <typename T>
	void CreateBullet(T&, sf::Vector2f);
	void CreateBullet(Tank&);
	void CreateBullet(Player&);
	void DrawBullets();

	std::vector<RecShape> vecRecShape;
	template<typename T> void FormArrayCells(T **, const sf::Vector2i, const sf::Vector2i, RecShape&);
	template<typename T> void DisbandArrayCells(T **);

	void CheckOnCollisionFrame(Tank&); //for the tanks
	void CheckOnCollisionBlocksSpawn(Tank&); //for the tanks
	void CheckOnCollisionBlocks(Tank&, const bool fPL = false); //for the tanks
	void CheckOnCollisionTanks(Tank&, Tank&); //for the tanks
	void CheckOnCollisionFrame(Bullet&); //for the bullets
	void CheckOnCollisionBlocks(Bullet&); //for the bullets
	void CheckOnCollisionTanks(Bullet&); //for the bullets
	void CheckOnCollisionBullets(Bullet&, Bullet&); //for the bullets
	void CheckOnCollisionPlayers(Bullet&); //for the bullets
	void CheckOnCollisionFrame(Player&); //for the players
	void CheckOnCollisionBlocksSpawn(Player&, const int); //for the players
	void CheckOnCollisionBlocks(Player&); //for the players
	void CheckOnCollisionTanks(Player&); //for the players
	void CheckOnCollisionBullets(Player&); //for the players
	void CheckOnCollisionPlayers(Player&, Player&); //for the players

	void CheckOnMoore();

	//actors
	Player *firstPlayer;
	Player *secondPlayer;
	void CreateActors();
	void RestartFirstPlayer(const bool flag = false);
	void RestartSecondPlayer(const bool flag = false);
	void DrawActors();
	friend void MoveFirstPlayer(GameField&, const Direction);
	friend void MoveSecondPlayer(GameField&, const Direction);
	void MonitoringKeys();
	void CheckPlayerBang(Player&, const bool off = false);
	
	struct AnimPlayer
	{
		std::unique_ptr<AnimBirth> playerBirth{ nullptr };
		std::unique_ptr<AnimSkin> playerSkin{ nullptr };
	} firstPlayerAnim, secondPlayerAnim;

	//monitoring tanks
	class TankCollision
	{
	public:
		void MonitoringCollision(GameField&);
	private:
		void CollisionFrame(GameField&);
		void CollisionBlocksSpawn(GameField&);
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
		void CollisionPlayers(GameField&);
	} objBulletCollision;

	//monitoring players
	class PlayerCollision
	{
	public:
		void MonitoringCollision(GameField&);
	private:
		void CollisionFrame(GameField&);
		void CollisionBlocksSpawn(GameField&);
		void CollisionBlocks(GameField&);
		void CollisionTanks(GameField&);
		void CollisionBullets(GameField&);
		void CollisionPlayers(GameField&);
	} objPlayerCollision;

	//checking
	class Checking
	{
	public:
		void CheckingShootingBullets(GameField&);
		void CheckingSkin(GameField&);
	} objChecking;

	//destroy objects
	template<class T, class I, class = typename std::enable_if<std::is_integral<I>::value>::type>
	void RemovalObj(std::vector<T> &obj, I index)
	{
		//std::vector<T> used to create objects
		const auto &iter = obj.cbegin() + index;
		obj.erase(iter);
		return;
	}

	void MonitoringAnim(const AnimBirth*);
	void MonitoringAnim(const AnimSkin*);
	void MonitoringAnim(const AnimBoom*);

	void DrawAnimBirth();
	void DrawAnimSkin();
	void DrawAnimBoom();

	std::unique_ptr<BlockSpawn> l_BS{ nullptr };
	std::unique_ptr<BlockSpawn> r_BS{ nullptr };
	std::unique_ptr<BlockSpawn> c_BS{ nullptr };
	void CreatePointsSpawnEnemy();

public:
	explicit GameField(sf::RenderWindow&, sf::Texture&);
	~GameField();

	void UpdateField();
};

#endif
