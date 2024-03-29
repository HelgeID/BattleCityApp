﻿//FINAL
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
#include "bonus.hpp"
#include "emblem.hpp"
#include "map.h"
#include "tile_map_dynamic.h"
#include "part_bricks_map.h"
#include "animation_game.h"
#include "check_threads.h"

#include "msg.h"
#include "fps.h"

#include "sound.h"

//ui
#include "ui_storage_tanks.hpp"
#include "ui_number_lifes.hpp"
#include "ui_number_flags.hpp"
#include "ui_number_tanks.hpp"
#include "ui_msg.hpp"

#include "curtain.h"

//test
#include <iostream>


class GameField;

extern std::mutex mtx;

typedef void (GameField::* pToCheckOnCollision)(Tank&);
static pToCheckOnCollision CheckOnCollision;

class GameField
{
	friend void READDATAOBJ(GameField*);

	sf::RenderWindow &window;
	sf::Texture &texture;

	Sound sound;
	friend void ControlSound(GameField*, const int);

	StorageTanks storage_tanks{ texture };
	NumberLifes number_lifes_first{ texture, "first player" };
	NumberLifes number_lifes_second{ texture, "second player" };
	NumberFlags number_flags{ texture };
	NumberAllTanks numberAllTanks{ texture };
	NumberTanksForPlayer numberTanksForFirstPlayer{ texture, "first player" };
	NumberTanksForPlayer numberTanksForSecondPlayer{ texture, "second player" };

	sf::Clock clock;
	float time;
	struct CR
	{
		int cr_aPlayer{ 0 }, cr_bPlayer{ 0 }, cr_cPlayer{ 0 }, cr_dPlayer{ 0 }, cr_aEnemy{ 0 }, cr_bEnemy{ 0 }, cr_cEnemy{ 0 }, cr_dEnemy{ 0 };

		void operator+=(const int value)
		{
			cr_aPlayer += value;
			cr_bPlayer += value;
			cr_cPlayer += value;
			cr_dPlayer += value;
			cr_aEnemy += value;
			cr_bEnemy += value;
			cr_cEnemy += value;
			cr_dEnemy += value;
		}

	} cr; //coef_reload

	sf::Clock clock_firstPlayer, clock_secondPlayer;
	sf::Time time_firstPlayer, time_secondPlayer;

	void UpdateTime();
	void UpdateCoefReload();
	void UpdateDirectionTanks();

	sf::RectangleShape field, outsideUP, outsideDOWN, outsideLEFT, outsideRIGHT;
	Map map;
	TileMap tMap;
	Part_Bricks_Map pbmap;
	void LoadLevelfromPNG();
	void FillField();
	void FillMap();
	void LoadMap();
	void ReadMap(std::vector<Block>::iterator&, const int, const int);
	void InitOutside();
	void DrawField();
	void DrawMap();
	void DrawBrickDamage();

	TileDynamic tDynamic{ texture };
	void DrawDynamicElements(); // draw tDynamic

	std::vector<Block> block;
	std::vector<sf::RectangleShape> partsBrickVec;
	void CreateBlocks();
	void DrawBlocks();

	std::vector<Block> moore;
	void CreateMoore(const char* block = "brick");
	void InitMoore(Block&, const int, const int, const int);
	void DrawMoore();

	std::vector<Tank> tank;
	bool sleeptanks{ false };
	bool completion_generation_tanks{ false }; //YES - when the pointer (model selection) of the tank came to the end. It is used for the array mapOfEnemy!!!
	bool permit_generation_tanks{ false }; //YES - the tank is waiting for its turn
	unsigned int number_all_tanks{ 0 };
	unsigned int number_dead_tanks{ 0 };
	unsigned int number_loaded_tanks{ 0 };
	unsigned int number_killed_tanks_PL1{ 0 };
	unsigned int number_killed_tanks_PL2{ 0 };
	unsigned int number_read_tanks{ 0 };
	void CreateTanks();
	void CreateTanks(const int);
	void CreateTank(const sf::Vector2f);
	void ReloadTank(Tank&, const sf::Vector2f);
	void updTanks();
	void DrawTanks();
	void DrawTank(Tank&);
	void MoveTank(Tank&, float, bool);
	void MoveTank(const Direction, Tank&, float);
	bool DistanceTank(Tank&, const float);
	bool DistanceTank(Tank&, Tank&, const float);
	void ControlTank_onFrame(Tank&);
	void CheckTankBang(const int, const int, const bool);
	void KillAllTanks();
	void ControlHangPoint();
	void ControlBonusTank();
	void ControlHeavyTank();
	void ControlFrontMode();
	void RotationTank(Tank&, const char*, const char*, const float);

	friend void ControlSpawnEnemies(GameField*, const int);
	bool LAUNCHING_TANKS_ON_OFF{ false };
	friend void LAUNCHING_TANKS(GameField*, const int);
	friend void LAUNCHING_TANKS_NUM(GameField*, const int);
	friend void LOAD_TANK(GameField*);
	friend int CONTROL_NUMBER_CURRENT_TANKS(GameField*);
	friend bool CONTROL_CollisionTanksBS(GameField*);
	friend bool CONTROL_CollisionTanksTank(GameField*, Tank&);
	friend bool CONTROL_CheckFinishTimeAnim(GameField*, const int);
	friend bool CONTROL_OffAllAnim(GameField*);

