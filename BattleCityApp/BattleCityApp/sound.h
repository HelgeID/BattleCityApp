#ifndef SOUND_H
#define SOUND_H

#include <SFML\Audio.hpp>
class Sound
{
	enum Status { absent_mode, stopping_mode, moving_mode } mode{ absent_mode };

	sf::Sound* sound;
	sf::Sound* sound_ms;//moving-stopping

	sf::Music music;
	sf::SoundBuffer movingBuff;
	sf::SoundBuffer stoppingBuff;
	sf::SoundBuffer shootBuff;

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
		if (mode != stopping_mode) {
			sound_ms->stop();
			sound_ms->setBuffer(stoppingBuff);
			sound_ms->setLoop(true);
			sound_ms->play();
			mode = stopping_mode;
		}
		return;
	}

	void Shoot()
	{
		sound->setBuffer(shootBuff);
		sound->setLoop(false);
		sound->play();
		while (sound->getStatus() != sf::SoundSource::Status::Stopped)
			;
		return;
	}
};

inline Sound::Sound()
{
	sound_ms = new sf::Sound();
	sound = new sf::Sound();

	std::string FILENAME("");

	//music
	FILENAME = "data/sounds/music.ogg"; OpenSnd(FILENAME);
	//moving
	FILENAME = "data/sounds/moving.ogg"; OpenSnd(FILENAME, movingBuff);
	//stopping
	FILENAME = "data/sounds/stopping.ogg"; OpenSnd(FILENAME, stoppingBuff);
	//shoot
	FILENAME = "data/sounds/shoot.ogg"; OpenSnd(FILENAME, shootBuff);
}

inline Sound::~Sound()
{
	delete sound_ms;
	delete sound;
}

#endif
