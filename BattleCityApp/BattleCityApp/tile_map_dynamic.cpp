#include "tile_map_dynamic.h"
#include <math.h>

//////////
//Map
//////////

bool TileMap::load(sf::Texture& texture, const int* map)
{
	this->texture = &texture;

	//init the size of the tile
	const sf::Vector2u tileSize = sf::Vector2u(16, 16);
	//init the size of the map (in a tiles)
	const unsigned int size(13);

	//resize the vertex array to fit the level size
	this->vertices.setPrimitiveType(sf::Quads);
	this->vertices.resize(pow(size, 2) * 4);

	//populate the vertex array, with one quad per tile
	for (unsigned int iX = 0; iX < size; ++iX) {
		for (unsigned int jY = 0; jY < size; ++jY) {
			//get the current tile number
			int tileNumber = map[iX + jY * size];

			//find position in the tile
			int tu = tileNumber % (this->texture->getSize().x / tileSize.x);
			int tv = tileNumber / (this->texture->getSize().x / tileSize.x);

			//get a pointer to the current tile's quad
			sf::Vertex* quad = &vertices[(iX + jY * size) * 4];

			//define its 4 corners
			quad[0].position = sf::Vector2f(iX * tileSize.x, jY * tileSize.y);
			quad[1].position = sf::Vector2f((iX + 1) * tileSize.x, jY * tileSize.y);
			quad[2].position = sf::Vector2f((iX + 1) * tileSize.x, (jY + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(iX * tileSize.x, (jY + 1) * tileSize.y);

			//define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		}
	}
	return true;
}

void TileMap::setSprite(const int tileNumber, sf::Vector2u index)
{
	//init the size of the map (in a tiles)
	const unsigned int size(13);
	//check for input data
	if (index.x >= size || index.y >= size)
		return;

	//init the size of the tile
	const sf::Vector2u tileSize = sf::Vector2u(16, 16);

	//find its position in the tileset texture
	int tu = tileNumber % (this->texture->getSize().x / tileSize.x);
	int tv = tileNumber / (this->texture->getSize().x / tileSize.x);

	//convert the position in matrix into the index of array
	int indexArr = takeIndex(index);
	
	//calculate the step: 0, 4, 8, 12 ...
	const int step = (indexArr * 4);

	//define its 4 corners
	this->vertices[step + 0].position = sf::Vector2f(index.x * tileSize.x, index.y * tileSize.y);
	this->vertices[step + 1].position = sf::Vector2f((index.x + 1) * tileSize.x, index.y * tileSize.y);
	this->vertices[step + 2].position = sf::Vector2f((index.x + 1) * tileSize.x, (index.y + 1) * tileSize.y);
	this->vertices[step + 3].position = sf::Vector2f(index.x * tileSize.x, (index.y + 1) * tileSize.y);

	//define its 4 texture coordinates
	this->vertices[step + 0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
	this->vertices[step + 1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
	this->vertices[step + 2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
	this->vertices[step + 3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

	return;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//apply the transform
	states.transform *= getTransform();

	//apply the tileset texture
	states.texture = &*texture;

	//draw the vertex array
	target.draw(vertices, states);

	return;
}

//////////
//Dynamic
//////////

TileDynamic::TileDynamic(sf::Texture& texture) : texture(&texture)
{
	//resize the vertex array to fit the level size
	this->vertices.setPrimitiveType(sf::Quads);
	this->vertices.resize(79 * 4); //here change, when add elements
}

TileDynamic::~TileDynamic()
{
}

void TileDynamic::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//apply the transform
	states.transform *= getTransform();

	//apply the tileset texture
	states.texture = &*texture;

	//draw the vertex array
	target.draw(vertices, states);

	return;
}

void TileDynamic::setSprite(const int index, const sf::Vector2f mapCoords, const sf::Vector2i texCoords, float size_TILE)
{
	size_TILE == 0 ? size_TILE = SIZE_TILE : 0;

	//calculate the step: 0, 4, 8, 12 ...
	const int step(index * 4);

	//define its 4 corners
	this->vertices[step + 0].position = sf::Vector2f(mapCoords.x, mapCoords.y);
	this->vertices[step + 1].position = sf::Vector2f(mapCoords.x + size_TILE, mapCoords.y);
	this->vertices[step + 2].position = sf::Vector2f(mapCoords.x + size_TILE, mapCoords.y + size_TILE);
	this->vertices[step + 3].position = sf::Vector2f(mapCoords.x, mapCoords.y + size_TILE);

	//define its 4 texture coordinates (16x16)
	this->vertices[step + 0].texCoords = sf::Vector2f(texCoords.x, texCoords.y);
	this->vertices[step + 1].texCoords = sf::Vector2f(texCoords.x + size_TILE, texCoords.y);
	this->vertices[step + 2].texCoords = sf::Vector2f(texCoords.x + size_TILE, texCoords.y + size_TILE);
	this->vertices[step + 3].texCoords = sf::Vector2f(texCoords.x, texCoords.y + size_TILE);

	return;
}
