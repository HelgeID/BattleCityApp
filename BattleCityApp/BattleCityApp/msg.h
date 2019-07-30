//FINAL
#ifndef MSG_H
#define MSG_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "general.hpp"

class MSG
{
private:
	sf::Text text;
	sf::Font font;

public:
	MSG()
	{
		try {
			if (!font.loadFromFile("data/fonts/arial.ttf"))
				throw EXIT_FAILURE;
		}

		catch (...)
		{
		}

		text.setString("msg");
		text.setFont(font);
		text.setFillColor(sf::Color::White);

		text.setCharacterSize(15);
		text.setStyle(sf::Text::Regular);
		
		if (p_zoom)
		{
			const sf::Vector2f scale(text.getScale());
			text.setScale(scale.x / 2, scale.y / 2);
		}
		
	}

	MSG(sf::Vector2f pos) : MSG() { text.setPosition(pos.x, pos.y); }

	sf::Text& TakeText() { return text; }
};

#endif
