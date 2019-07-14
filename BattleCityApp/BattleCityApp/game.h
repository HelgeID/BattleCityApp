#ifndef GAME_H
#define GAME_H

#define TITLE "BattleCity"

#include <SFML\Graphics.hpp>

class Game
{
	sf::RenderWindow &window;
	sf::Texture* texture;

public:
	explicit Game(sf::RenderWindow &);
	~Game();

	void InitParamsGame();
	void InitParamsWin(bool restart = false);

	void GameMenu();
	void GameStage();
	void GameLaunch();


	void winStyle(sf::Window&, sf::Uint32);

private:
	bool loadTexture();
	bool loadIcon();

	void zoomOn();
	void zoomOff();

	void HideConsole();
	void ShowConsole();
	bool IsConsoleVisible();

	void TakeConsolePos(int*, int*);
};

#endif
