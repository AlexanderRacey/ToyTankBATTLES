#include <SFML/Graphics/Transformable.hpp>
#include <levelsystem.h>
#include "add_entity.h"
#include "system_resources.h"

using namespace sf;
using namespace std;

// Add player to start position of each level
shared_ptr<Entity> AddEntity::makePlayer(Scene* scene, const Vector2f& pos)
{
	auto player = scene->makeEntity();

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