#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <SFML/Graphics.hpp>
#include <string>

#define SIZE_TILE 16.f

class TileMap : public sf::Drawable, public sf::Transformable
{
	sf::VertexArray vertices;
	sf::Texture *texture;

	//position in matrix -> index of array
	int takeIndex(sf::Vector2u index) { return (index.y * 13 + index.x); }
	//index of array -> position in matrix
	sf::Vector2u takeIndex(int index) { return sf::Vector2u(index % 13, index / 13); }

public:
	bool load(sf::Texture&, const int*);
	void setSprite(const int, sf::Vector2u);

private:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
};

class TileDynamic : public sf::Drawable, public sf::Transformable
{
	sf::VertexArray vertices;
	sf::Texture *texture;

public:
	TileDynamic(sf::Texture&);
	~TileDynamic();

private:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

public:
	void setSprite(const int, const sf::Vector2f, const sf::Vector2i);
};

#endif
