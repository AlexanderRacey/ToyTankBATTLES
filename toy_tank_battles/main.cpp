#include <SFML/Graphics.hpp>
#include "scenes/scene_menu.h"
#include "engine.h"
#include "game.h"

using namespace sf;
using namespace std;

int gameWidth = 1280;
int gameHeight = 720;

MenuScene menu;
Level1Scene level1;
HighScoresScene highscores;
SettingsScene settings;

/*void Render(RenderWindow &window)
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

	// Press escape to close window
	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		window.close();
	}
}*/

int main()
{
	Engine::Start(gameWidth, gameHeight, "Toy Tank Battles", &menu);
	//Load();

	/*while (window.isOpen())
	{
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}
	return 0;*/
}