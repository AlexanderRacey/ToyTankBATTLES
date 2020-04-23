#include "engine.h"
#include "../game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <system_resources.h>
#include <levelsystem.h>
#include <iostream>
#include <thread>
#include "../components/cmp_pickup.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "../components/cmp_actor_movement.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_music.h"
#include "scene_gameOver.h"
#include "system_renderer.h"
#include "../add_entity.h"
#include "../components/cmp_breakable.h"

using namespace std;
using namespace sf;


// Resources
Sprite gameOverSprite;
Texture gameOverTexture;
Vector2f gameOverTargetCoords;
Vector2u gameOverTextureSize;
Vector2u windowSizeGameOver;


// Display menu title
void GameOverScene::SetTitle()
{
	gameOverTexture = *Resources::load<Texture>("titleGameOver.png");
	float x = Engine::GetWindow().getSize().x;
	float y = Engine::GetWindow().getSize().y;
	gameOverTextureSize = gameOverTexture.getSize();
	windowSizeGameOver = Engine::GetWindow().getSize();
	float scaleX = (float)windowSizeGameOver.x / gameOverTextureSize.x;
	float scaleY = (float)windowSizeGameOver.y / gameOverTextureSize.y;
	gameOverTargetCoords = { x, y };
	gameOverSprite.setTexture(gameOverTexture);
	gameOverSprite.setPosition(windowSizeGameOver.x / 2, windowSizeGameOver.y / 3);
	gameOverSprite.setOrigin(gameOverTextureSize.x / 2, gameOverTextureSize.y / 2);
}

// Display background
void GameOverScene::SetBackground()
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

void GameOverScene::Load()
{
	// Play music
	s1.stop();
	s2.stop();
	s3.play3(2, true);
	s3.playing();

	// Display settings 
	cout << "Game Over Load \n";

	float x2 = Engine::getWindowSize().x;
	float y2 = Engine::getWindowSize().y;
	SetBackground();
	SetTitle();

	font.loadFromFile("res/fonts/OdibeeSans-Regular.ttf");

	// Create game over menu
	
	gameOverMenu[0].setFont(font);
	gameOverMenu[0].setFillColor(Color(0, 168, 243, 255));
	gameOverMenu[0].setString("High Score: " + playerHighScore);
	gameOverMenu[0].setPosition(Vector2f((x2 / 2) - 155, (y2 / 2) + 40));

	gameOverMenu[1].setFont(font);
	gameOverMenu[1].setFillColor(Color(0, 168, 243, 255));
	gameOverMenu[1].setString("Press ENTER to Return to Menu");
	gameOverMenu[1].setPosition(Vector2f((x2 / 2) - 165, (y2 / 2) + 120));
	
	selectedItemIndex4 = 0;
	setLoaded(true);
}

void GameOverScene::UnLoad()
{
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().y;
	Engine::GetWindow().setView(View(FloatRect(0, 0, x2, y2)));
	BackgroundSprite.reset();
	Background.reset();
	Scene::UnLoad();
	cout << "Game Over Unload" << endl;
}

void GameOverScene::Update(const double& dt)
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

	// Reload scene
	if (Keyboard::isKeyPressed(Keyboard::R))
	{
		UnLoad();
		Load();
	}

	// Select options from menu
	else if (Keyboard::isKeyPressed(Keyboard::Enter))
	{
		Engine::ChangeScene(&menu);
		this_thread::sleep_for(chrono::milliseconds(170));
	}
}

void GameOverScene::Render()
{
	Scene::Render();

	Renderer::queue(BackgroundSprite.get());
	Renderer::queue(&gameOverSprite);

	// Display settings menu
	for (int j = 0; j < MAX_NUMBER_OF_GOMSGS; j++)
	{
		Renderer::queue(&gameOverMenu[j]);
	}
}

void GameOverScene::MoveUp()
{
	if (selectedItemIndex4 - 1 >= 0)
	{
		gameOverMenu[selectedItemIndex4].setFillColor(Color(255, 127, 39, 255));
		selectedItemIndex4--;
		gameOverMenu[selectedItemIndex4].setFillColor(Color(0, 168, 243, 255));
	}
}

void GameOverScene::MoveDown()
{
	if (selectedItemIndex4 + 1 < MAX_NUMBER_OF_GOMSGS)
	{
		gameOverMenu[selectedItemIndex4].setFillColor(Color(255, 127, 39, 255));
		selectedItemIndex4++;
		gameOverMenu[selectedItemIndex4].setFillColor(Color(0, 168, 243, 255));
	}
}