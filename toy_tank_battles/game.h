#include "scenes/scene_highScores.h"
#include "scenes/scene_level1.h"
#include "scenes/scene_level2.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_settings.h"

// Create game scenes
extern MenuScene menu;
extern Level1Scene level1;
extern Level2Scene level2;
extern SettingsScene settings;
extern HighScoresScene highscores;

/*
extern Texture blueTank;
extern Sprite playerTank;

extern Sprite backgroundSprite3;
extern Texture backgroundTexture3;
extern Vector2u backgroundSize3;
extern Vector2u windowSizeLevel1;*/
static shared_ptr<Entity> player;
//extern vector<shared_ptr<Texture>> picks;