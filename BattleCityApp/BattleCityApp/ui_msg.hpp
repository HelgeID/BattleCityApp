#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>

#include "general.hpp"
#include "object.hpp"

class UIMsg : public Object
{
	bool presenceflag = { false };
public:
	std::chrono::time_point<std::chrono::steady_clock> startClock, endClock;
	UIMsg(sf::Texture &texture, const sf::Vector2f size) : Object(texture, size) { startClock = std::chrono::steady_clock::now(); }
	bool& PresenceMSG() { return presenceflag; }
	void DellMSG() { presenceflag = false; return; }
};

//start Game Over MSG =>>> call func: GameOverMSG()
//finish Game Over MSG =>>> call method obj: UIGameOverMSG::DellMSG()
class UIGameOverMSG : public UIMsg
{
public:
	UIGameOverMSG(sf::Texture &texture) : UIMsg(texture, sf::Vector2f(32.f, 16.f)){}
	void GameOverMSG();//msg GameOver
};

//start Pause MSG =>>> call func: PauseMSG()
//finish Pause MSG =>>> call method obj: UIPauseMSG::DellMSG()
class UIPauseMSG : public UIMsg
{
public:
	UIPauseMSG(sf::Texture &texture) : UIMsg(texture, sf::Vector2f(40.f, 8.f)) {}
	void PauseMSG(const bool); //msg Pause (on, off)
};

template <typename T>
void UpdMSGBlink(T* obj)
{
	if (!no_close)
		return;

	sf::sleep(sf::milliseconds(1000));

	if (!no_close)
		return;

	const sf::Vector2i coordUIMsg(obj->getSpriteObj());
	while (true)
	{
		if (!no_close)
			return;

		if (obj->PresenceMSG() != true)
			break;

		obj->endClock = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(obj->endClock - obj->startClock);

		if (elapsed.count() > 200) {
			!obj->isClearObj() ? obj->clearSpriteObj(): 
				obj->setSpriteObj(coordUIMsg.x, coordUIMsg.y);
			obj->startClock = std::chrono::steady_clock::now();
		}
	}
	obj->clearSpriteObj();
	//std::cerr << "finish MSG Blink" << std::endl;
	return;
}

template <typename T>
void UpdMSGMove(T* obj, sf::Vector2i point, const Direction dir)
{
	if (!no_close)
		return;

	while (!(point.x == obj->getPosObj().x &&
				point.y == obj->getPosObj().y) && no_close)
	{
		switch (dir) {
			case UP: obj->moveObj(0.f, -1.f); break;
			case LEFT: obj->moveObj(-1.f, 0.f); break;
			case DOWN: obj->moveObj(0.f, 1.f); break;
			case RIGHT: obj->moveObj(1.f, 0.f); break;
		}
		sf::sleep(sf::milliseconds(8));
	}
	//std::cerr << "finish MSG Move" << std::endl;
	return;
}

template <typename T>
void UpdMSGMoveBlink(T* obj, sf::Vector2i point,const Direction dir)
{
	UpdMSGMove<T>(obj, point, dir);
	UpdMSGBlink<T>(obj);
	return;
}

//calls
inline void UIGameOverMSG::GameOverMSG()
{
	this->PresenceMSG() = true;
	this->setSpriteObj(288, 184);
	const sf::Vector2i point(SizeField / 2 + 16, SizeField / 2 + 16);
	this->setPosObj(SizeField / 2 + 16, SizeField);
	std::unique_ptr<std::thread> updMSGMoveBlink(new std::thread(&UpdMSGMoveBlink<UIGameOverMSG>, this, point, Direction::UP));
	updMSGMoveBlink->detach();
	return;
}

inline void UIPauseMSG::PauseMSG(const bool pause)
{
	(PresenceMSG() = pause) ? this->setSpriteObj(288, 175) : this->clearSpriteObj();
	this->setPosObj(SizeField / 2 + 12, SizeField / 2 + 16);
	if (PresenceMSG())
	{
		std::unique_ptr<std::thread> updMSGBlink(new std::thread(&UpdMSGBlink<UIPauseMSG>, this));
		updMSGBlink->detach();
	}
	return;
}
