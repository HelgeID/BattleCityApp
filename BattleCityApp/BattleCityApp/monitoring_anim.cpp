#include "field.h"
#include "general.hpp"

void GameField::MonitoringAnim(const AnimBirth* ptr)
{
	//ptr - not used !!!
	if (firstPlayerBirth != nullptr) {
		if (firstPlayerBirth->FinishTime())
			firstPlayerBirth = nullptr;
		else
			firstPlayerBirth->Update();
	}

	if (secondPlayerBirth != nullptr) {
		if (secondPlayerBirth->FinishTime())
			secondPlayerBirth = nullptr;
		else
			secondPlayerBirth->Update();
	}
	return;
}

void GameField::MonitoringAnim(const AnimSkin* ptr)
{
	//ptr - not used !!!
	return;
}

void GameField::MonitoringAnim(const AnimBoom* ptr)
{
	//ptr - not used !!!
	const size_t bulletBoomSize = sizeof(this->bulletBoom) / sizeof(*this->bulletBoom);
	for (int i(0); i < bulletBoomSize; ++i) {
		if (bulletBoom[i] != nullptr) {
			if (bulletBoom[i]->FinishTime())
				bulletBoom[i] = nullptr;
			else
				bulletBoom[i]->Update();
		}
	}
	return;
}
