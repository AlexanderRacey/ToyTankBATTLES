#include <SFML/Graphics/Transformable.hpp>
#include <levelsystem.h>
#include "add_entity.h"
#include "animation.h"
#include "system_resources.h"
#include "components/cmp_actor_movevent.h"
#include "components/cmp_sprite.h"
#include "components/ActorMovementComponent.h"

using namespace sf;
using namespace std;

// Add player to start position of each level
shared_ptr<Entity> AddEntity::makePlayer(Scene* scene, const Vector2f& pos)
{
	auto player = scene->makeEntity();
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	player->addTag("player");

	auto tankAnimation = player->addComponent<AnimationComponent>(Vector2f(58.5f, 55.0f));
	Texture playerTexture = *Resources::load<Texture>("playerSpritesheet.png");
	tankAnimation->setSpritesheet(playerTexture);
	tankAnimation->setFrameCount(8);
	tankAnimation->setFrameTime(0.06f);

	player->addComponent<PlayerMovementComponent>();

	return player;
}

shared_ptr<Entity> AddEntity::makeEnemy(Scene* scene, const Vector2f& pos)
{
	auto enemy = scene->makeEntity();
	enemy->setPosition(pos);
	enemy->addTag("enemy");
	auto animation = enemy->addComponent<AnimationComponent>(Vector2f(83.0f, 80.0f));
	shared_ptr<Texture> s = Resources::load<Texture>("enemySpritesheet.png");
	animation->setSpritesheet(*s);
	animation->setScale(Vector2f(0.6, 0.6));
	animation->setFrameCount(8);
	animation->setFrameTime(0.10);
	enemy->addComponent<EnemyAiComponent>();

	return enemy;
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