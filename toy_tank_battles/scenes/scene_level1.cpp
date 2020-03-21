// scene_menu.cpp
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

using namespace std;
using namespace sf;

Texture house;
Sprite houseSprite;

void Level1Scene::Load()
{
	cout << " Scene 1 Load" << endl;
	ls::loadLevelFile("res/level1test.txt", 70.0f);

	//Set level to appear at middle of window
	auto ho = (Engine::getWindowSize().y/2) - ((ls::getHeight() * ls::getTileSize()) /2);
	auto wid = (Engine::getWindowSize().x / 2) - ((ls::getWidth() * ls::getTileSize()) / 2);
	ls::setOffset(Vector2f(wid, ho));

	//Simulate long loading times
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	cout << " Scene 1 Load Done" << endl;

	house = ls::getTexture(ls::WALL);
	//*Resources::load<Texture>("BlueHouse.png");
	houseSprite.setTexture(house);
	houseSprite.setTextureRect(IntRect(0, 0, 100, 100));
	houseSprite.setPosition(Vector2f(wid, ho));
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

}

void Level1Scene::Render()
{
	//auto _sprites = ls::getSprites();
	ls::render(Engine::GetWindow());
	//Renderer::queue(&houseSprite);
	Scene::Render();
}