#include "cmp_breakable.h"
#include "cmp_sprite.h"
#include <system_resources.h>

using namespace sf;
using namespace std;

BreakableComponent::BreakableComponent(Entity* p) : Component(p), _exploded(false), _timer(0) {};

void BreakableComponent::update(double dt) {
	//if timer was set by explosion subtract from timer
	if (_timer > 0) {
		_timer -= dt;
	}

	if (_exploded) {

		//if timer run out clear sprite delete
		if (_timer <= 0) {
			_parent->setForDelete();
		}
	}

}

//Set exploded when bullet destroys house
void BreakableComponent::setExploded() {
	_exploded = true;
	_timer = 1.f;
	//set sprite to explotion sprite
	_parent->GetCompatibleComponent<SpriteComponent>()[0]->setTexture(Resources::load<Texture>("smokeGrey4.png"));
	_parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(.70f, .70f);
	auto bounds = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getLocalBounds();
	_parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setOrigin(bounds.getSize().x/2, bounds.getSize().x/2);
}

bool BreakableComponent::isExploded() {
	return _exploded;
}