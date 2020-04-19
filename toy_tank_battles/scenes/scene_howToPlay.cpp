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
	titleTexture2 = *Resources::load<Texture>("titleHTP.png");
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
	instructionMenu[0].setFont(font);
	instructionMenu[0].setFillColor(Color(255, 127, 39, 255));
	instructionMenu[0].setString("You are the Blue Tank");
	instructionMenu[0].setPosition(Vector2f((x2 / 2) - 180, (y2 / 2) + 40));

	instructionMenu[1].setFont(font);
	instructionMenu[1].setFillColor(Color(255, 127, 39, 255));
	instructionMenu[1].setString("Eliminate all Orange Tanks!");
	instructionMenu[1].setPosition(Vector2f((x2 / 2) - 180, (y2 / 2) + 80));

	instructionMenu[2].setFont(font);
	instructionMenu[2].setFillColor(Color(255, 127, 39, 255));
	instructionMenu[2].setString(" ");
	instructionMenu[2].setPosition(Vector2f((x2 / 2) - 180, (y2 / 2) + 120));

	instructionMenu[3].setFont(font);
	instructionMenu[3].setFillColor(Color(255, 127, 39, 255));
	instructionMenu[3].setString(" - Use the WASD keys to move your tank");
	instructionMenu[3].setPosition(Vector2f((x2 / 2) - 180, (y2 / 2) + 160));

	instructionMenu[4].setFont(font);
	instructionMenu[4].setFillColor(Color(255, 127, 39, 255));
	instructionMenu[4].setString(" - Use the SPACE bar to fire your weapon");
	instructionMenu[4].setPosition(Vector2f((x2 / 2) - 180, (y2 / 2) + 200));

	instructionMenu[5].setFont(font);
	instructionMenu[5].setFillColor(Color(255, 127, 39, 255));
	instructionMenu[5].setString(" ");
	instructionMenu[5].setPosition(Vector2f((x2 / 2) - 180, (y2 / 2) + 240));

	instructionMenu[6].setFont(font);
	instructionMenu[6].setFillColor(Color(0, 168, 243, 255));
	instructionMenu[6].setString("Press ENTER to Return to Menu");
	instructionMenu[6].setPosition(Vector2f((x2 / 2) - 180, (y2 / 2) + 280));

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
	else if (Keyboard::isKeyPressed(Keyboard::Enter)) 
	{
		Engine::ChangeScene(&menu);
		this_thread::sleep_for(chrono::milliseconds(170));
	}
}

void HowToPlayScene::Render()
{
	Scene::Render();

	Renderer::queue(BackgroundSprite.get());
	Renderer::queue(&titleSprite2);

	// Display settings menu
	for (int j = 0; j < MAX_NUMBER_OF_INSTRUCTIONS; j++)
	{
		Renderer::queue(&instructionMenu[j]);
	}
}

void HowToPlayScene::MoveUp()
{
	if (selectedItemIndex2 - 1 >= 0)
	{
		instructionMenu[selectedItemIndex2].setFillColor(Color(255, 127, 39, 255));
		selectedItemIndex2--;
		instructionMenu[selectedItemIndex2].setFillColor(Color(0, 168, 243, 255));
	}
}

void HowToPlayScene::MoveDown()
{
	if (selectedItemIndex2 + 1 < MAX_NUMBER_OF_INSTRUCTIONS)
	{
		instructionMenu[selectedItemIndex2].setFillColor(Color(255, 127, 39, 255));
		selectedItemIndex2++;
		instructionMenu[selectedItemIndex2].setFillColor(Color(0, 168, 243, 255));
	}
}