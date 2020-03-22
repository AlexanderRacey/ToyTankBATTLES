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

Sprite backgroundSprite3;
Texture backgroundTexture3;
Vector2u backgroundSize3;
Vector2u windowSizeLevel1;

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

void Level1Scene::Load()
{
	cout << " Scene 1 Load" << endl;
	ls::loadLevelFile("res/level1test.txt", 70.0f);

	SetBackground();

	//Set level to appear at middle of window
	auto ho = (Engine::getWindowSize().y/2) - ((ls::getHeight() * ls::getTileSize()) /2);
	auto wid = (Engine::getWindowSize().x / 2) - ((ls::getWidth() * ls::getTileSize()) / 2);
	ls::setOffset(Vector2f(wid, ho));

	//Simulate long loading times
	this_thread::sleep_for(chrono::milliseconds(3000));
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
	Scene::Render();
	//auto _sprites = ls::getSprites();
	Renderer::queue(&backgroundSprite3);
	ls::render(Engine::GetWindow());
	//Renderer::queue(&houseSprite);
}