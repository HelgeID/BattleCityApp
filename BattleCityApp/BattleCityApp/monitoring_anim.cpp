#include "field.h"
#include "general.hpp"

void GameField::MonitoringAnim(const AnimBirth* ptr)
{
	//ptr - not used !!!
	auto Monitoring = [&](Anim& playerAnim)
	{
		if (playerAnim.playerBirth != nullptr) {
			if (playerAnim.playerBirth->FinishTime())
				playerAnim.playerBirth = nullptr;
			else
				playerAnim.playerBirth->Update();
		}
		return;
	};
	Monitoring(firstPlayerAnim);
	Monitoring(secondPlayerAnim);

	return;
}

void GameField::MonitoringAnim(const AnimSkin* ptr)
{
	//ptr - not used !!!
	auto Monitoring = [&](Anim& playerAnim, Player* player)
	{
		if (playerAnim.playerSkin != nullptr) {
			if (playerAnim.playerSkin->FinishTime()) {
				playerAnim.playerSkin = nullptr;
				player->SkinOff();
			}
			else {
				playerAnim.playerSkin->Update(player->getPosObj());
			}
		}
	};
	Monitoring(firstPlayerAnim, firstPlayer);
	Monitoring(secondPlayerAnim, secondPlayer);

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
