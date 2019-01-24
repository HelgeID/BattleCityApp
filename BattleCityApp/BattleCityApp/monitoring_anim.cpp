#include "field.h"
#include "general.hpp"

void GameField::CreateAnimBirth(const sf::Vector2f point, const int index)
{
	std::unique_ptr<AnimBirth> anim(new AnimBirth(texture, point));
	tankAnimArr[index].tankBirth = std::move(anim);
	return;
}

void GameField::CreateAnimBoom(const sf::Vector2f point, const char* name = "")
{
	auto Create = [&](std::shared_ptr<AnimBoom>* boom, const size_t boomSize, const float maxTime, const bool loop = false)
	{
		std::unique_ptr<AnimBoom> anim(new AnimBoom(texture, point, AnimBoom::AnimChose::Small, maxTime, loop));
		for (int i(0); i < boomSize; ++i) {
			if (boom[i] == nullptr) {
				boom[i] = std::move(anim);
				break;
			}
		}
	};
	
	if (name == "bulletObj") {
		const size_t bulletBoomSize = sizeof(this->bulletBoom) / sizeof(*this->bulletBoom);
		Create(bulletBoom, bulletBoomSize, 0.4f); //0.4 - max time of Animation, loop == false
	}

	if (name == "tankObj") {
		const size_t tankBoomSize = sizeof(this->tankBoom) / sizeof(*this->tankBoom);
		Create(tankBoom, tankBoomSize, 1.5f, true); //1.5 - max time of Animation, loop == true
	}
}

void GameField::MonitoringAnim(const AnimBirth* ptr)
{
	//ptr - not used !!!
	auto Monitoring = [&](AnimPlayer& playerAnim)
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

	for (size_t i(0); i < 4; i++) {
		if (tankAnimArr[i].tankBirth != nullptr) {
			if (tankAnimArr[i].tankBirth->FinishTime())
				tankAnimArr[i].tankBirth = nullptr;
			else
				tankAnimArr[i].tankBirth->Update();
		}
	}
	return;
}

void GameField::MonitoringAnim(const AnimSkin* ptr)
{
	//ptr - not used !!!
	auto Monitoring = [&](AnimPlayer& playerAnim, Player* player)
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

	for (size_t i(0); i < 4; i++) {
		if (tankAnimArr[i].tankSkin != nullptr) {
			if (tankAnimArr[i].tankSkin->FinishTime())
				tankAnimArr[i].tankSkin = nullptr;
			else
				tankAnimArr[i].tankSkin->Update(tank[i].getPosObj());
		}
	}
	return;
}

void GameField::MonitoringAnim(const AnimBoom* ptr)
{
	//ptr - not used !!!
	auto Monitoring = [&](std::shared_ptr<AnimBoom>* boom, const size_t boomSize)
	{
		for (int i(0); i < boomSize; ++i) {
			if (boom[i] != nullptr) {
				if (boom[i]->FinishTime())
					boom[i] = nullptr;
				else
					boom[i]->Update();
			}
		}
	};

	const size_t bulletBoomSize(sizeof(this->bulletBoom) / sizeof(*this->bulletBoom));
	Monitoring(bulletBoom, bulletBoomSize);

	const size_t tankBoomSize(sizeof(this->tankBoom) / sizeof(*this->tankBoom));
	Monitoring(tankBoom, tankBoomSize);

	return;
}
