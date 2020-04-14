#include "cmp_bullet.h"

using namespace std;
using namespace sf;

void BulletComponent::update(double dt)
{
    _lifetime -= dt;
    if (_lifetime <= 0.f) 
    {
        _parent->setForDelete();
    }
}

BulletComponent::BulletComponent(Entity* p, float lifetime, float size, float impulse, float speed, float damage) : Component(p), _lifetime(lifetime), _size(size), _impulse(impulse), _speed(speed), _damage(damage) {}