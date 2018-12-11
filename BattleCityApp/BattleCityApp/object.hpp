#pragma once

#include <SFML/Graphics.hpp>
#include "general.hpp"

class Object
{
	sf::Texture &texture;
	sf::Sprite sprite;

public:
	Object(sf::Texture &texture, bool zoom = false) : texture(texture)
	{
		sprite.setTexture(texture);
		clearSpriteObj();

		/*
		z00m false : 1x --> 16x16
		z00m true  : 2x --> 32x32
		*/
		if (zoom)
			sprite.scale(sf::Vector2f(2.f, 2.f));
	}

	void setSpriteObj(const int posX, const int posY)
	{
		sf::IntRect rect(posX, posY, 16, 16);
		sprite.setTextureRect(rect);
		return;
	}

	void clearSpriteObj()
	{
		sf::Vector2u size = this->texture.getSize();
		sf::IntRect rect(size.x - 16, size.y - 16, 16, 16);
		sprite.setTextureRect(rect);
		return;
	}

	void setPosObj(const float x, const float y)
	{
		sprite.setPosition(x, y);
		return;
	}

	void moveObj(const float x, const float y)
	{
		sprite.move(sf::Vector2f(x, y));
		return;
	}

	sf::Sprite takeObj() const { return sprite; }

	~Object()
	{
	}
};
