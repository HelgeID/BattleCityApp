#include "field.h"
#include "general.hpp"

void ControlCurtain(GameField* gField)
{
	sf::sleep(sf::milliseconds(1500));
	while (!gField->curtain->FinishCurtain())
	{
		sf::sleep(sf::milliseconds(3));
		mtx.lock();
		gField->curtain->UpdCurtain();
		mtx.unlock();
	}

	mtx.lock();
	delete gField->curtain;
	gField->curtain = nullptr;
	mtx.unlock();

	return;
}
