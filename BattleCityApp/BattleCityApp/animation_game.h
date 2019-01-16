#pragma once
#include "animation.h"

struct Time
{
	sf::Clock frameClock, elapsedClock;
	sf::Time frameTime, elapsedTime;
	float maxTime = 0.f;

	bool FinishTime() const { return elapsedTime.asSeconds() >= maxTime; }
};

class AnimBirth : Animation, public Time
{
	Animation* animation;
	AnimatedObject *obj;
	enum AnimFlag { zero, one, two } animFlag;

public:
	AnimBirth(sf::Texture& texture, const sf::Vector2f position, const float maxTime = 4.f)
	{
		this->SetTexture(texture);
		this->maxTime = maxTime;

		animation = &*this;

		animation->AddFrame(sf::IntRect(256, 96, 16, 16));
		animation->AddFrame(sf::IntRect(272, 96, 16, 16));
		animFlag = zero;

		obj = new AnimatedObject(sf::seconds(0.12), true, true);

		obj->setPosition(position);
		obj->PlayAnimation(*animation);
		elapsedTime = elapsedClock.restart();
	}

	~AnimBirth()
	{
		delete obj;
	}

	void Update()
	{
		elapsedTime = elapsedClock.getElapsedTime();
		if (FinishTime())
			return;

		if (!obj->IsPlaying())
			obj->PlayAnimation();

		if (animFlag == zero && elapsedTime.asSeconds() >= 1.3f) {
			//obj->StopAnimation();
			animation->DelHeadFrame();
			animation->AddFrame(sf::IntRect(288, 96, 16, 16));
			animFlag = one;
		}

		else if (animFlag == one && elapsedTime.asSeconds() >= 2.6f) {
			//obj->StopAnimation();
			animation->DelHeadFrame();
			animation->AddFrame(sf::IntRect(304, 96, 16, 16));
			animFlag = two;
		}

		frameTime = frameClock.restart();

		obj->Update(frameTime);
		return;
	}

	AnimatedObject TakeAnim()
	{
		return *obj;
	}
} static *animBirth;

class AnimSkin : Animation, public Time
{
	Animation* animation;
	AnimatedObject *obj;

public:
	AnimSkin(sf::Texture& texture, const sf::Vector2f position, const float maxTime = 10.f)
	{
		this->SetTexture(texture);
		this->maxTime = maxTime;

		animation = &*this;

		this->AddFrame(sf::IntRect(256, 144, 16, 16));
		this->AddFrame(sf::IntRect(272, 144, 16, 16));

		obj = new AnimatedObject(sf::seconds(0.06), true, true);
		obj->setPosition(position);
		obj->PlayAnimation(*animation);
		elapsedTime = elapsedClock.restart();
	}

	~AnimSkin()
	{
		delete obj;
	}

	void Update(const sf::Vector2f position)
	{
		elapsedTime = elapsedClock.getElapsedTime();
		if (FinishTime())
			return;

		frameTime = frameClock.restart();
		obj->setPosition(position);
		obj->Update(frameTime);
		return;
	}

	AnimatedObject TakeAnim()
	{
		return *obj;
	}
} static *animSkin;

class AnimBoom : Animation, public Time
{
	Animation* animation;
	AnimatedObject *obj;

public:
	enum AnimChose { Small, Bigger };

	AnimBoom(sf::Texture& texture, const sf::Vector2f position, AnimChose chose, const float maxTime = 0.f)
	{
		this->SetTexture(texture);
		this->maxTime = maxTime;

		animation = &*this;

		if (chose == Small) {
			this->AddFrame(sf::IntRect(256, 128, 16, 16));
			this->AddFrame(sf::IntRect(272, 128, 16, 16));
			this->AddFrame(sf::IntRect(288, 128, 16, 16));
			obj = new AnimatedObject(sf::seconds(0.1f), true, false);
		}

		if (chose == Bigger) {
			this->AddFrame(sf::IntRect(304, 128, 32, 32));
			this->AddFrame(sf::IntRect(336, 128, 32, 32));
			obj = new AnimatedObject(sf::seconds(0.2f), true, true);
		}

		obj->setPosition(position);
		obj->PlayAnimation(*animation);
		elapsedTime = elapsedClock.restart();
	}

	~AnimBoom()
	{
		delete obj;
	}

	void Update()
	{
		elapsedTime = elapsedClock.getElapsedTime();
		if (FinishTime())
			return;

		frameTime = frameClock.restart();

		obj->Update(frameTime);
		return;
	}

	AnimatedObject TakeAnim()
	{
		return *obj;
	}
} static *animBoom;
