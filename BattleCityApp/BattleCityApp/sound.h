//FINAL
#ifndef SOUND_H
#define SOUND_H

#include <SFML\Audio.hpp>
#include "general.hpp"
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

	friend void ShootSnd(Sound*, const int);
	friend void BonusSnd(Sound*, const int);
	friend void TakeBonusSnd(Sound*, const int);
	friend void Explosion_fSnd(Sound*, const int);
	friend void Explosion_tSnd(Sound*, const int);
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
