#pragma once
#include <SFML/Graphics.hpp>
#include <string>

//additional functionality
class RecShape : public sf::RectangleShape
{
public:
	RecShape()
	{
		this->setSize(sf::Vector2f((float)SizeCell, (float)SizeCell));
		this->setFillColor(sf::Color(127, 127, 127, 64));
	}
};

class Frame
{
	void initFrame(const std::string name, sf::Vector2f& size)
	{
		if (name == "first player" || name == "second player" || name == "tank")
			size = sf::Vector2f(size.x - 1, size.y - 1);
		frame.setSize(size);
		return;
	}

public:
	sf::RectangleShape frame;
	std::string name;
	Frame(const std::string name = "", sf::Vector2f size = sf::Vector2f(16.f, 16.f)) : name(name)
	{
		initFrame(name, size);
		if (name == "tank") {
			frame.setOutlineColor(sf::Color(127, 0, 0, 64));
			frame.setFillColor(sf::Color(64, 0, 0, 64));
		}
		else if (name == "block") {
			frame.setOutlineColor(sf::Color(0, 0, 127, 64));
			frame.setFillColor(sf::Color(0, 0, 64, 64));
		}
		else if (name == "bullet") {
			frame.setOutlineColor(sf::Color(0, 127, 127, 64));
			frame.setFillColor(sf::Color(0, 127, 64, 64));
		}
		else if (name == "first player") {
			frame.setOutlineColor(sf::Color(127, 127, 0, 64));
			frame.setFillColor(sf::Color(64, 64, 0, 64));
		}
		else if (name == "second player") {
			frame.setOutlineColor(sf::Color(0, 127, 0, 64));
			frame.setFillColor(sf::Color(0, 64, 0, 64));
		}
		else if (name == "block spawn") {
			frame.setOutlineColor(sf::Color(127, 0, 0, 64));
			frame.setFillColor(sf::Color(0, 0, 0, 64));
		}
		else if (name == "emblem") {
			frame.setOutlineColor(sf::Color(0, 0, 0, 48));
			frame.setFillColor(sf::Color(127, 64, 0, 48));
		}
	}

	~Frame()
	{
	}

	void setPosFrame(const float x, const float y)
	{
		frame.setPosition(x, y);
		return;
	}

};
