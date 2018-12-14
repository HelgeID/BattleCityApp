#include "part_bricks_map.h"
#include "general.hpp"

void Part_Bricks_Map::fillMap(const sf::RectangleShape &element)
{
	VertexCoord::VertexCoord coord = {
		element.getPosition().x, element.getPosition().y,
		element.getSize().x, element.getSize().y
	};
	addElement(coord);
	return;
}

void Part_Bricks_Map::addElement(const VertexCoord::VertexCoord &coord)
{
	(vertices.getPrimitiveType() != sf::Quads) ? vertices.setPrimitiveType(sf::Quads) : NULL;

	const int size(4);
	sf::Vertex *vertex = new sf::Vertex[size];
	vertex[0].position = sf::Vector2f(coord.X, coord.Y);
	vertex[1].position = sf::Vector2f(coord.X + coord.wX, coord.Y);
	vertex[2].position = sf::Vector2f(coord.X + coord.wX, coord.Y + coord.hY);
	vertex[3].position = sf::Vector2f(coord.X, coord.Y + coord.hY);

	for (int i = 0; i < size; i++) {
		if (p_showframe)
			vertex[i].color = sf::Color(127, 127, 127, 32);
		else
			vertex[i].color = sf::Color(0, 0, 0, 255);
		vertices.append(vertex[i]);
	}

	delete[] vertex;
	return;
}

void Part_Bricks_Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//apply the transform
	states.transform *= getTransform();

	//draw the vertex array
	target.draw(vertices, states);

	return;
}
