#include "cmp_breakable.h"
#include "../animation.h"
#include "../add_entity.h"
#include "cmp_sprite.h"
#include "../game.h"
#include <system_resources.h>
#include "../animation.h"

using namespace sf;
using namespace std;

BreakableComponent::BreakableComponent(Entity* p) : Component(p), _exploded(false), _timer(0) {};

void BreakableComponent::update(double dt)
{
	// If timer was set by explosion subtract from timer
	if (_timer > 0)
	{
		_timer -= dt;
	}

	if (_exploded)
	{
		// If timer run out clear sprite delete
		if (_timer <= 0)
		{
			_parent->setForDelete();
		}
	}

}

//Set exploded when bullet destroys house
void BreakableComponent::setExploded()
{
	_exploded = true;
	_timer = 1.f;
	//set sprite to explotion sprite
	auto spComp = _parent->GetCompatibleComponent<SpriteComponent>();
	if (!spComp.empty()){
		spComp[0]->setTexture(Resources::load<Texture>("smokeGrey4.png"));
		spComp[0]->getSprite().setScale(.70f, .70f);
	auto bounds = spComp[0]->getSprite().getLocalBounds();
	spComp[0]->getSprite().setOrigin(bounds.getSize().x/2, bounds.getSize().x/2);
	}
	else {
		auto anim = _parent->GetCompatibleComponent<AnimationComponent>();
		anim[0]->setSpritesheet(*Resources::load<Texture>("smokeGrey4.png"));
		anim[0]->setFrameCount(1);
		anim[0]->setScale(Vector2f(0.7, 0.7));
		auto bounds = anim[0]->getSprite().getLocalBounds();
		anim[0]->setSize(bounds.getSize());
	}
}

bool BreakableComponent::isExploded()
{
	return _exploded;
}
