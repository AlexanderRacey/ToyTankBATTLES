#include "add_entity.h"
#include "animation.h"
#include "system_resources.h"
#include "components/cmp_sprite.h"
#include <SFML/Graphics/Transformable.hpp>
#include <levelsystem.h>

using namespace sf;
using namespace std;

// Add player to start position of each level
shared_ptr<Entity> AddEntity::makePlayer(Scene* scene, const Vector2f& pos)
{
	auto player = scene->makeEntity();
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	player->addTag("player");

	auto animation = player->addComponent<AnimationComponent>(Vector2f(83.0f, 78.0f));
	Texture texture = *Resources::load<Texture>("playerSpritesheet.png");
	animation->setSpritesheet(texture);
	animation->setFrameCount(12);
	animation->setFrameTime(0.06f);

	return player;
}

// Add fake player1 to simulate movement for animations
shared_ptr<Entity> AddEntity::makeFakePlayer1(Scene* scene, const Vector2f& pos, const Vector2f& distance, float time)
{
	auto fakePlayer1 = scene->makeEntity();
	fakePlayer1->setPosition(pos);
	fakePlayer1->addTag("fakePlayer1");

	return fakePlayer1;
}

// Add fake player2 to simulate movement for animations
shared_ptr<Entity> AddEntity::makeFakePlayer2(Scene* scene, const Vector2f& pos, const Vector2f& distance, float time)
{
	auto fakePlayer2 = scene->makeEntity();
	fakePlayer2->setPosition(pos);
	fakePlayer2->addTag("fakePlayer2");

	return fakePlayer2;
}

// Create game walls
void AddEntity::makeWalls(Scene* scene) 
{
	auto walls = ls::findTiles(ls::WALL);
	for (auto w : walls) 
	{
		auto pos = ls::getTilePosition(w);
		pos += Vector2f(10.f, 10.f);
		auto e = scene->makeEntity();
		e->setPosition(pos);
	}
}

// Make health pickup
shared_ptr<Entity> AddEntity::makeHealthPickup(Scene* scene, const Vector2f& pos)
{
	auto makeHealthPickup = scene->makeEntity();
	makeHealthPickup->setPosition(pos);
	makeHealthPickup->addTag("health");

	auto animation = makeHealthPickup->addComponent<AnimationComponent>(Vector2f(30.0f, 30.0f));
	Texture texture = *Resources::load<Texture>("bear.png");
	animation->setSpritesheet(texture);
	animation->setFrameCount(1);
	animation->setFrameTime(0.06f);
	
	return makeHealthPickup;
}