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
#include "scene_winner.h"
#include "system_renderer.h"
#include "../add_entity.h"
#include "../components/cmp_breakable.h"

using namespace std;
using namespace sf;


// Resources
Sprite winnerSprite;
Texture winnerTexture;
Vector2f winnerTargetCoords;
Vector2u winnerTextureSize;
Vector2u windowSizeWinner;


// Display menu title
void WinnerScene::SetTitle()
{
	winnerTexture = *Resources::load<Texture>("titleWinnner.png");
	float x = Engine::GetWindow().getSize().x;
	float y = Engine::GetWindow().getSize().y;
	winnerTextureSize = winnerTexture.getSize();
	windowSizeWinner = Engine::GetWindow().getSize();
	float scaleX = (float)windowSizeWinner.x / winnerTextureSize.x;
	float scaleY = (float)windowSizeWinner.y / winnerTextureSize.y;
	winnerTargetCoords = { x, y };
	winnerSprite.setTexture(winnerTexture);
	winnerSprite.setPosition(windowSizeWinner.x / 2, windowSizeWinner.y / 3);
	winnerSprite.setOrigin(winnerTextureSize.x / 2, winnerTextureSize.y / 2);
}

// Display background
void WinnerScene::SetBackground()
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

void WinnerScene::Load()
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
	winnerText.setFont(font);
	winnerText.setFillColor(Color::Black);
	winnerText.setString("You defeated all enemy Tanks!");
	winnerText.setPosition(Vector2f((x2 / 2) - 155, (y2 / 2) + 40));

	winnerHighScoreText.setFont(font);
	winnerHighScoreText.setFillColor(Color::Black);
	winnerHighScoreText.setString("High Score: " + playerHighScore);
	winnerHighScoreText.setPosition(Vector2f((x2 / 2) - 155, (y2 / 2) + 80));

	winnerMenu[0].setFont(font);
	winnerMenu[0].setFillColor(Color(0, 168, 243, 255));
	winnerMenu[0].setString("Play Again");
	winnerMenu[0].setPosition(Vector2f((x2 / 2) - 155, (y2 / 2) + 160));

	winnerMenu[1].setFont(font);
	winnerMenu[1].setFillColor(Color(255, 127, 39, 255));
	winnerMenu[1].setString("Return to Main Menu");
	winnerMenu[1].setPosition(Vector2f((x2 / 2) - 165, (y2 / 2) + 200));
	
	selectedItemIndex5 = 0;
	setLoaded(true);
}

void WinnerScene::UnLoad()
{
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().y;
	Engine::GetWindow().setView(View(FloatRect(0, 0, x2, y2)));
	BackgroundSprite.reset();
	Background.reset();
	Scene::UnLoad();
	cout << "Game Over Unload" << endl;
}

void WinnerScene::Update(const double& dt)
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

	// Menu selection
	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		MoveUp();
		this_thread::sleep_for(chrono::milliseconds(170));
	}
	else if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		MoveDown();
		this_thread::sleep_for(chrono::milliseconds(170));
	}

	// Select options from menu
	else if (Keyboard::isKeyPressed(Keyboard::Enter))
	{
		switch (GetPressedItem())
		{
			case 0:
				playerScore = 0;
				Engine::ChangeScene(&level1);
				break;
			case 1:
				Engine::ChangeScene(&menu);
				this_thread::sleep_for(chrono::milliseconds(170));
				break;
		}
	}
}

void WinnerScene::Render()
{
	Scene::Render();

	Renderer::queue(BackgroundSprite.get());
	Renderer::queue(&winnerSprite);

	// Display settings menu
	for (int j = 0; j < MAX_NUMBER_OF_GOMSGS; j++)
	{
		Renderer::queue(&winnerMenu[j]);
	}
}

void WinnerScene::MoveUp()
{
	if (selectedItemIndex5 - 1 >= 0)
	{
		winnerMenu[selectedItemIndex5].setFillColor(Color(255, 127, 39, 255));
		selectedItemIndex5--;
		winnerMenu[selectedItemIndex5].setFillColor(Color(0, 168, 243, 255));
	}
}

void WinnerScene::MoveDown()
{
	if (selectedItemIndex5 + 1 < MAX_NUMBER_OF_GOMSGS)
	{
		winnerMenu[selectedItemIndex5].setFillColor(Color(255, 127, 39, 255));
		selectedItemIndex5++;
		winnerMenu[selectedItemIndex5].setFillColor(Color(0, 168, 243, 255));
	}
}