#include <SFML/Graphics.hpp>
#include "scenes/scene_menu.h"
#include "engine.h"
#include "game.h"

using namespace sf;
using namespace std;

int gameWidth = 1440;
int gameHeight = 900;

MenuScene menu;
HowToPlayScene howtoplay;
SettingsScene settings;
Level1Scene level1;
Level2Scene level2;
HighScoresScene highscores;

// Start game
int main()
{
	Engine::Start(gameWidth, gameHeight, "Toy Tank Battles", &menu);
}