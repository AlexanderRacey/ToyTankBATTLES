#include "scenes/scene_menu.h"
#include "scenes/scene_highScores.h"
#include "scenes/scene_howToPlay.h"
#include "scenes/scene_level1.h"
#include "scenes/scene_level2.h"
#include "scenes/scene_level3.h"
#include "scenes/scene_winner.h"
#include "scenes/scene_settings.h"
#include "scenes/scene_gameOver.h"

// Create game scenes
extern MenuScene menu;
extern Level1Scene level1;
extern Level2Scene level2;
extern Level3Scene level3;
extern SettingsScene settings;
extern HighScoresScene highscores;
extern HowToPlayScene howtoplay;
extern GameOverScene gameover;
extern WinnerScene winner;

// Shared Entities
static shared_ptr<Texture> Background;
static unique_ptr<Sprite> BackgroundSprite;
static shared_ptr<Entity> player;
static shared_ptr<Entity> enemies;
static shared_ptr<Entity> playerTurret;
static shared_ptr<Entity> smoke;

// Sounds
extern MusicPlayer s1;
extern MusicPlayer s2;
extern MusicPlayer s3;
extern MusicPlayer s4;

// HUD items
extern int playerScore;
extern int playerHighScore;
