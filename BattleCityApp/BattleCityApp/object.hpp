#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "general.hpp"

enum Direction { UP, LEFT, DOWN, RIGHT };

struct TileCoords
{
	sf::Vector2f mapCoords;
	sf::Vector2i texCoords;
};

class Object
{
	sf::Texture &texture;
	sf::Sprite sprite;
	sf::Vector2f size;

public:
	int xRect, yRect; // for rect texture

	Object(sf::Texture &texture, sf::Vector2f size = sf::Vector2f(16.f, 16.f)) : texture(texture)
	{
		sprite.setTexture(texture);
		this->size = size;
		clearSpriteObj();
	}

	void setSpriteObj(const int posX, const int posY)
	{
		xRect = posX; yRect = posY;
		sf::IntRect rectInit(posX, posY, this->size.x, this->size.y);
		sprite.setTextureRect(rectInit);
		return;
	}

	sf::Vector2i getSpriteObj() const
	{
		return sf::Vector2i(
			sprite.getTextureRect().left, sprite.getTextureRect().top
		);
	}

	void clearSpriteObj()
	{
		sf::Vector2u sizeTexture = this->texture.getSize();
		const sf::IntRect rectInit(
			(int)sizeTexture.x - this->size.x,
			(int)sizeTexture.y - this->size.y,
			this->size.x,
			this->size.y
		);
		sprite.setTextureRect(rectInit);
		return;
	}

	bool isClearObj()
	{
		sf::Vector2u sizeTexture = this->texture.getSize();
		const sf::IntRect rectInit(
			(int)sizeTexture.x - this->size.x,
			(int)sizeTexture.y - this->size.y,
			this->size.x,
			this->size.y
		);

		const sf::IntRect rectObj = sprite.getTextureRect();

		return (rectInit == rectObj);
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
