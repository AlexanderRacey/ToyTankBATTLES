#include <SFML/Graphics.hpp>
#include <iostream>
#include <levelsystem.h>
#include <thread>
#include "engine.h"
#include "scene_highScores.h"
#include "../game.h"
#include <system_resources.h>

using namespace std;
using namespace sf;

//static shared_ptr<Entity> player;

void HighScoresScene::Load()
{
	s2.stop();
	s3.stop();
	s1.playing();
	SetBackground();
}

void HighScoresScene::SetBackground() {
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

void HighScoresScene::UnLoad()
{
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().y;
	BackgroundSprite.reset();
	Background.reset();
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