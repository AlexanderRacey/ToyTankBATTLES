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
#include "../components/cmp_sound.h"

using namespace std;
using namespace sf;

// Resources
Sprite titleSprite1;
Texture titleTexture1;
Vector2f targetCoords1;
Vector2u titleTextureSize1;
Vector2u windowSizeSettings;

SoundPlayer settingsSelect;
SoundPlayer settingsMove;

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
	screenSettingsText.setFont(font);
	screenSettingsText.setFillColor(Color::Black);
	screenSettingsText.setString("Screen Settings");
	screenSettingsText.setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) - 60));

	settingsMenu[0].setFont(font);
	settingsMenu[0].setFillColor(Color(0, 168, 243, 255));
	settingsMenu[0].setString("1440 x 900");
	settingsMenu[0].setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) - 20));

	settingsMenu[1].setFont(font);
	settingsMenu[1].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[1].setString("1504 x 846");
	settingsMenu[1].setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) + 20));

	settingsMenu[2].setFont(font);
	settingsMenu[2].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[2].setString("1920 x 1080");
	settingsMenu[2].setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) + 60));

	settingsMenu[3].setFont(font);
	settingsMenu[3].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[3].setString("Fullscreen");
	settingsMenu[3].setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) + 100));

	musicSettingsText.setFont(font);
	musicSettingsText.setFillColor(Color::Black);
	musicSettingsText.setString("Music Settings");
	musicSettingsText.setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) + 160));

	settingsMenu[4].setFont(font);
	settingsMenu[4].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[4].setString("Music Volume Up");
	settingsMenu[4].setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) + 200));

	settingsMenu[5].setFont(font);
	settingsMenu[5].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[5].setString("Music Volume Down");
	settingsMenu[5].setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) + 240));

	musicVolumeText.setFont(font);
	musicVolumeText.setFillColor(Color::Black);
	musicVolumeText.setString("Current Volume: " + to_string(s1.getVolume()));
	musicVolumeText.setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) + 280));

	settingsMenu[6].setFont(font);
	settingsMenu[6].setFillColor(Color(255, 127, 39, 255));
	settingsMenu[6].setString("Return to Menu");
	settingsMenu[6].setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) + 330));

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

	// Update music disply
	musicVolumeText.setString("Current Volume: " + to_string(s1.getVolume()));

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
				s1.setVolume(s1.getVolume() + 1.0f);
				s2.setVolume(s2.getVolume() + 1.0f);
				s3.setVolume(s3.getVolume() + 1.0f);
				s4.setVolume(s4.getVolume() + 1.0f);
				this_thread::sleep_for(chrono::milliseconds(170));
				break;
			case 5:
				s1.setVolume(s1.getVolume() - 1.0f);
				s2.setVolume(s2.getVolume() - 1.0f);
				s3.setVolume(s3.getVolume() - 1.0f);
				s4.setVolume(s4.getVolume() - 1.0f);
				this_thread::sleep_for(chrono::milliseconds(170));
				break;
			case 6:
				settingsSelect.enemyFire(1, false);
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
	Renderer::queue(&screenSettingsText);
	Renderer::queue(&musicSettingsText);
	Renderer::queue(&musicVolumeText);

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
		settingsMove.playerFire(0, false);
		settingsMenu[selectedItemIndex2].setFillColor(Color(255, 127, 39, 255));
		selectedItemIndex2--;
		settingsMenu[selectedItemIndex2].setFillColor(Color(0, 168, 243, 255));
	}
}

void SettingsScene::MoveDown()
{
	if (selectedItemIndex2 + 1 < MAX_NUMBER_OF_SETTINGS)
	{
		settingsMove.playerFire(0, false);
		settingsMenu[selectedItemIndex2].setFillColor(Color(255, 127, 39, 255));
		selectedItemIndex2++;
		settingsMenu[selectedItemIndex2].setFillColor(Color(0, 168, 243, 255));
	}
}