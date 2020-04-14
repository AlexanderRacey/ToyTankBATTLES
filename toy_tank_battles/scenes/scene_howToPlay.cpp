#include "engine.h"
#include "scene_howToPlay.h"
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

using namespace std;
using namespace sf;

// Resources
Sprite titleSprite2;
Texture titleTexture2;
Vector2f targetCoords2;
Vector2u titleTextureSize2;
/*
Sprite backgroundSprite2;
Texture backgroundTexture2;
Vector2u backgroundSize2;*/
Vector2u windowSizeHowToPlay;

// Display settings title
void HowToPlayScene::SetTitle()
{
	titleTexture2 = *Resources::load<Texture>("titleSettings.png");
	float x = Engine::GetWindow().getSize().x;
	float y = Engine::GetWindow().getSize().y;
	titleTextureSize2 = titleTexture2.getSize();
	windowSizeHowToPlay = Engine::GetWindow().getSize();
	float scaleX = (float)windowSizeHowToPlay.x / titleTextureSize2.x;
	float scaleY = (float)windowSizeHowToPlay.y / titleTextureSize2.y;
	targetCoords2 = { x, y };
	titleSprite2.setTexture(titleTexture2);
	titleSprite2.setPosition(windowSizeHowToPlay.x / 2, windowSizeHowToPlay.y / 3);
	titleSprite2.setOrigin(titleTextureSize2.x / 2, titleTextureSize2.y / 2);
}

// Display background
void HowToPlayScene::SetBackground()
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
void HowToPlayScene::Load()
{
	// Display settings 
	cout << "How To Play Load \n";

	float x2 = Engine::getWindowSize().x;
	float y2 = Engine::getWindowSize().y;
	SetBackground();
	SetTitle();

	font.loadFromFile("res/fonts/OdibeeSans-Regular.ttf");

	// Create settings menu
	settingsMenu[0].setFont(font);
	settingsMenu[0].setFillColor(Color(0, 168, 243, 255));
	settingsMenu[0].setString("These need to be replaced with instructions");
	settingsMenu[0].setPosition(sf::Vector2f((x2 / 2) - 80, (y2 / 2) + 40));

	settingsMenu[1].setFont(font);
	settingsMenu[1].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[1].setString("1504 x 846");
	settingsMenu[1].setPosition(sf::Vector2f((x2 / 2) - 80, (y2 / 2) + 80));

	settingsMenu[2].setFont(font);
	settingsMenu[2].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[2].setString("1920 x 1080");
	settingsMenu[2].setPosition(sf::Vector2f((x2 / 2) - 80, (y2 / 2) + 120));

	settingsMenu[3].setFont(font);
	settingsMenu[3].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[3].setString("Fullscreen");
	settingsMenu[3].setPosition(sf::Vector2f((x2 / 2) - 80, (y2 / 2) + 160));

	settingsMenu[4].setFont(font);
	settingsMenu[4].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[4].setString("Back to Title");
	settingsMenu[4].setPosition(sf::Vector2f((x2 / 2) - 80, (y2 / 2) + 200));

	selectedItemIndex2 = 0;
	setLoaded(true);
}

// Unload function
void HowToPlayScene::UnLoad()
{
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().y;
	Engine::GetWindow().setView(View(FloatRect(0, 0, x2, y2)));
	BackgroundSprite.reset();
	Background.reset();
	Scene::UnLoad();
}

// Update scene
void HowToPlayScene::Update(const double& dt)
{
	Scene::Update(dt);

	Event event;
	while (Engine::GetWindow().pollEvent(event))
	
	{
		if (event.type == Event::Resized) {
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
	else if (Keyboard::isKeyPressed(Keyboard::Enter)) 
	{
		switch (GetPressedItem())
		{
			case 0:
				Engine::setNewWindowSize(Vector2u(1280, 720), 2);
				this_thread::sleep_for(chrono::milliseconds(170));
				break;
			case 1:
				Engine::setNewWindowSize(Vector2u(1504, 864), 2);
				this_thread::sleep_for(chrono::milliseconds(170));
				break;
			case 2:
				Engine::setNewWindowSize(Vector2u(1920, 1080), 2);
				this_thread::sleep_for(chrono::milliseconds(170));
				break;
			case 3:
				Engine::setNewWindowSize(Vector2u(1920, 1080), 1);
				this_thread::sleep_for(chrono::milliseconds(170));
				break;
			case 4:
				Engine::ChangeScene(&menu);
				this_thread::sleep_for(chrono::milliseconds(170));
				break;
		}
	}
}

void HowToPlayScene::Render()
{
	Scene::Render();

	Renderer::queue(BackgroundSprite.get());
	Renderer::queue(&titleSprite2);

	// Display settings menu
	for (int j = 0; j < MAX_NUMBER_OF_SETTINGS; j++)
	{
		Renderer::queue(&settingsMenu[j]);
	}
}

void HowToPlayScene::MoveUp()
{
	if (selectedItemIndex2 - 1 >= 0)
	{
		settingsMenu[selectedItemIndex2].setFillColor(Color(255, 127, 39, 255));
		selectedItemIndex2--;
		settingsMenu[selectedItemIndex2].setFillColor(Color(0, 168, 243, 255));
	}
}

void HowToPlayScene::MoveDown()
{
	if (selectedItemIndex2 + 1 < MAX_NUMBER_OF_SETTINGS)
	{
		settingsMenu[selectedItemIndex2].setFillColor(Color(255, 127, 39, 255));
		selectedItemIndex2++;
		settingsMenu[selectedItemIndex2].setFillColor(Color(0, 168, 243, 255));
	}
}