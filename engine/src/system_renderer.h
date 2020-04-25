#pragma once
#include <SFML/Graphics.hpp>
//#include "cmp_sprite.h"

using namespace sf;
using namespace std;

namespace Renderer 
{
	void initialise(RenderWindow&);
	void shutdown();
	void update(const double&);
	void queue(const Drawable* s);
	void queueAnimation(const Sprite& sprite);
	void render();
};
