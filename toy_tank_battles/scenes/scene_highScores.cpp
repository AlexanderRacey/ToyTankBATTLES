#include <SFML/Graphics.hpp>
#include <iostream>
#include <levelsystem.h>
#include <thread>
#include "engine.h"
#include "scene_highScores.h"
#include "../game.h"

using namespace std;
using namespace sf;

//static shared_ptr<Entity> player;

void HighScoresScene::Load()
{

}

void HighScoresScene::UnLoad()
{
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().y;
	Engine::GetWindow().setView(View(FloatRect(0, 0, x2, y2)));
	Scene::UnLoad();
}

void HighScoresScene::Update(const double& dt)
{
	Scene::Update(dt);
	Event event;
	while (Engine::GetWindow().pollEvent(event)) {
		if (event.type == Event::Resized) {
			UnLoad();
			Engine::GetWindow().setSize(Vector2u(event.size.width, event.size.height));
			Engine::GetWindow().display();
			Load();
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::R)) {
		UnLoad();
		Load();
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::ChangeScene(&menu);
	}
}

void HighScoresScene::Render()
{
	Scene::Render();
}