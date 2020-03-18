// Main Class File
#include <SFML/Graphics.hpp>
#include "engine.h"
#include "game.h"

using namespace sf;
using namespace std;

int gameWidth = 800;
int gameHeight = 600;

MenuScene menu;

void Render(RenderWindow &window)
{
}

void Load()
{

}

void Update(RenderWindow &window)
{
	static Clock clock;
	float dt = clock.restart().asSeconds();

	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			window.close();
			return;
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		window.close();
	}
}

int main()
{
	Engine::Start(gameWidth, gameHeight, "Toy Tank Battles", &menu);
	Load();

	/*while (window.isOpen())
	{
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}
	return 0;*/
}