#pragma once
#include <SFML/Graphics.hpp>

class Frame
{
	void initFrame()
	{
		frame.setOutlineThickness(1);
		frame.setSize(sf::Vector2f(14.0, 14.0));
	}

public:
	sf::RectangleShape frame;
	Frame(char* name = "")
	{
		initFrame();
		if (name == "tank") {
			frame.setOutlineColor(sf::Color(127, 0, 0, 64));
			frame.setFillColor(sf::Color(64, 0, 0, 64));
		}
		else if (name == "block") {
			frame.setOutlineColor(sf::Color(0, 0, 127, 64));
			frame.setFillColor(sf::Color(0, 0, 64, 64));
		}
	}

	~Frame()
	{
	}

	void setPosFrame(const float x, const float y)
	{
		frame.setPosition(x + 1, y + 1);
		return;
	}

	void offsetLeft() {

	}
	void offsetRight() {

	}
	void offsetUP() {

	}
	void offsetDown() {

	}
};
