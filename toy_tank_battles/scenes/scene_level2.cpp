#include "scene_level2.h"
#include "engine.h"
#include "../game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <system_resources.h>
#include <levelsystem.h>
#include <iostream>
#include <fstream>
#include <thread>
#include "../components/cmp_pickup.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "../components/cmp_actor_movement.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_music.h"
#include "../components/cmp_health.h"
#include "../components/cmp_breakable.h"
#include "system_renderer.h"
#include "../add_entity.h"


using namespace std;
using namespace sf;


// Player HUD
//int playerHighScore;
//int playerScore;


// Display background
void Level2Scene::SetBackground()
{
	_Background = Resources::load<Texture>("background.png");
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().x;
	Vector2u BackgroundSize = _Background->getSize();
	Vector2u windowSizeLevel1 = Engine::GetWindow().getSize();
	float scaleX2 = (float)windowSizeLevel1.x / BackgroundSize.x;
	float scaleY2 = (float)windowSizeLevel1.y / BackgroundSize.y;
	_BackgroundSprite = make_unique<sf::Sprite>();
	_BackgroundSprite->setTexture(*_Background);
	_BackgroundSprite->setPosition(0, 0);
	_BackgroundSprite->setScale(scaleX2, scaleY2);
	_BackgroundSprite->setOrigin(0, 0);
}

void Level2Scene::SetPickups()
{
	//make array of Pickup components based on number represented on map
	vector<shared_ptr<Texture>> picks = { Resources::load<Texture>("bear.png"), Resources::load<Texture>("giraffe.png"),
		Resources::load<Texture>("hippo.png"), Resources::load<Texture>("penguin.png"), Resources::load<Texture>("parrot.png")
	};

	auto pickups = ls::findTiles(ls::PICKUP);
	for (auto p : pickups) {
		int type = rand() % 5;
		Vector2f pos = ls::getTilePosition(p);
		auto e = makeEntity();
		e->setPosition(pos);

		e->addComponent<SpriteComponent>();
		e->GetCompatibleComponent<SpriteComponent>()[0]->setTexture(picks[type]);
		//e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(.35f, .35f);
		Vector2u TextureSize = e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getTexture()->getSize();
		float scaleX = (ls::getTileSize() / TextureSize.x) / 2.5;
		float scaleY = (ls::getTileSize() / TextureSize.y) / 2.5;
		e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(scaleX, scaleY);
		auto bounds = e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getLocalBounds();
		// not centered... not sure how to fix that
		e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setOrigin(bounds.getSize().x / 2, bounds.getSize().y / 2);
		// Add pickup component
		e->addComponent<PickupComponent>(type);
	}
}

//ADDs breakable houses to tiles
void Level2Scene::SetBreakables()
{
	auto brokenHouses = ls::findTiles(ls::BROKEN);
	auto brokenHouses2 = ls::findTiles(ls::BROKEN_R);
	brokenHouses.insert(end(brokenHouses), begin(brokenHouses2), end(brokenHouses2));
	int type;
	for (auto b : brokenHouses)
	{
		type = rand() % 3;
		auto pos = ls::getTilePosition(b);
		auto e = makeEntity();
		e->setPosition(pos);
		e->addTag("brokenHouse");
		e->addComponent<BreakableComponent>();

		//ADD house sprite
		e->addComponent<SpriteComponent>();
		if (type == 0) {
			e->GetCompatibleComponent<SpriteComponent>()[0]->setTexture(Resources::load<Texture>("BrokenH.png"));
		}
		else
		{
			e->GetCompatibleComponent<SpriteComponent>()[0]->setTexture(Resources::load<Texture>("BrokenH1.png"));
		}
		auto bounds = e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getGlobalBounds();
		e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setOrigin(bounds.getSize().x / 2, bounds.getSize().y / 2);
		//Rotate Broken houses on tile Broken_R
		if (ls::getTile(b) == ls::BROKEN_R)
		{
			e->setRotation(90.f);
		}
	}
}


