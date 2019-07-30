//FINAL
#ifndef CURTAIN_H
#define CURTAIN_H

#include <SFML/Graphics.hpp>
#include "general.hpp"

class Curtain : public sf::Drawable
{
	sf::RectangleShape rsUP;
	sf::RectangleShape rsDOWN;

	void setPos_rsUP(const sf::Vector2f pos) { rsUP.setPosition(pos); return; }
	void setPos_rsDOWN(const sf::Vector2f pos) { rsDOWN.setPosition(pos); return; }
	void setSize_rsUP(sf::Vector2f size) { rsUP.setSize(size); return; }
	void setSize_rsDOWN(sf::Vector2f size) { rsDOWN.setSize(size); return; }

public:
	Curtain(sf::RectangleShape field)
	{
		const sf::Vector2f size{ SizeField, SizeField / 2 };
		const sf::Vector2f pos_rsUP = field.getPosition();
		const sf::Vector2f pos_rsDOWN = sf::Vector2f(pos_rsUP.x, pos_rsUP.y + SizeField / 2);
		setPos_rsUP(pos_rsUP); setPos_rsDOWN(pos_rsDOWN);
		setSize_rsUP(size); setSize_rsDOWN(size);
		rsUP.setFillColor(sf::Color(99, 99, 99));
		rsDOWN.setFillColor(sf::Color(99, 99, 99));
	}

	void UpdCurtain()
	{
		rsUP.move(0.f, -1.f);
		rsDOWN.move(0.f, 1.f);
		return;
	}

	bool FinishCurtain()
	{
		const float point1(rsUP.getPosition().y + SizeField / 2);
		const float point2(rsDOWN.getPosition().y);
		return (point2 - point1 > SizeField);
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(rsUP, states);
		target.draw(rsDOWN, states);
	}
};

#endif
