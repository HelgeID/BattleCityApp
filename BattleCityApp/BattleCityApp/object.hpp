﻿#include <SFML/Graphics.hpp>

class Object
{
	sf::Texture &texture;
	sf::Sprite* sprite = nullptr;

public:
	Object(sf::Texture &texture, bool zoom = false) : texture(texture)
	{
		sprite = new sf::Sprite();
		sprite->setTexture(texture);
		clearObj();

		/*
		z00m false : 1x --> 16x16
		z00m true  : 2x --> 32x32
		*/
		if (zoom)
			sprite->scale(sf::Vector2f(2.f, 2.f));
	}

	void setObj(const int posX, const int posY)
	{
		sf::IntRect rect(posX, posY, 16, 16);
		sprite->setTextureRect(rect);
		return;
	}

	void clearObj()
	{
		sf::Vector2u size = this->texture.getSize();
		sf::IntRect rect(size.x - 16, size.y - 16, 16, 16);
		sprite->setTextureRect(rect);
		return;
	}

	sf::Sprite takeObj() const { return *sprite; }

	~Object()
	{
		delete sprite;
	}
};
