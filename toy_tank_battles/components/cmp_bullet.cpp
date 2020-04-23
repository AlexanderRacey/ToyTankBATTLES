#include "cmp_bullet.h"
#include "cmp_breakable.h"
#include <engine.h>
#include "../animation.h"
#include "cmp_health.h"
#include <levelsystem.h>
#include "../game.h"

using namespace std;
using namespace sf;

void BulletComponent::update(double dt) {};


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

EnemyBullet::EnemyBullet(Entity* p, float lifetime, float speed, float damage, Vector2f offset)
    : BulletComponent(p, lifetime, speed, damage), _offset(offset) {};

bool EnemyBullet::checkCollision() 
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

void EnemyBullet::setDirection(Vector2f dir) {
    direction = dir;
}

void EnemyBullet::update(double dt)
{
    Vector2f pos = _parent->getPosition();

    _lifetime -= dt;
    if (_lifetime <= 0.f)
    {
        _parent->setForDelete();
    }
    else if (ls::isSolidWall(ls::getTileAt(pos))) {
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

PlayerBullet::PlayerBullet(Entity* p, float lifetime, float speed, float damage, Vector2f offset) : BulletComponent(p, lifetime, speed, damage), _offset(offset) {}

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
    else if (ls::isSolidWall(ls::getTileAt(pos, _offset))) {
        return true;
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

void PlayerBullet::setDirection(Vector2f dir)
{
    direction = dir;
    FloatRect bulletBounds = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getBounds();

    ///right
    if (dir == Vector2f(1, 0)) {
        _offset = Vector2f(bulletBounds.getSize().x, 0);
    }
    //left
    else if (dir == Vector2f(-1, 0)) {
        _offset = Vector2f(-bulletBounds.getSize().x, 0);
    }
    //up
    else if (dir == Vector2f(0, -1)) {
        _offset = Vector2f(0, -bulletBounds.getSize().y);
    }
    //down
    else if (dir == Vector2f(0, 1)) {
        _offset = Vector2f(0, bulletBounds.getSize().y);
    }
}
