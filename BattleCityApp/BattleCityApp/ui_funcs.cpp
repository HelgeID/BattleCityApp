#include "field.h"

void GameField::usesUI_tank()
{
	for (int index(0); index < storage_tanks.TakeArrUITankSize(); ++index) {
		if (index == storage_tanks.TakeArrUITankSize())
			break;
		if (!storage_tanks.TakeArrUITank()[index].isClearObj()) {
			storage_tanks.TakeArrUITank()[index].offUITank();
			break;
		}
	}
	return;
}

void GameField::usesUI_nlifes()
{
	number_lifes_first.uiNumber->UPD_UINumber(firstPlayer->takelife());
	number_lifes_second.uiNumber->UPD_UINumber(secondPlayer->takelife());

	if (!TwoPlayer()) {
		number_lifes_second.uiPlayer->offUIPlayer();
		number_lifes_second.uiNumber->offUINumber();
	}
	return;
}

void GameField::usesUI_nflags()
{
	unsigned int num1(p_level / 10), num2(p_level % 10);
	!num1 ? number_flags.uiNumber1->offUINumber() : number_flags.uiNumber1->UPD_UINumber(num1);
	number_flags.uiNumber2->UPD_UINumber(num2);
	return;
}
