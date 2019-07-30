//FINAL
#pragma once
#include <SFML/Graphics.hpp>

#include "general.hpp"
#include "object.hpp"
#include "frame.hpp"

class Emblem: public Object, public Frame
{
private:
	bool presenceflag;

public:
	Emblem(sf::Texture &texture) : Object(texture), Frame("emblem")
	{
		InitEmblem();
	}

	void CrushEmblem() {
		this->setSpriteObj(320, 32);
		presenceflag = false;
		return;
	}

	void InitEmblem() {
		this->setSpriteObj(304, 32);
		presenceflag = true;
		return;
	}

	bool isPresence() const { return presenceflag; }
};
