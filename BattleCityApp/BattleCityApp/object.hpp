#pragma once

#include <SFML/Graphics.hpp>
#include "general.hpp"

class Object
{
	sf::Texture &texture;
	sf::Sprite sprite;

public:
	Object(sf::Texture &texture) : texture(texture)
	{
		sprite.setTexture(texture);
		clearSpriteObj();
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

	sf::Vector2f getPosObj() const
	{
		return sprite.getPosition();
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
