#pragma once
#include "SFML/Audio.hpp"
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;

// Create MusicPlayer class from NonCopyable
class MusicPlayer final : private NonCopyable
{
	public:
		MusicPlayer();
		~MusicPlayer() = default;
		MusicPlayer(const MusicPlayer&) = delete;
		const MusicPlayer& operator = (const MusicPlayer&) = delete;

		void play1(const int file, bool loop = false);
		void play2(const int file, bool loop = false);
		void play3(const int file, bool loop = false);
		void play4(const int file, bool loop = false);
		void stop();
		void setPaused(bool paused);
		bool playing() const;
		float getVolume(); 
		void setVolume(float volume);

	protected:
		float m_volume;
		Music m_music;
};