#include "field.h"
#include "general.hpp"

void ControlSound(GameField* gField)
{
	auto PRESENCE_PLAYER = [&]()
	{
		std::lock_guard<std::mutex> lg(mtx);
		if (gField->firstPlayer->Presence() || gField->secondPlayer->Presence())
			return true;
		return false;
	};

	auto KEY_MOVE = [&]()
	{
		std::lock_guard<std::mutex> lg(mtx);
		if ((gField->firstPlayer->Presence() && (Key_A || Key_D || Key_W || Key_S)) ||
			(gField->secondPlayer->Presence() && (Key_Left || Key_Right || Key_Up || Key_Down)))
			return true;
		return false;
	};

	//start
	sf::sleep(sf::milliseconds(1000));
	gField->sound.StartMusic();
	while (gField->sound.getMusic())
		;

	while (true)
	{
		if (KEY_MOVE() && !pause)
			gField->sound.Moving();
		else
			gField->sound.Stopping();

		if (!PRESENCE_PLAYER() || pause)
			gField->sound.Absent();

		sf::sleep(sf::milliseconds(100));
	}
	return;
}
