#include "events.h"

GameEvent::GameEvent(sf::RenderWindow &window)
	: window(window)
{
}

GameEvent::~GameEvent()
{
}

void GameEvent::UpdateEvent()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}

	return;
}
