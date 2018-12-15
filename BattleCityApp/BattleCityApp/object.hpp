#pragma once

#include <SFML/Graphics.hpp>
#include "general.hpp"

enum Direction { UP, LEFT, DOWN, RIGHT };

class Object
{
	sf::Texture &texture;
	sf::Sprite sprite;
	sf::Vector2f size;

public:
	Object(sf::Texture &texture, sf::Vector2f size = sf::Vector2f(16.f, 16.f)) : texture(texture)
	{
		sprite.setTexture(texture);
		this->size = size;
		clearSpriteObj();
	}

	void setSpriteObj(const int posX, const int posY)
	{
		sf::IntRect rect(posX, posY, this->size.x, this->size.y);
		sprite.setTextureRect(rect);
		return;
	}

	void clearSpriteObj()
	{
		sf::Vector2u sizeTexture = this->texture.getSize();
		sf::IntRect rect(
			(int)sizeTexture.x - this->size.x,
			(int)sizeTexture.y - this->size.y,
			this->size.x,
			this->size.y
		);
		sprite.setTextureRect(rect);
		return;
	}

	void setPosObj(const float x, const float y)
	{
		sprite.setPosition(x, y);
		return;
	}

	sf::Vector2f getPosObj() const
	{
		return sprite.getPosition();
	}

	sf::Vector2f getSizeObj() const
	{
		return size;
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
