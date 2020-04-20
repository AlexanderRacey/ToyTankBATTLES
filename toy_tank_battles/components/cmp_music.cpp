#include "cmp_music.h"
#include <engine.h>
#include <SFML/Audio.hpp>
#include <algorithm>

using namespace sf;
using namespace std;

MusicPlayer::MusicPlayer() : m_volume(10.0f) {}

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
			//TODO this probably warns anyway
			std::cout << "ERROR with music file." << std::endl;
		}
	}
}
void MusicPlayer::play2(const int title, bool loop)
{
	if (title == 1) {
		if (m_music.openFromFile("res/sound/mainMusic.ogg"))
		{
			m_music.setVolume(m_volume);
			m_music.setLoop(loop);
			m_music.play();
		}
		else
		{
			//TODO this probably warns anyway
			std::cout << "ERROR with music file." << std::endl;
		}
	}
}

void MusicPlayer::play3(const int title, bool loop)
{
	if (title == 2) {
		if (m_music.openFromFile("res/sound/gameOver.ogg"))
		{
			m_music.setVolume(m_volume);
			m_music.setLoop(loop);
			m_music.play();
		}
		else
		{
			//TODO this probably warns anyway
			std::cout << "ERROR with music file." << std::endl;
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
	return (m_music.getStatus() == sf::SoundSource::Playing);
}

float MusicPlayer::getVolume() const
{
	return m_volume;
}