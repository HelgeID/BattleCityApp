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

	void offsetFrame(const int value)
	{
		switch (value%10)
		{
			case 1: offsetRight(); goto exit;
			case 2: offsetDown(); goto exit;
			case 3: offsetLeft(); goto exit;
			case 4: offsetUP(); goto exit;
		}
	exit:
		;
		return;
	}

private:
	void offsetLeft() {
		const int offset(7);
		const float x = (frame.getPosition().x);
		const float y = (frame.getPosition().y);

		frame.setPosition(x, y);
		frame.setSize(sf::Vector2f((float)offset - 1, (float)offset * 2));
		//frame.setOutlineColor(sf::Color(0, 127, 0, 64));
		//frame.setFillColor(sf::Color(0, 64, 0, 64));
		return;
	}

	void offsetRight() {
		const int offset(7);
		const float x = (frame.getPosition().x + 1) + offset;
		const float y = (frame.getPosition().y);

		frame.setPosition(x, y);
		frame.setSize(sf::Vector2f((float)offset - 1, (float)offset * 2));
		//frame.setOutlineColor(sf::Color(0, 127, 0, 64));
		//frame.setFillColor(sf::Color(0, 64, 0, 64));
		return;
	}

	void offsetUP() {
		const int offset(7);
		const float x = (frame.getPosition().x);
		const float y = (frame.getPosition().y);

		frame.setPosition(x, y);
		frame.setSize(sf::Vector2f((float)offset * 2, (float)offset - 1));
		//frame.setOutlineColor(sf::Color(0, 127, 0, 64));
		//frame.setFillColor(sf::Color(0, 64, 0, 64));
		return;
	}

	void offsetDown() {
		const int offset(7);
		const float x = (frame.getPosition().x);
		const float y = (frame.getPosition().y + 1) + offset;

		frame.setPosition(x, y);
		frame.setSize(sf::Vector2f((float)offset * 2, (float)offset - 1));
		//frame.setOutlineColor(sf::Color(0, 127, 0, 64));
		//frame.setFillColor(sf::Color(0, 64, 0, 64));
		return;
	}
};
