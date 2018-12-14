#ifndef PART_BRICKS_MAP_H
#define PART_BRICKS_MAP_H

#include <SFML/Graphics.hpp>

namespace VertexCoord
{
	struct VertexCoord
	{
		float X, Y, wX, hY;
	};
}

class Part_Bricks_Map: public sf::Drawable, public sf::Transformable
{
	sf::VertexArray vertices;
public:
	void fillMap(const sf::RectangleShape& );

private:
	void addElement(const VertexCoord::VertexCoord& );

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
};

#endif
