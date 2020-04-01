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
#include "../components/cmp_text.h"
#include "../game.h"
#include "../components/cmp_sprite.h"

using namespace std;
using namespace sf;

Sprite backgroundSprite3;
shared_ptr<Texture> bear;
Texture backgroundTexture3;
Vector2u backgroundSize3;
Vector2u windowSizeLevel1;
Sprite bearSprite;

vector<shared_ptr<Texture>> picks;


// Display background
void Level1Scene::SetBackground()
{
	backgroundTexture3 = *Resources::load<Texture>("background.png");
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().x;
	backgroundSize3 = backgroundTexture3.getSize();
	windowSizeLevel1 = Engine::GetWindow().getSize();
	float scaleX2 = (float)windowSizeLevel1.x / backgroundSize3.x;
	float scaleY2 = (float)windowSizeLevel1.y / backgroundSize3.y;
	backgroundSprite3.setTexture(backgroundTexture3);
	backgroundSprite3.setPosition(0, 0);
	backgroundSprite3.setScale(scaleX2, scaleY2);
	backgroundSprite3.setOrigin(0, 0);
}

void Level1Scene::SetPickups() {
	//make array of Pickup components based on number represented on map
	picks = { Resources::load<Texture>("bear.png"), Resources::load<Texture>("chick.png"), Resources::load<Texture>("giraffe.png"),
		Resources::load<Texture>("hippo.png"), Resources::load<Texture>("penguin.png")
	};

	auto pickups = ls::findTiles(ls::PICKUP);
	for (auto p : pickups) {
		auto pos = ls::getTilePosition(p);
		cout << "tile pos " << pos << endl;
	//	pos += Vector2f(ls::getTileSize() / 2, ls::getTileSize() / 2);
		cout << "tile pos " << pos << endl;
		auto e = makeEntity();
		e->setPosition(pos);
		e->addComponent<SpriteComponent>();
		e->GetCompatibleComponent<SpriteComponent>()[0]->setTexture(picks[rand() % 5]);
		e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(.35f, .35f);
		auto bounds = e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getGlobalBounds();
		//not centered... not sure how to fix that
		e->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setOrigin(bounds.getSize());
	}

}

void Level1Scene::Load()
{
	cout << " Scene 1 Load" << endl;
	ls::loadLevelFile("res/Level1test.txt", 90.0f);
	SetBackground();

	//Set level to appear at middle of window
	//this is not the middle anymore will need to figure somethings out
	auto ho = (Engine::getWindowSize().y/2) - ((ls::getHeight() * ls::getTileSize()) /2);
	auto wid = (Engine::getWindowSize().x / 2) - ((ls::getWidth() * ls::getTileSize()) / 2);
	ls::setOffset(Vector2f(wid, ho));

	//Simulate long loading times
	this_thread::sleep_for(chrono::milliseconds(3000));
	cout << " Scene 1 Load Done" << endl;
	SetPickups();

	setLoaded(true);
}

void Level1Scene::UnLoad()
{
	cout << "Scene 1 Unload" << endl;
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