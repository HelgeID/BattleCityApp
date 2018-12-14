#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <SFML/Graphics.hpp>
#include <string>

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

#endif
