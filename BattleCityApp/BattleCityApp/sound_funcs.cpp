//FINAL
#include "sound.h"

void ShootSnd(Sound* obj, const int value)
{
	if (p_no_sound || gameover || !no_close)
		return;

	while (obj->sound_shoot->getStatus() != sf::SoundSource::Status::Stopped && no_close)
		;
	{
		std::lock_guard<std::mutex> lg(mtx_snd);
		obj->sound_shoot->setBuffer(obj->getBufferSnd("shootBuff"));
		obj->sound_shoot->setLoop(false);
		obj->sound_shoot->play();
	}
	return;
}

void BonusSnd(Sound* obj, const int value)
{
	if (p_no_sound || gameover || !no_close)
		return;

	while (obj->sound_bonus->getStatus() != sf::SoundSource::Status::Stopped && no_close)
		;
	{
		std::lock_guard<std::mutex> lg(mtx_snd);
		obj->sound_bonus->setBuffer(obj->getBufferSnd("bonusBuff"));
		obj->sound_bonus->setLoop(false);
		obj->sound_bonus->play();
	}
	return;
}

void TakeBonusSnd(Sound* obj, const int value)
{
	if (p_no_sound || gameover || !no_close)
		return;

	while (obj->sound_takebonus->getStatus() != sf::SoundSource::Status::Stopped && no_close)
		;
	{
		std::lock_guard<std::mutex> lg(mtx_snd);
		obj->sound_takebonus->setBuffer(obj->getBufferSnd("takebonusBuff"));
		obj->sound_takebonus->setLoop(false);
		obj->sound_takebonus->play();
	}
	return;
}

void Explosion_fSnd(Sound* obj, const int value)
{
	if (p_no_sound || !no_close)
		return;

	while (obj->sound_explosion_f->getStatus() != sf::SoundSource::Status::Stopped && no_close)
		;
	{
		std::lock_guard<std::mutex> lg(mtx_snd);
		obj->sound_explosion_f->setBuffer(obj->getBufferSnd("explosion_fBuff"));
		obj->sound_explosion_f->setLoop(false);
		obj->sound_explosion_f->play();
	}
	return;
}

void Explosion_tSnd(Sound* obj, const int value)
{
	if (p_no_sound || !no_close)
		return;

	while (obj->sound_explosion_t->getStatus() != sf::SoundSource::Status::Stopped && no_close)
		;
	{
		std::lock_guard<std::mutex> lg(mtx_snd);
		obj->sound_explosion_t->setBuffer(obj->getBufferSnd("explosion_tBuff"));
		obj->sound_explosion_t->setLoop(false);
		obj->sound_explosion_t->play();
	}
	return;
}
