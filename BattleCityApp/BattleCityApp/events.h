#ifndef EVENTS_H
#define EVENTS_H

#include <SFML\Graphics.hpp>

class GameEvent
{
	sf::RenderWindow &window;

public:
	explicit GameEvent(sf::RenderWindow &);
	~GameEvent();

	void UpdateEvent();
};

#endif
