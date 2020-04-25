#include "cmp_music.h"
#include <engine.h>
#include <SFML/Audio.hpp>
#include <algorithm>

using namespace sf;
using namespace std;

MusicPlayer::MusicPlayer() : m_volume(8.0f) {}

void MusicPlayer::play1(const int title, bool loop)
{
	if (title == 0) 
	{
		if (m_music.openFromFile("res/sound/menuMusic.ogg"))
		{
			m_music.setVolume(m_volume);
			m_music.setLoop(loop);
			m_music.play();
		}
		else
		{
			cout << "ERROR with music file." << endl;
		}
	}
}

void MusicPlayer::play2(const int title, bool loop)
{
	if (title == 1) 
	{
		if (m_music.openFromFile("res/sound/mainMusic.ogg"))
		{
			m_music.setVolume(m_volume);
			m_music.setLoop(loop);
			m_music.play();
		}
		else
		{
			cout << "ERROR with music file." << endl;
		}
	}
}

void MusicPlayer::play3(const int title, bool loop)
{
	if (title == 2)
	{
		if (m_music.openFromFile("res/sound/gameOver.ogg"))
		{
			m_music.setVolume(m_volume);
			m_music.setLoop(loop);
			m_music.play();
		}
		else
		{
			cout << "ERROR with music file." << endl;
		}
	}
}

void MusicPlayer::play4(const int title, bool loop)
{
	if (title == 3)
	{
		if (m_music.openFromFile("res/sound/winFanfare.ogg"))
		{
			m_music.setVolume(m_volume);
			m_music.setLoop(loop);
			m_music.play();
		}
		else
		{
			cout << "ERROR with music file." << endl;
		}
	}
}

void MusicPlayer::stop()
{
	m_music.stop();
}

void MusicPlayer::setPaused(bool paused)
{
	(paused) ? m_music.pause() : m_music.play();
}

bool MusicPlayer::playing() const
{
	return (m_music.getStatus() == SoundSource::Playing);
}

float MusicPlayer::getVolume()
{
	return m_volume;
}

void MusicPlayer::setVolume(float volume)
{
	this->m_volume = volume;
	this->m_music.setVolume(this->m_volume);
}