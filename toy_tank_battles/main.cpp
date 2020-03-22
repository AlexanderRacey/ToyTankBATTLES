#include <SFML/Graphics.hpp>
#include "scenes/scene_menu.h"
#include "engine.h"
#include "game.h"

using namespace sf;
using namespace std;

int gameWidth = 1280;
int gameHeight = 720;

MenuScene menu;
SettingsScene settings;
Level1Scene level1;
HighScoresScene highscores;

// Start game
int main()
{
	Engine::Start(gameWidth, gameHeight, "Toy Tank Battles", &menu);
}