void Level2Scene::Load()
{
	// Get window size
	float x2 = Engine::getWindowSize().x;
	float y2 = Engine::getWindowSize().y;
	Engine::GetWindow().setSize(Vector2u(x2, y2));
	Engine::GetWindow().display();

	// Load level
	ls::loadLevelFile("res/level_2.txt", 90.0f);

	//Set level to appear at middle of window
	auto ho = (Engine::getWindowSize().y / 2) - ((ls::getHeight() * ls::getTileSize()) / 2);
	auto wid = (Engine::getWindowSize().x / 2) - ((ls::getWidth() * ls::getTileSize()) / 2);
	ls::setOffset(Vector2f(wid, ho));

	// Create player object
	player = AddEntity::makePlayer(this, Vector2f(x2 / 2, y2 / 2));

	// Play music 
	s1.stop();
	s3.stop();
	s4.stop();
	s2.play2(1, true);

	if (!font.loadFromFile("res/fonts/OdibeeSans-Regular.ttf"))
	{
		cout << "Cannot load font!" << endl;
	}

	HUDtext.setString("Health: " + to_string(_playerHealth) + " / 100                                 " + "Score :  " + to_string(playerScore));
	HUDtext.setFont(font);
	HUDtext.setCharacterSize(50);
	HUDtext.setPosition(wid + 200, 10);
	HUDtext.setFillColor(Color(0, 168, 243, 255));

	// Create enemies
	auto enp = ls::findTiles(ls::ENEMY);
	for (auto e : enp)
	{
		auto pos = ls::getTilePosition(e);
		auto enemy = AddEntity::makeEnemy(this, pos);
	}

	// Load resources
	SetBackground();
	SetPickups();
	SetBreakables();

	// Simulate long loading times
	this_thread::sleep_for(chrono::milliseconds(3000));
	cout << "Level 2 Load Done" << endl;
	setLoaded(true);
}

void Level2Scene::UnLoad()
{
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().y;
	Engine::GetWindow().setView(View(FloatRect(0, 0, x2, y2)));

	// Reset player and remove pickups
	player.reset();
	playerTurret.reset();
	//picks.clear();
	_Background.reset();
	_BackgroundSprite.reset();

	// Finish Unload
	ls::unload();
	cout << "Level 1 Unload" << endl;
	Scene::UnLoad();
}

void Level2Scene::Update(const double& dt)
{
	auto player = Engine::findEntity("player")[0];
	int health = player->GetCompatibleComponent<HealthComponent>()[0]->getHealth();
	_playerHealth = health;
	_playerScore = playerScore;
	// Update HUD
	HUDtext.setString("Health: " + to_string(_playerHealth) + " / 100                                 " + "Score :  " + to_string(_playerScore));

	//change into next level if all enemies are defeated
	vector<shared_ptr<Entity>> enemies = Engine::findEntity("enemy");
	if (enemies.empty())
	{
		UnLoad();
		Engine::ChangeScene((Scene*)&level3);
	}
	else if (_playerHealth < 1)
	{
		// Plays gameOver scene if player is dead
		//set score
		playerScore = _playerScore;
		if (playerScore > playerHighScore) {
			playerHighScore = playerScore;
		}
		this_thread::sleep_for(chrono::milliseconds(200));
		Engine::ChangeScene((Scene*)&gameover);
		_playerHealth = 100;
	}

	// Reset scene
	if (Keyboard::isKeyPressed(Keyboard::R))
	{
		UnLoad();
		Load();
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		Engine::ChangeScene(&menu);
		this_thread::sleep_for(chrono::milliseconds(200));
	}

	// Update scene
	Scene::Update(dt);
}

void Level2Scene::Render()
{
	Renderer::queue(_BackgroundSprite.get());
	Renderer::queue(&HUDtext);
	for (auto& s : ls::_sprites)
	{
		Renderer::queue(s.get());
	}
	Scene::Render();
}

