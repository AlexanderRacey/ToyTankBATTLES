#include <SFML/Graphics/Transformable.hpp>
#include <levelsystem.h>
#include "add_entity.h"
#include "animation.h"
#include "system_resources.h"
#include "components/cmp_actor_movement.h"
#include "components/cmp_sprite.h"
#include "components/cmp_health.h"
#include "components/cmp_breakable.h"
#include "components/cmp_timed_component.h"
#include "components/cmp_text.h"

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
	player->addComponent<HealthComponent>();
	player->addComponent<BreakableComponent>();
	auto text = player->addComponent<TextComponent>("Player");
	text->setPosition({ -20, -70.f });


	return player;
}

// Create enemies
shared_ptr<Entity> AddEntity::makeEnemy(Scene* scene, const Vector2f& pos)
{
	auto enemy = scene->makeEntity();
	enemy->setPosition(pos);
	enemy->addTag("enemy");
	enemy->addComponent<HealthComponent>();
	auto animation = enemy->addComponent<EnemyAnimationComp>(Vector2f(57.5f, 55.0f));
	shared_ptr<Texture> s = Resources::load<Texture>("enemySpritesheet.png");
	animation->setSpritesheet(*s);
	animation->setFrameCount(8);
	animation->setFrameTime(0.06f);

	enemy->addComponent<EnemyAiComponent>();
	enemy->addComponent<BreakableComponent>();

	auto text = enemy->addComponent<TextComponent>("Enemy");
	text->setPosition({ -25, -70.f });

	return enemy;
}

// Create smoke
shared_ptr<Entity> AddEntity::makeSmoke(Scene* scene, const Vector2f& pos)
{
	auto smoke = scene->makeEntity();
	smoke->setPosition(pos);
	smoke->addTag("smoke");

	auto smokeAnimation = smoke->addComponent<AnimationComponent>(Vector2f(87.0f, 87.0f));
	Texture smokeTexture = *Resources::load<Texture>("smokeSpritesheet.png");
	smokeAnimation->setSpritesheet(smokeTexture);
	smokeAnimation->setFrameCount(30);
	smokeAnimation->setFrameTime(0.06f);
	smoke->addComponent<TimedComponent>();

	return smoke;
}