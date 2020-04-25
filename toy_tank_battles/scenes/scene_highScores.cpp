#include "engine.h"
#include "scene_highScores.h"
#include "../game.h"
#include "system_renderer.h"
#include "system_resources.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "../components/cmp_text.h"
#include "../components/cmp_sound.h"


using namespace std;
using namespace sf;

// Resources
Sprite highScoreSprite;
Texture highScoreTexture;
Vector2f targetCoordsHighScore;
Vector2u titleHighScoreSize;
Vector2u windowSizeHighScore;

SoundPlayer highScoreSound;

// Display settings title
void HighScoresScene::SetTitle()
{
	highScoreTexture = *Resources::load<Texture>("titleHighScores.png");
	float x = Engine::GetWindow().getSize().x;
	float y = Engine::GetWindow().getSize().y;
	titleHighScoreSize = highScoreTexture.getSize();
	windowSizeHighScore = Engine::GetWindow().getSize();
	float scaleX = (float)windowSizeHighScore.x / titleHighScoreSize.x;
	float scaleY = (float)windowSizeHighScore.y / titleHighScoreSize.y;
	targetCoordsHighScore = { x, y };
	highScoreSprite.setTexture(highScoreTexture);
	highScoreSprite.setPosition(windowSizeHighScore.x / 2, windowSizeHighScore.y / 3);
	highScoreSprite.setOrigin(titleHighScoreSize.x / 2, titleHighScoreSize.y / 2);
}

// Set background
void HighScoresScene::SetBackground() 
{
	Background = Resources::load<Texture>("background.png");
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().x;
	Vector2u BackgroundSize = Background->getSize();
	Vector2u windowSizeLevel1 = Engine::GetWindow().getSize();
	float scaleX2 = (float)windowSizeLevel1.x / BackgroundSize.x;
	float scaleY2 = (float)windowSizeLevel1.y / BackgroundSize.y;
	BackgroundSprite = make_unique<sf::Sprite>();
	BackgroundSprite->setTexture(*Background);
	BackgroundSprite->setPosition(0, 0);
	BackgroundSprite->setScale(scaleX2, scaleY2);
	BackgroundSprite->setOrigin(0, 0);
}

// Load function
void HighScoresScene::Load()
{
	s2.stop();
	s3.stop();
	s4.stop();
	s1.playing();
	SetBackground();

	// Display settings 
	cout << "HighScore Load \n";

	float x2 = Engine::getWindowSize().x;
	float y2 = Engine::getWindowSize().y;
	SetBackground();
	SetTitle();

	font.loadFromFile("res/fonts/OdibeeSans-Regular.ttf");

	// Create howToPlay list
	highScoreText.setFont(font);
	highScoreText.setFillColor(Color::Black);
	highScoreText.setString("[Coming Soon]");
	highScoreText.setPosition(Vector2f((x2 / 2) - 70, (y2 / 2)));

	highScoreMenu[0].setFont(font);
	highScoreMenu[0].setFillColor(Color(0, 168, 243, 255));
	highScoreMenu[0].setString("Press ENTER to return to Menu");
	highScoreMenu[0].setPosition(Vector2f((x2 / 2) - 140, (y2 / 2) + 140));

	selectedItemIndex6 = 0;
	setLoaded(true);
}

// Unload function
void HighScoresScene::UnLoad()
{
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().y;
	BackgroundSprite.reset();
	Background.reset();
	Engine::GetWindow().setView(View(FloatRect(0, 0, x2, y2)));
	Scene::UnLoad();
}

// Update function
void HighScoresScene::Update(const double& dt)
{
	Scene::Update(dt);
	Event event;
	while (Engine::GetWindow().pollEvent(event))
	{
		if (event.type == Event::Resized)
		{
			UnLoad();
			Engine::GetWindow().setSize(Vector2u(event.size.width, event.size.height));
			Engine::GetWindow().display();
			Load();
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::R)) 
	{
		UnLoad();
		Load();
	}

	if (Keyboard::isKeyPressed(Keyboard::Enter)) 
	{
		highScoreSound.enemyFire(1, false);
		Engine::ChangeScene(&menu);
		this_thread::sleep_for(chrono::milliseconds(170));
	}
}

void HighScoresScene::Render()
{
	Scene::Render();

	Renderer::queue(BackgroundSprite.get());
	Renderer::queue(&highScoreSprite);
	Renderer::queue(&highScoreText);

	// Display highScore menu
	for (int j = 0; j < MAX_NUMBER_OF_INSTRUCTIONS; j++)
	{
		Renderer::queue(&highScoreMenu[j]);
	}
}