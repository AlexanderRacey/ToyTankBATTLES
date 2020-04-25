#include "cmp_bullet.h"
#include "cmp_breakable.h"
#include <engine.h>
#include "../animation.h"
#include "cmp_health.h"
#include <levelsystem.h>
#include "../game.h"
#include "cmp_actor_movement.h"
#include "cmp_sound.h"

using namespace std;
using namespace sf;

// Sound
SoundPlayer popSound;
SoundPlayer playerPopSound;

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

void BulletComponent::setDamage(float dam)
{
    _damage = dam;
}

void BulletComponent::addDamage(float dam)
{
    _damage += dam;
}

//enemy bullet:-------------

EnemyBullet::EnemyBullet(Entity* p, float lifetime, float speed, float damage, Vector2f offset)
    : BulletComponent(p, lifetime, speed, damage), _offset(offset) {};


//checks if enemy bullets colides with its target
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

//sets direction and offset
void EnemyBullet::setDirection(Vector2f dir) {
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

//updates bullet and deletes it eventually
void EnemyBullet::update(double dt)
{
    Vector2f pos = _parent->getPosition();
    
    _lifetime -= dt;
    if (_lifetime <= 0.f)
    {
        _parent->setForDelete();
    }
    else if (ls::isSolidWall(ls::getTileAt(pos, _offset))) {
        _parent->setForDelete();
    }
    else
    {
        //if bullet has target checks collision, if collides with target interact with target
        if (targetset)
        {
            if (checkCollision())
            {
                if (_target == Engine::findEntity("player")[0])
                {
                    //deduct healthpoints if hit player, if health points depleted, explode player
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
                        popSound.pop(4, false);
                        health[0]->deductHealth(_damage);
                        _parent->setForDelete();
                    }
                }
                else
                {
                    //explode broken house if collision is true
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

//player bullet:-----------

PlayerBullet::PlayerBullet(Entity* p, float lifetime, float speed, float damage, Vector2f offset) : BulletComponent(p, lifetime, speed, damage), _offset(offset) {}

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

//checks collision of player bullet and acts accordingly
bool PlayerBullet::checkCollision()
{
    Vector2f pos = _parent->getPosition();

    //if tile at position is a broken house and it is not exploded yet, explode house
    if (ls::getTileAt(pos, _offset) == ls::BROKEN || ls::getTileAt(pos, _offset) == ls::BROKEN_R)
    {
        vector<shared_ptr<Entity>> potTargets = Engine::findEntity("brokenHouse");
        for (auto t : potTargets)
        {
            auto sp = t->GetCompatibleComponent<SpriteComponent>();
            auto bounds = sp[0]->getSprite().getGlobalBounds();

            FloatRect bBounds = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getBounds();

            if (bBounds.intersects(bounds))
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
    }//if bullet collides with a solid wall just return true to delete the bullet
    else if (ls::isSolidWall(ls::getTileAt(pos, _offset))) {
        return true;
    }
    else
    {
        //else check if bullet collides with an enemy and then deduct health from that enemy
        vector<shared_ptr<Entity>> potTargets = Engine::findEntity("enemy");
        for (auto t : potTargets)
        {
            auto sp = t->GetCompatibleComponent<AnimationComponent>();
            auto movement = t->GetCompatibleComponent<EnemyAiComponent>();
            if (!sp.empty() && !movement.empty())
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
                                playerPopSound.playerPop(5, false);
                                healtcomp[0]->deductHealth(_damage);
                                movement[0]->notifyEnemy();
                            }
                            else 
                            {
                                //if health depleted add to score and explode enemy
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

//sets direction and offset
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
