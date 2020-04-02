#include "cmp_pickup.h"
#include "cmp_sprite.h"
#include "engine.h"

using namespace sf;
using namespace std;

PickupComponent::PickupComponent(Entity* p, int t) : Component(p), type(t) {};

void PickupComponent::update(double dt) {

	const FloatRect boundingBox = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getGlobalBounds();

	auto player = Engine::findEntity("player")[0];

	const FloatRect playerBounds = player->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getGlobalBounds();

	//delete pickup and give player power ups depending on type
	if (boundingBox.intersects(playerBounds)) {
		switch (type) {
		case 0:
			//
			//example speed up player
			//player->GetCompatibleComponent<PlayerMovementComponent>()[]->setSpeed(1.f);
			break;
		case 1:
			// 
			break;
		case 2:
			//
			break;
		case 3:
			//
			break;
		case 4:
			// 
			break;
		}

		//delete
		_parent->setForDelete();
	}
}