#include "field.h"
#include <vector>

void parse_number(std::vector<unsigned int>& vecNumber, int number)
{
	vecNumber.push_back(number % 10);
	int temp(number / 10);
	if (temp == 0) return;
	parse_number(vecNumber, temp);
	return;
}

void init_numberUI(std::vector<unsigned int>& vecNumber,
	UINumber* uiNUM1, 
	UINumber* uiNUM2, 
	UINumber* uiNUM3)
{
	switch (vecNumber.size())
	{
	case 1:
		uiNUM1->offUINumber();
		uiNUM2->offUINumber();
		uiNUM3->UPD_UINumber(vecNumber[0]);
		break;

	case 2:
		uiNUM1->offUINumber();
		uiNUM2->UPD_UINumber(vecNumber[1]);
		uiNUM3->UPD_UINumber(vecNumber[0]);
		break;

	case 3:
		uiNUM1->UPD_UINumber(vecNumber[2]);
		uiNUM2->UPD_UINumber(vecNumber[1]);
		uiNUM3->UPD_UINumber(vecNumber[0]);
		break;

	}

	return;
}

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
	number_lifes_first.uiNumber->UPD_UINumber(firstPlayer->numLife() - 1);
	number_lifes_second.uiNumber->UPD_UINumber(secondPlayer->numLife() - 1);

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

void GameField::usesUI_nalltanks()
{
	std::vector<unsigned int> vecNumber;
	parse_number(vecNumber, number_all_tanks - number_dead_tanks);
	init_numberUI(vecNumber, numberAllTanks.uiNumber1, numberAllTanks.uiNumber2, numberAllTanks.uiNumber3);

	return;
}

void GameField::usesUI_ntanksforplayer1()
{
	std::vector<unsigned int> vecNumber;
	parse_number(vecNumber, number_killed_tanks_PL1);
	init_numberUI(vecNumber, numberTanksForFirstPlayer.uiNumber1, numberTanksForFirstPlayer.uiNumber2, numberTanksForFirstPlayer.uiNumber3);
	return;
}

void GameField::usesUI_ntanksforplayer2()
{
	std::vector<unsigned int> vecNumber;
	parse_number(vecNumber, number_killed_tanks_PL2);
	init_numberUI(vecNumber, numberTanksForSecondPlayer.uiNumber1, numberTanksForSecondPlayer.uiNumber2, numberTanksForSecondPlayer.uiNumber3);
	return;
}
