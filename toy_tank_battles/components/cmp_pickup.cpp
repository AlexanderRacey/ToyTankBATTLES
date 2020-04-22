#include "cmp_pickup.h"
#include "cmp_sprite.h"
#include "engine.h"
#include "../animation.h"
#include "cmp_health.h"
#include "cmp_actor_movement.h"
#include "cmp_bullet.h"
#include "cmp_text.h"

using namespace sf;
using namespace std;

PickupComponent::PickupComponent(Entity* p, int t) : Component(p), type(t) {};

void PickupComponent::update(double dt) 
{	
	const FloatRect boundingBox = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getGlobalBounds();
	auto player = Engine::findEntity("player");

	if (!player.empty())
	{
		auto sprites = player[0]->GetCompatibleComponent<AnimationComponent>();
		if (!sprites.empty())
		{
			const FloatRect playerBounds = sprites[0]->getSprite().getGlobalBounds();

			//delete pickup and give player power ups depending on type
			if (boundingBox.intersects(playerBounds))
			{
				switch (type)
				{
					case 0:
						//
						//example speed up player
						player[0]->GetCompatibleComponent<PlayerMovementComponent>()[0]->increaseSpeed(2.f);
						player[0]->GetCompatibleComponent<TextComponent>()[0]->SetText("Faster ++", true);
						player[0]->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Blue);
						break;
					case 1:
						//better health 
						player[0]->GetCompatibleComponent<HealthComponent>()[0]->addHealth(10);
						break;
					case 2:
						//better damage
						player[0]->GetCompatibleComponent<PlayerMovementComponent>()[0]->addDamage(5.f);
						player[0]->GetCompatibleComponent<TextComponent>()[0]->SetText("Damage Increased ++", true);
						player[0]->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Blue);
						break;
					case 3:
						//full health
						player[0]->GetCompatibleComponent<HealthComponent>()[0]->setHealth(100);
						player[0]->GetCompatibleComponent<TextComponent>()[0]->SetText("Full Health ++", true);
						player[0]->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Magenta);
						break;
					case 4:
						///parrot
						player[0]->GetCompatibleComponent<HealthComponent>()[0]->setInvincible();
						break;
				}
				//delete
				_parent->setForDelete();
			}
		}

	}
}

void PickupComponent::render(){}