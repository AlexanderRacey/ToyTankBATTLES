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

static shared_ptr<Texture> Background;
static unique_ptr<Sprite> BackgroundSprite;
static shared_ptr<Entity> player;