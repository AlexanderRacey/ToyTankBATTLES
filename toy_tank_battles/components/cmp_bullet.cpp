#include "cmp_bullet.h"
#include "cmp_breakable.h"
#include <engine.h>
#include "../animation.h"
#include "cmp_health.h"
#include <levelsystem.h>
#include "../game.h"

using namespace std;
using namespace sf;

void BulletComponent::update(double dt)
{
    _lifetime -= dt;
    if (_lifetime <= 0.f)
    {
        _parent->setForDelete();
    }
    else
    {
        if (targetset) 
        {
            if (checkCollision()) 
            {
                if (_target == Engine::findEntity("player")[0]) 
                {                    
                    auto health = _target->GetCompatibleComponent<HealthComponent>();

                    if (health[0]->getHealth() < 0) 
                    {
                        auto breakable = _target->GetCompatibleComponent<BreakableComponent>();
                        if (!breakable.empty()) 
                        {
                            breakable[0]->setExploded();
                            _parent->setForDelete();
                        }
                    }
                    else
                    {
                       // playerHealth -= 20;
                        health[0]->deductHealth(_damage);
                        _parent->setForDelete();
                    }
                }
                else
                {
                    auto breakable = _target->GetCompatibleComponent<BreakableComponent>();
                    if (!breakable.empty()) 
                    {
                        breakable[0]->setExploded();
                        _parent->setForDelete();
                    }
                }
            }
        }
        move(dt);
    }
}

BulletComponent::BulletComponent(Entity* p, float lifetime, float speed, float damage)
    : Component(p), _lifetime(lifetime),  _speed(speed), _damage(damage) {}

void BulletComponent::setTarget(shared_ptr<Entity> tar)
{
    _target = tar;
    targetset = true;
}

void BulletComponent::setDirection(Vector2f dir)
{
    direction = dir;
}

void BulletComponent::move(double dt)
{
    _speed = 100.f;
    Vector2f newPos = _parent->getPosition() + (direction * (float) (dt * _speed));
    _parent->setPosition(newPos);
}

bool BulletComponent::checkCollision() 
{
    if (_target->isAlive()) 
    {
        FloatRect bulletBounds = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getBounds();

        FloatRect targetBounds;
        if (_target == Engine::findEntity("player")[0])
        {
            targetBounds = _target->GetCompatibleComponent<AnimationComponent>()[0]->getSprite().getGlobalBounds();
        }
        else 
        {
            targetBounds = _target->GetCompatibleComponent<SpriteComponent>()[0]->getBounds();
        }

        if (bulletBounds.intersects(targetBounds))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

PlayerBullet::PlayerBullet(Entity* p, float lifetime, float speed, float damage) : BulletComponent(p, lifetime, speed, damage) {}

void BulletComponent::setDamage(float dam)
{
    _damage = dam;
}

void BulletComponent::addDamage(float dam)
{
    _damage += dam;
}

void PlayerBullet::update(double dt)
{
    _lifetime -= dt;
    if (_lifetime <= 0.f)
    {
        _parent->setForDelete();
    }
    else if (checkCollision()) 
    {
        _parent->setForDelete();
    }
    else
    {
        move(dt);
    }
}



bool PlayerBullet::checkCollision()
{
    Vector2f pos = _parent->getPosition();

    if (ls::getTileAt(pos) == ls::BROKEN || ls::getTileAt(pos) == ls::BROKEN_R) 
    {
        vector<shared_ptr<Entity>> potTargets = Engine::findEntity("brokenHouse");
        for (auto t : potTargets)
        {
            auto sp = t->GetCompatibleComponent<SpriteComponent>();
            auto bounds = sp[0]->getSprite().getGlobalBounds();

            FloatRect bBounds = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getBounds();
            if (bounds.intersects(bBounds))
            {
                if (t->isAlive())
                {
                    playerScore += 25;
                    t->GetCompatibleComponent<BreakableComponent>()[0]->setExploded();
                    return true;
                }
            }
        }
        return false;
    }
    else
    {
        vector<shared_ptr<Entity>> potTargets = Engine::findEntity("enemy");
        for (auto t : potTargets)
        {
            auto sp = t->GetCompatibleComponent<AnimationComponent>();
            if (!sp.empty())
            {
                auto bounds = sp[0]->getSprite().getGlobalBounds();

                FloatRect bBounds = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getBounds();
                if (bounds.intersects(bBounds))
                {
                    if (t->isAlive())
                    {
                        auto healtcomp = t->GetCompatibleComponent<HealthComponent>();
                        if(!healtcomp.empty())
                        {
                            if (healtcomp[0]->getHealth() > 0)
                            {
                                healtcomp[0]->deductHealth(_damage);
                            }
                            else 
                            {
                                playerScore += 100;
                                t->GetCompatibleComponent<BreakableComponent>()[0]->setExploded();
                            }
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
}
