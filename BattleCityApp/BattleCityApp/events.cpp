#include "events.h"
#include "general.hpp"

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
			//window.close();
			!p_showframe ? p_showframe = true : p_showframe = false;
		if (event.type == sf::Event::KeyPressed)
			KeyActive = true;
		if (event.type == sf::Event::KeyReleased)
			KeyActive = false;
	}
	
	//sending: UpdateEvent() - (this)
	//receiving: MonitoringKeys()
	(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ? Key_A = true : Key_A = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ? Key_D = true : Key_D = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) ? Key_W = true : Key_W = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) ? Key_S = true : Key_S = false;

	(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ? Key_Left = true : Key_Left = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ? Key_Right = true : Key_Right = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ? Key_Up = true : Key_Up = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) ? Key_Down = true : Key_Down = false;

	(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) ? Key_F = true : Key_F = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) ? Key_Ctrl = true : Key_Ctrl = false;
	return;
}