	struct AnimTank
	{
		std::unique_ptr<AnimBirth> tankBirth{ nullptr };
		std::unique_ptr<AnimSkin> tankSkin{ nullptr };
	} tankAnimArr[6];
	void CreateAnimBirth(const sf::Vector2f, const int);

	std::array<Bullet*, 8> bulletArr = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	std::shared_ptr<AnimBoom> bulletBoom[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	std::shared_ptr<AnimBoom> tankBoom[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	std::shared_ptr<AnimBoom> emblemBoom[1] = { nullptr };
	void CreateAnimBoom(const sf::Vector2f, const char*);
	void CreateAnimBigBoom();

	template <typename T>
	void CreateBullet(T&, sf::Vector2f);
	void CreateBullet(Tank&);
	void CreateBullet(Player&);
	void updBullets();
	void DrawBullets();

	std::vector<RecShape> vecRecShape;
	template<typename T> void FormArrayCells(T **, const sf::Vector2i, const sf::Vector2i, RecShape&);
	template<typename T> void DisbandArrayCells(T **);

	friend void EnumerationTanks(GameField&);

	bool fPL{ false };
	void CheckOnCollisionFrame(Tank&); //for the tanks
	void CheckOnCollisionBlocksSpawn(Tank&); //for the tanks
	void CheckOnCollisionBlocks(Tank&); //for the tanks
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
	void CheckOnEmblem();
	void CheckOnBonus();

	//emblem
	Emblem emblem{ texture };
	void CreateEmblem();
	void DrawEmblem();

	//actors
	Player *firstPlayer;
	Player *secondPlayer;
	std::unique_ptr<BlockSpawn> lPlayer_BS{ nullptr };
	std::unique_ptr<BlockSpawn> rPlayer_BS{ nullptr };

	bool TwoPlayer() const { return p_player > 1; }

	void CreateActors();
	friend void CreateActorsWait(GameField*, const int);
	friend void RestartPlayer(GameField*, const int);
	void RestartFirstPlayer(const bool flag = false);
	void RestartSecondPlayer(const bool flag = false);
	void updPlayers();
	void DrawActors();
	friend void MoveFirstPlayer(GameField&, const Direction);
	friend void MoveSecondPlayer(GameField&, const Direction);
	void MonitoringKeys();
	void CheckPlayerBang(Player&, const bool off = false);
	void PerfectionPlayer(Player&);

	void Load_Lifes_and_Stars();
	void Save_Lifes_and_Stars();
	void Reset_Lifes_and_Stars();
	
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

	void CreateBonus();
	void DrawBonus();
	Bonus *bonus{ nullptr };

	void onBonusTankFun(Player&);
	void onBonusSkinFun(Player&);
	void onBonusStarFun(Player&);
	void onBonusShovelFun(Player&);
	void onBonusClockFun(Player&);
	void onBonusGrenadeFun(Player&);
	void onBonusPistolFun(Player&);

	friend void WaitingShovel(GameField*, const int);
	friend void WaitingClock(GameField*, const int);


	//UI FUNCS
	void DrawUI();
	void usesUI_tank();
	void usesUI_nflags();
	void usesUI_nlifes();
	void usesUI_nlifes_off();
	void usesUI_nalltanks();
	void usesUI_ntanksforplayer1();
	void usesUI_ntanksforplayer2();

	Curtain *curtain{ nullptr };
	void DrawCurtain();
	friend void ControlCurtain(GameField*, const int);

	UIGameOverMSG uiGameOverMSG{ texture };
	UIPauseMSG uiPauseMSG{ texture };
	void StartGameOverMSG();
	void StartPauseMSG();
	void StopPauseMSG();

	void GameOver();
	void GameWinning();
	void MonitoringElements();

	ManagerThreads mThreads;

public:
	explicit GameField(sf::RenderWindow&, sf::Texture&);
	~GameField();

	void UpdateField();
};

class GameFPS
{
private:
	FPS fps;
	MSG msg{ sf::Vector2f(8.f, 2.f) };

public:
	GameFPS()
	{
	}

	void StartFrame()
	{
		if (!p_showfps)
			return;
		fps.StartFrame();
		return;
	}

	void FinishFrame()
	{
		if (!p_showfps)
			return;
		fps.FinishFrame();
		return;
	}

	void ProcessingFPS(sf::RenderWindow &window)
	{
		if (!p_showfps)
			return;

		auto s_fps = std::to_string(fps.updateFPS());
		auto s_timeFrame = std::to_string(fps.updateTimeFrame());
		auto s = "FPS: " + s_fps + "  " + "Time Frame: " + s_timeFrame;

		msg.TakeText().setString(s);
		window.draw(msg.TakeText());

		return;
	}
};

#endif
