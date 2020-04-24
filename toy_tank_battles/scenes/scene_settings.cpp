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
#include "../components/cmp_music.h"
#include "../components/cmp_sound.h"
#include "../components/cmp_text.h"

using namespace std;
using namespace sf;


// Resources
Sprite titleSprite1;
Texture titleTexture1;
Vector2f targetCoords1;
Vector2u titleTextureSize1;

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
void SettingsScene::Load()
{
	// Display settings 
	cout << "Settings Load \n";

	float x2 = Engine::getWindowSize().x;
	float y2 = Engine::getWindowSize().y;
	SetBackground();
	SetTitle();

	font.loadFromFile("res/fonts/OdibeeSans-Regular.ttf");

	// Create settings menu
	settingsMenu[0].setFont(font);
	settingsMenu[0].setFillColor(Color(0, 168, 243, 255));
	settingsMenu[0].setString("1440 x 900");
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
	settingsMenu[4].setString("Music Volume Up");
	settingsMenu[4].setPosition(sf::Vector2f((x2 / 2) - 80, (y2 / 2) + 240));

	settingsMenu[5].setFont(font);
	settingsMenu[5].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[5].setString("Music Volume Down");
	settingsMenu[5].setPosition(sf::Vector2f((x2 / 2) - 80, (y2 / 2) + 280));

	settingsMenu[6].setFont(font);
	settingsMenu[6].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[6].setString("Sound Effect Volume Up");
	settingsMenu[6].setPosition(sf::Vector2f((x2 / 2) - 80, (y2 / 2) + 320));

	settingsMenu[7].setFont(font);
	settingsMenu[7].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[7].setString("Sound Effect Volume Down");
	settingsMenu[7].setPosition(sf::Vector2f((x2 / 2) - 80, (y2 / 2) + 360));

	settingsMenu[8].setFont(font);
	settingsMenu[8].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[8].setString("Return to Menu");
	settingsMenu[8].setPosition(sf::Vector2f((x2 / 2) - 80, (y2 / 2) + 440));

	selectedItemIndex2 = 0;
	setLoaded(true);
}

// Unload function
void SettingsScene::UnLoad()
{
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().y;
	Engine::GetWindow().setView(View(FloatRect(0, 0, x2, y2)));
	BackgroundSprite.reset();
	Background.reset();
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
				Engine::setNewWindowSize(Vector2u(1440, 900), 2);
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
				MusicPlayer().setVolume(MusicPlayer().getVolume() + 5.0f);
				break;
			case 5:
				MusicPlayer().setVolume(MusicPlayer().getVolume() - 5.0f);
				break;
			case 6:
				//SoundPlayer().setVolume(SoundPlayer().getVolume() + 5.0f);
				break;
			case 7:
				//SoundPlayer().setVolume(SoundPlayer().getVolume() - 5.0f);
				break;
			case 8:
				Engine::ChangeScene(&menu);
				this_thread::sleep_for(chrono::milliseconds(170));
				break;
		}
	}
}

void SettingsScene::Render()
{
	Scene::Render();

	Renderer::queue(BackgroundSprite.get());
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