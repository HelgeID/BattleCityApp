#ifndef SOUND_H
#define SOUND_H

#include <SFML\Audio.hpp>
#include <mutex>

static std::mutex mtx_snd;

class Sound
{
	enum Status { absent_mode, stopping_mode, moving_mode } mode{ absent_mode };

	sf::Sound *sound_shoot, *sound_bonus, *sound_takebonus, *sound_explosion_f, *sound_explosion_t;
	sf::Sound *sound_ms;//moving-stopping

	sf::Music music;
	sf::SoundBuffer movingBuff;
	sf::SoundBuffer stoppingBuff;
	sf::SoundBuffer shootBuff;
	sf::SoundBuffer bonusBuff;
	sf::SoundBuffer takebonusBuff;

	sf::SoundBuffer explosion_fBuff;
	sf::SoundBuffer explosion_tBuff;

	void OpenSnd(const std::string FILENAME)
	{
		if (!music.openFromFile(FILENAME))
			exit(EXIT_FAILURE);
		return;
	}

	void OpenSnd(const std::string FILENAME, sf::SoundBuffer& buffer)
	{
		if (!buffer.loadFromFile(FILENAME))
			exit(EXIT_FAILURE);
		return;
	}

public:
	Sound();
	~Sound();

	Status getStatus() const { return mode; }

	sf::SoundBuffer& getBufferSnd(const char* nameBuffSnd)
	{
		if (nameBuffSnd == "movingBuff")
			return movingBuff;
		if (nameBuffSnd == "stoppingBuff")
			return stoppingBuff;
		if (nameBuffSnd == "shootBuff")
			return shootBuff;
		if (nameBuffSnd == "bonusBuff")
			return bonusBuff;
		if (nameBuffSnd == "takebonusBuff")
			return takebonusBuff;
		if (nameBuffSnd == "explosion_fBuff")
			return explosion_fBuff;
		if (nameBuffSnd == "explosion_tBuff")
			return explosion_tBuff;
	}

	void StartMusic()
	{
		music.setLoop(false);
		music.play();
	}

	bool getMusic() const
	{
		if (music.getStatus() == sf::SoundSource::Status::Playing)
			return true;
		return false;
	}

	void Absent()
	{
		if (mode != absent_mode) {
			sound_ms->stop();
			sound_ms->resetBuffer();
			mode = absent_mode;
		}
	}

	void Moving()
	{
		if (p_no_sound || gameover)
			return;

		if (mode != moving_mode) {
			sound_ms->stop();
			sound_ms->setBuffer(movingBuff);
			sound_ms->setLoop(true);
			sound_ms->play();
			mode = moving_mode;
		}
		return;
	}

	void Stopping()
	{
		if (p_no_sound || gameover)
			return;

		if (mode != stopping_mode) {
			sound_ms->stop();
			sound_ms->setBuffer(stoppingBuff);
			sound_ms->setLoop(true);
			sound_ms->play();
			mode = stopping_mode;
		}
		return;
	}

	friend void ShootSnd(Sound* obj)
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

	friend void BonusSnd(Sound* obj)
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

	friend void TakeBonusSnd(Sound* obj)
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

	friend void Explosion_fSnd(Sound* obj)
	{
		if (p_no_sound || gameover || !no_close)
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

	friend void Explosion_tSnd(Sound* obj)
	{
		if (p_no_sound || gameover || !no_close)
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
};

inline Sound::Sound()
{
	sound_shoot = new sf::Sound();
	sound_bonus = new sf::Sound();
	sound_takebonus = new sf::Sound();
	sound_explosion_f = new sf::Sound();
	sound_explosion_t = new sf::Sound();
	sound_ms = new sf::Sound();

	std::string FILENAME("");

	//music
	FILENAME = "data/sounds/music.ogg"; OpenSnd(FILENAME);
	//moving
	FILENAME = "data/sounds/moving.ogg"; OpenSnd(FILENAME, movingBuff);
	//stopping
	FILENAME = "data/sounds/stopping.ogg"; OpenSnd(FILENAME, stoppingBuff);
	//shoot
	FILENAME = "data/sounds/shoot.ogg"; OpenSnd(FILENAME, shootBuff);
	//bonus
	FILENAME = "data/sounds/bonus.ogg"; OpenSnd(FILENAME, bonusBuff);
	//takebonus
	FILENAME = "data/sounds/takebonus.ogg"; OpenSnd(FILENAME, takebonusBuff);

	//explosion_flag
	FILENAME = "data/sounds/explosion_flag.ogg"; OpenSnd(FILENAME, explosion_fBuff);
	//explosion_tank
	FILENAME = "data/sounds/explosion_tank.ogg"; OpenSnd(FILENAME, explosion_tBuff);
}

inline Sound::~Sound()
{
	delete sound_shoot; delete sound_bonus; delete sound_takebonus; delete sound_explosion_f; delete sound_explosion_t;
	delete sound_ms;
}

#endif
