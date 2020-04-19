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

	if (_exploded)
	{

		_parent->setForDelete();

	}

}

//Set exploded when bullet destroys house
void BreakableComponent::setExploded()
{
	_exploded = true;
	auto smoke = AddEntity::makeSmoke(_parent->scene, _parent->getPosition());
}

bool BreakableComponent::isExploded()
{
	return _exploded;
}
