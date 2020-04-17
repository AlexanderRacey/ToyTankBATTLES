#include <SFML/Graphics/Transformable.hpp>
#include <levelsystem.h>
#include "add_entity.h"
#include "animation.h"
#include "system_resources.h"
#include "components/cmp_actor_movement.h"
#include "components/cmp_sprite.h"

using namespace sf;
using namespace std;

// Add player to start position of each level
shared_ptr<Entity> AddEntity::makePlayer(Scene* scene, const Vector2f& pos)
{
	auto player = scene->makeEntity();
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	player->addTag("player");

	auto tankAnimation = player->addComponent<AnimationComponent>(Vector2f(57.5f, 54.0f));
	Texture playerTexture = *Resources::load<Texture>("playerSpritesheet.png");
	tankAnimation->setSpritesheet(playerTexture);
	tankAnimation->setFrameCount(8);
	tankAnimation->setFrameTime(0.06f);

	player->addComponent<PlayerMovementComponent>();

	return player;
}

// Create enemies
shared_ptr<Entity> AddEntity::makeEnemy(Scene* scene, const Vector2f& pos)
{
	auto enemy = scene->makeEntity();
	enemy->setPosition(pos);
	enemy->addTag("enemy");
	//Add Turret 
	/*
	auto turret = enemy->addComponent<SpriteComponent>();
	turret->setTexture(Resources::load<Texture>("enemyTurret.png"));
	auto bounds = turret->getSprite().getGlobalBounds();
	turret->getSprite().setOrigin(bounds.getSize().x / 2, bounds.getSize().y);
	*/
	auto animation = enemy->addComponent<EnemyAnimationComp>(Vector2f(83.0f, 80.0f));
	shared_ptr<Texture> s = Resources::load<Texture>("enemySpritesheet.png");
	animation->setSpritesheet(*s);
	animation->setScale(Vector2f(0.6, 0.6));
	animation->setFrameCount(8);
	animation->setFrameTime(0.10);

	enemy->addComponent<EnemyAiComponent>();

	return enemy;
}