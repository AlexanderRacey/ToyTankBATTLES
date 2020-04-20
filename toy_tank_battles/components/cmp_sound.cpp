#include "cmp_sound.h"
#include <engine.h>
#include <SFML/Audio.hpp>
#include <algorithm>

using namespace sf;
using namespace std;

SoundPlayer::SoundPlayer() : m_volume(8.0f) {}

// Player fire
void SoundPlayer::playerFire(const int title, bool loop)
{
	if (title == 0) 
	{
		if (m_music.openFromFile("res/sound/playerFire.ogg"))
		{
			m_music.setVolume(m_volume);
			m_music.play();
		}
		else
		{
			cout << "ERROR with music file." << endl;
		}
	}
}

// Enemy fire
void SoundPlayer::enemyFire(const int title, bool loop)
{
	if (title == 1) 
	{
		if (m_music.openFromFile("res/sound/enemyFire.ogg"))
		{
			m_music.setVolume(m_volume);
			m_music.play();
		}
		else
		{
			cout << "ERROR with music file." << endl;
		}
	}
}

// Smoke explosion
void SoundPlayer::smokeExplosion(const int title, bool loop)
{
	if (title == 2)
	{
		if (m_music.openFromFile("res/sound/smokeExplosion.ogg"))
		{
			m_music.setVolume(m_volume);
			m_music.play();
		}
		else
		{
			cout << "ERROR with music file." << endl;
		}
	}
}

void SoundPlayer::stop()
{
	m_music.stop();
}

void SoundPlayer::setPaused(bool paused)
{
	(paused) ? m_music.pause() : m_music.play();
}

bool SoundPlayer::playing() const
{
	return (m_music.getStatus() == SoundSource::Playing);
}

float SoundPlayer::getVolume() const
{
	return m_volume;
}