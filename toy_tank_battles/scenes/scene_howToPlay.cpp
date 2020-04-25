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
#include "../components/cmp_sound.h"

using namespace std;
using namespace sf;

// Resources
Sprite titleSprite2;
Texture titleTexture2;
Vector2f targetCoords2;
Vector2u titleTextureSize2;
Vector2u windowSizeHowToPlay;

SoundPlayer moveSound;
SoundPlayer selectSound;

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
	s2.stop();
	s3.stop();
	s4.stop();
	s1.playing();

	// Display settings 
	cout << "How To Play Load \n";

	float x2 = Engine::getWindowSize().x;
	float y2 = Engine::getWindowSize().y;
	SetBackground();
	SetTitle();

	font.loadFromFile("res/fonts/OdibeeSans-Regular.ttf");

	// Create howToPlay list
	storyText.setFont(font);
	storyText.setFillColor(Color(255, 127, 39, 255));
	storyText.setString("The evil orange tanks have take over Toy Town\n           Destroy them all before it's too late!");
	storyText.setPosition(Vector2f((x2 / 2) - 200, (y2 / 2) - 40));

	instructionList.setFont(font);
	instructionList.setFillColor(Color::Black);
	instructionList.setString(" - Use the Up and Down keys to navigate the menus\n - Use ENTER to select an option\n - Use WASD keys to move your tank up, left, down and right\n - Use SPACE to fire your weapon");
	instructionList.setPosition(Vector2f((x2 / 2) - 200, (y2 / 2) + 60));

	instructionMenu[0].setFont(font);
	instructionMenu[0].setFillColor(Color(0, 168, 243, 255));
	instructionMenu[0].setString("Press ENTER to return to Menu");
	instructionMenu[0].setPosition(Vector2f((x2 / 2) - 140, (y2 / 2) + 240));

	selectedItemIndex3 = 0;
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
		selectSound.enemyFire(1, false);
		Engine::ChangeScene(&menu);
		this_thread::sleep_for(chrono::milliseconds(170));
	}
}

void HowToPlayScene::Render()
{
	Scene::Render();

	Renderer::queue(BackgroundSprite.get());
	Renderer::queue(&titleSprite2);
	Renderer::queue(&storyText);
	Renderer::queue(&instructionList);

	// Display settings menu
	for (int j = 0; j < MAX_NUMBER_OF_INSTRUCTIONS; j++)
	{
		Renderer::queue(&instructionMenu[j]);
	}
}
