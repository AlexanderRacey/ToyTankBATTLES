#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <levelsystem.h>
#include <system_resources.h>
#include <thread>
#include "engine.h"
#include "maths.h"
#include "scene_level2.h"
#include "system_renderer.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "../add_entity.h"
#include "../game.h"

using namespace std;
using namespace sf;

/*Texture* house;
Sprite houseSprite;*/

/*
Texture blueTank2;
Sprite playerTank2;

Sprite backgroundSprite4;
Texture backgroundTexture4;
Vector2u backgroundSize4;
Vector2u windowSizeLevel1;

vector<shared_ptr<Texture>> picks;
*/
//shared_ptr<Entity> player;


//Display background
void Level2Scene::SetBackground()
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

void Level2Scene::Load()
{
	cout << " Scene 2 Load" << endl;
	ls::loadLevelFile("res/Level1test.txt", 90.0f);
	//SetBackground();

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
	cout << " Scene 2 Load Done" << endl;

	setLoaded(true);
}

void Level2Scene::UnLoad()
{
	// Get player position
	const auto pp = player->getPosition();
	if (ls::getTileAt(pp) == ls::END) 
	{
		Engine::ChangeScene((Scene*)&menu);
	}
	BackgroundSprite.reset();
	Background.reset();
	cout << "Scene 2 Unload" << endl;
	ls::unload();
	Scene::UnLoad();
}

void Level2Scene::Update(const double& dt)
{

}

void Level2Scene::Render()
{
	
	Renderer::queue(BackgroundSprite.get());
	for (auto& s : ls::_sprites)
	{
		Renderer::queue(s.get());
	}
	Scene::Render();
}