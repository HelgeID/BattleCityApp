//FINAL
#pragma once

#include <SFML/Graphics.hpp>
#include "general.hpp"
#include "object.hpp"
#include "frame.hpp"

class BlockSpawn : public Object, public Frame
{
	bool spawn;
public:
	BlockSpawn(sf::Texture &texture, sf::Vector2f pos) : Object(texture), Frame("block spawn"), spawn(false)
	{
		this->setPosObj(pos.x, pos.y);
		this->setPosFrame(pos.x, pos.y);
	}
	bool& Spawn() { return spawn; };
};
