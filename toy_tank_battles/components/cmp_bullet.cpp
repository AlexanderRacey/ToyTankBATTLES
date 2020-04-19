#include "cmp_bullet.h"
#include "cmp_breakable.h"
#include <engine.h>
#include "../animation.h"
#include "cmp_health.h"

using namespace std;
using namespace sf;

void BulletComponent::update(double dt)
{
    _lifetime -= dt;
    if (_lifetime <= 0.f) 
    {
        _parent->setForDelete();
    }
    else{
        if (targetset) {

            if (checkCollision()) {
                if (_target == Engine::findEntity("player")[0]) {
                    auto health = _target->GetCompatibleComponent<HealthComponent>();

                  if (health[0]->getHealth() < 0) {
                        auto breakable = _target->GetCompatibleComponent<BreakableComponent>();
                        if (!breakable.empty()) {
                            breakable[0]->setExploded();
                            _parent->setForDelete();
                        }
                    }
                    else {
                        health[0]->deductHealth(_damage);
                        _parent->setForDelete();
                    }
                }else{
                auto breakable = _target->GetCompatibleComponent<BreakableComponent>();
                if (!breakable.empty()) {
                    breakable[0]->setExploded();
                    _parent->setForDelete();
                }
                }
            }
        }
        move(dt);
    }
}

BulletComponent::BulletComponent(Entity* p, float lifetime, float size, float impulse, float speed, float damage) : Component(p), _lifetime(lifetime), _size(size), _impulse(impulse), _speed(speed), _damage(damage) {

}

void BulletComponent::setTarget(shared_ptr<Entity> tar) {
    _target = tar;
    targetset = true;
}

void BulletComponent::setDirection(Vector2f dir) {
    direction = dir;
}

void BulletComponent::move(double dt) {
    _speed = 50.f;
    Vector2f newPos = _parent->getPosition() + (direction * (float) (dt * _speed));
    _parent->setPosition(newPos);
}

bool BulletComponent::checkCollision() {
    if (_target->isAlive()) {
        FloatRect bulletBounds = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getBounds();
        //would not work for animation,need to implement new check. 
        FloatRect targetBounds;
        if (_target == Engine::findEntity("player")[0]){
            targetBounds = _target->GetCompatibleComponent<AnimationComponent>()[0]->getSprite().getGlobalBounds();
        }
        else {
            targetBounds = _target->GetCompatibleComponent<SpriteComponent>()[0]->getBounds();
        }
      

        if (bulletBounds.intersects(targetBounds)) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}