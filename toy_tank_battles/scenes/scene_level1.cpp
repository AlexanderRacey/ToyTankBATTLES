#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <levelsystem.h>
#include <system_resources.h>
#include <thread>
#include "engine.h"
#include "maths.h"
#include "scene_level1.h"
#include "system_renderer.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "../add_entity.h"
#include "../game.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_pickup.h"
#include "../components/cmp_breakable.h"

using namespace std;
using namespace sf;

Texture blueTank1;
Sprite playerTank1;

Sprite backgroundSprite3;
Texture backgroundTexture3;
Vector2u backgroundSize3;



// Display background
void Level1Scene::SetBackground()
{
	backgroundTexture3 = *Resources::load<Texture>("background.png");
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().x;
	backgroundSize3 = backgroundTexture3.getSize();
	Vector2u windowSizeLevel1 = Engine::GetWindow().getSize();
	float scaleX2 = (float)windowSizeLevel1.x / backgroundSize3.x;
	float scaleY2 = (float)windowSizeLevel1.y / backgroundSize3.y;
	backgroundSprite3.setTexture(backgroundTexture3);
	backgroundSprite3.setPosition(0, 0);
	backgroundSprite3.setScale(scaleX2, scaleY2);
	backgroundSprite3.setOrigin(0, 0);
}

void Level1Scene::SetPickups() {
	//make array of Pickup components based on number represented on map
	vector<shared_ptr<Texture>> picks = { Resources::load<Texture>("bear.png"), Resources::load<Texture>("giraffe.png"),
		Resources::load<Texture>("hippo.png"), Resources::load<Texture>("penguin.png"), Resources::load<Texture>("parrot.png")
	};

	auto pickups = ls::findTiles(ls::PICKUP);
	for (auto p : pickups) {
		int type = rand() % 5;
		auto pos = ls::getTilePosition(p);
		auto e = makeEntity();
		e->setPosition(pos);
		
		e->addComponent<SpriteComponent>();
		e->GetCompatibleComponent<SpriteComponent>()[0]->setTexture(picks[type]);
		e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(.35f, .35f);
		auto bounds = e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getGlobalBounds();
		//not centered... not sure how to fix that
		e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setOrigin(bounds.getSize());
		//Add pickup component
		e->addComponent<PickupComponent>(type);
	}

}

//ADDs breakable houses to tiles
void Level1Scene::SetBreakables() {

	auto brokenHouses = ls::findTiles(ls::BROKEN);
	auto brokenHouses2 = ls::findTiles(ls::BROKEN_R);
	brokenHouses.insert(end(brokenHouses), begin(brokenHouses2), end(brokenHouses2));
	int type;
	for (auto b : brokenHouses) {
		type = rand() % 3;
		auto pos = ls::getTilePosition(b);
		auto e = makeEntity();
		e->setPosition(pos);
		//ADD Breakable Component
		e->addComponent<BreakableComponent>();
		//ADD house sprite
		e->addComponent<SpriteComponent>();
		if (type == 0) {
			e->GetCompatibleComponent<SpriteComponent>()[0]->setTexture(Resources::load<Texture>("BrokenH.png"));
		}
		else {
			e->GetCompatibleComponent<SpriteComponent>()[0]->setTexture(Resources::load<Texture>("BrokenH1.png"));
		}
		auto bounds = e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getGlobalBounds();
		e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setOrigin(bounds.getSize().x/2, bounds.getSize().y / 2);
		//Rotate Broken houses on tile Broken_R
		if (ls::getTile(b) == ls::BROKEN_R) {
			e->setRotation(90.f);
		}

	}
	
}

void Level1Scene::Load()
{
	cout << " Scene 1 Load" << endl;
	ls::loadLevelFile("res/Level1test.txt", 90.0f);
	SetBackground();

	// Get window size
	float x2 = Engine::getWindowSize().x;
	float y2 = Engine::getWindowSize().y;
	Engine::GetWindow().setSize(Vector2u(x2, y2));
	Engine::GetWindow().display();

	//Set level to appear at middle of window
	//this is not the middle anymore will need to figure somethings out
	auto ho = (Engine::getWindowSize().y/2) - ((ls::getHeight() * ls::getTileSize()) /2);
	auto wid = (Engine::getWindowSize().x / 2) - ((ls::getWidth() * ls::getTileSize()) / 2);
	ls::setOffset(Vector2f(wid, ho));

	// Create player object
	player = AddEntity::makePlayer(this, Vector2f(x2 / 2, y2 / 2));

	//Simulate long loading times
	this_thread::sleep_for(chrono::milliseconds(3000));
	cout << " Scene 1 Load Done" << endl;
	SetPickups();
	SetBreakables();
	setLoaded(true);
}

void Level1Scene::UnLoad()
{
	// Get player position
	const auto pp = player->getPosition();
	if (ls::getTileAt(pp) == ls::END) 
	{
		Engine::ChangeScene((Scene*)&level2);
	}
	else if (!player->isAlive()) 
	{
		this_thread::sleep_for(chrono::milliseconds(200));
		Engine::ChangeScene((Scene*)&level1);
	}

	cout << "Scene 1 Unload" << endl;
   // picks.clear();
	ls::unload();
	Scene::UnLoad();
}

void Level1Scene::Update(const double& dt)
{
	Scene::Update(dt);
}

void Level1Scene::Render()
{
	
	Renderer::queue(&backgroundSprite3);
	for (auto& s : ls::_sprites)
	{
		Renderer::queue(s.get());
	}
	Scene::Render();

}