﻿#include "block_spawn.hpp"
#include "field.h"

void GameField::CreatePointsSpawnEnemy()
{
	std::unique_ptr<BlockSpawn> l_BS{ new BlockSpawn(texture, sf::Vector2f(128.f, 80.f)) };
	std::unique_ptr<BlockSpawn> r_BS{ new BlockSpawn(texture, sf::Vector2f(64.f, 100.f)) };
	std::unique_ptr<BlockSpawn> c_BS{ new BlockSpawn(texture, sf::Vector2f(64.f, 32.f)) };
	this->l_BS = std::move(l_BS);
	this->r_BS = std::move(r_BS);
	this->c_BS = std::move(c_BS);
	return;
}