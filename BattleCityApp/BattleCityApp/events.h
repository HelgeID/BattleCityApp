#ifndef EVENTS_H
#define EVENTS_H

#include <SFML\Graphics.hpp>

class GameEvent
{
	sf::RenderWindow &window;

public:
	explicit GameEvent(sf::RenderWindow &);
	~GameEvent();

	template <typename T>
	void UpdateEvent(T*);
};

template <typename T>
void GameEvent::UpdateEvent(T* game)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			window.setTitle("closing, please wait...");
			if (no_close)
				no_close = false;
			game->winStyle(window, sf::Style::Titlebar);
		}
		//!p_showframe ? p_showframe = true : p_showframe = false;
		if (event.type == sf::Event::KeyPressed)
			KeyActive = true;
		if (event.type == sf::Event::KeyReleased)
			KeyActive = false;
	}

	//sending: UpdateEvent() - (this)
	//receiving: MonitoringKeys()
	(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !gameover) ? Key_A = true : Key_A = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !gameover) ? Key_D = true : Key_D = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !gameover) ? Key_W = true : Key_W = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !gameover) ? Key_S = true : Key_S = false;

	(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !gameover) ? Key_Left = true : Key_Left = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !gameover) ? Key_Right = true : Key_Right = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !gameover) ? Key_Up = true : Key_Up = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !gameover) ? Key_Down = true : Key_Down = false;

	(sf::Keyboard::isKeyPressed(sf::Keyboard::F) && !gameover) ? Key_F = true : Key_F = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) && !gameover) ? Key_Ctrl = true : Key_Ctrl = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !gameover) ? Key_Space = true : Key_Space = false;
	(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) ? Key_Return = true : Key_Return = false;
	return;
}

#endif
