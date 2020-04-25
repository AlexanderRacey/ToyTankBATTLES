#pragma once
#include "SFML/Audio.hpp"
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;

// Create SoundPlayer class from NonCopyable
class SoundPlayer final : private NonCopyable
{
	public:
		SoundPlayer();
		~SoundPlayer() = default;
		SoundPlayer(const SoundPlayer&) = delete;
		const SoundPlayer& operator = (const SoundPlayer&) = delete;

		void playerFire(const int file, bool loop = false);
		void enemyFire(const int file, bool loop = false);
		void smokeExplosion(const int file, bool loop = false);
		void toySqueak(const int file, bool loop = false);
		void stop();
		void setPaused(bool paused);
		bool playing() const;
		float getVolume() const;

	private:
		float m_volume;
		Music m_music;
};