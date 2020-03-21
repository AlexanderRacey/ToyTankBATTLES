#include "engine.h"
#include "scene_settings.h"
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

static shared_ptr<Entity> player;

// Resources
Sprite titleSprite1;
Texture titleTexture1;
Vector2f targetCoords1;
Vector2u titleTextureSize1;

Sprite backgroundSprite1;
Texture backgroundTexture1;
Vector2u backgroundSize1;
Vector2u windowSizeSettings;

// Display settings title
void SettingsScene::SetTitle()
{
	titleTexture1 = *Resources::load<Texture>("titleSettings.png");
	float x = Engine::GetWindow().getSize().x;
	float y = Engine::GetWindow().getSize().y;
	titleTextureSize1 = titleTexture1.getSize();
	windowSizeSettings = Engine::GetWindow().getSize();
	float scaleX = (float)windowSizeSettings.x / titleTextureSize1.x;
	float scaleY = (float)windowSizeSettings.y / titleTextureSize1.y;
	targetCoords1 = { x, y };
	titleSprite1.setTexture(titleTexture1);
	titleSprite1.setPosition(windowSizeSettings.x / 2, windowSizeSettings.y / 3);
	titleSprite1.setOrigin(titleTextureSize1.x / 2, titleTextureSize1.y / 2);
}

// Display background
void SettingsScene::SetBackground()
{
	backgroundTexture1 = *Resources::load<Texture>("background.png");
	float x1 = Engine::GetWindow().getSize().x;
	float y1 = Engine::GetWindow().getSize().x;
	backgroundSize1 = backgroundTexture1.getSize();
	windowSizeSettings = Engine::GetWindow().getSize();
	float scaleX1 = (float)windowSizeSettings.x / backgroundSize1.x;
	float scaleY1 = (float)windowSizeSettings.y / backgroundSize1.y;
	backgroundSprite1.setTexture(backgroundTexture1);
	backgroundSprite1.setPosition(0, 0);
	backgroundSprite1.setScale(scaleX1, scaleY1);
	backgroundSprite1.setOrigin(0, 0);
}

// Load function
void SettingsScene::Load()
{
	// Display settings 
	cout << "Settings Load \n";
	{
		auto txtSettings = makeEntity();
		auto t = txtSettings->addComponent<TextComponent>("Settings");
		t->getText().setCharacterSize(54);
		t->getText().setOrigin(t->getText().getGlobalBounds().width / 2, t->getText().getGlobalBounds().height / 2);
		txtSettings->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 100.0f));
	}

	float x2 = Engine::getWindowSize().x;
	float y2 = Engine::getWindowSize().y;
	SetBackground();
	SetTitle();

	font.loadFromFile("res/fonts/OdibeeSans-Regular.ttf");

	settingsMenu[0].setFont(font);
	settingsMenu[0].setFillColor(Color(0, 168, 243, 255));
	settingsMenu[0].setString("Default");
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
void SettingsScene::UnLoad()
{
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().y;
	Engine::GetWindow().setView(View(FloatRect(0, 0, x2, y2)));
	Scene::UnLoad();
}

// Update scene
void SettingsScene::Update(const double& dt)
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

	if (Keyboard::isKeyPressed(Keyboard::R))
	{
		UnLoad();
		Load();
	}

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
				Engine::setNewWindowSize(Vector2u(1504, 864), 2);
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

void SettingsScene::Render()
{
	Scene::Render();

	Renderer::queue(&backgroundSprite1);
	Renderer::queue(&titleSprite1);

	// Display settings menu
	for (int j = 0; j < MAX_NUMBER_OF_SETTINGS; j++)
	{
		Renderer::queue(&settingsMenu[j]);
	}
}

void SettingsScene::MoveUp()
{
	if (selectedItemIndex2 - 1 >= 0)
	{
		settingsMenu[selectedItemIndex2].setFillColor(Color(255, 127, 39, 255));
		selectedItemIndex2--;
		settingsMenu[selectedItemIndex2].setFillColor(Color(0, 168, 243, 255));
	}
}

void SettingsScene::MoveDown()
{
	if (selectedItemIndex2 + 1 < MAX_NUMBER_OF_SETTINGS)
	{
		settingsMenu[selectedItemIndex2].setFillColor(Color(255, 127, 39, 255));
		selectedItemIndex2++;
		settingsMenu[selectedItemIndex2].setFillColor(Color(0, 168, 243, 255));
	}
}