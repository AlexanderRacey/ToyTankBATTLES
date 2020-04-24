#include <SFML/Audio.hpp>
#include <system_resources.h>
#include <levelsystem.h>
#include "cmp_actor_movement.h"
#include "cmp_bullet.h"
#include "cmp_physics.h"
#include "../animation.h"
#include "../add_entity.h"
#include "cmp_breakable.h"
#include "cmp_sound.h"
#include "../game.h"

using namespace sf;
using namespace std;

SoundPlayer playerFireSound;
SoundPlayer enemyFireSound;

// -- Actor movement --
void ActorMovementComponent::update(double dt) {};

bool ActorMovementComponent::validMove(const Vector2f& pos)
{
    return (!ls::isWall(ls::getTileAt(pos)));
};

ActorMovementComponent::ActorMovementComponent(Entity* p) : _speed(50.0f), Component(p) {};

void ActorMovementComponent::move(const Vector2f& pos)
{
    if (validMove(pos))
    {
        _parent->setPosition(pos);
    }
};

void ActorMovementComponent::move(float x, float y) { move(Vector2f(x, y)); }

void ActorMovementComponent::increaseSpeed(float sp)
{
    _speed += sp;
}

float ActorMovementComponent::getSpeed() const { return _speed; }

void ActorMovementComponent::setSpeed(float speed) { _speed = speed; }



// -- Player movement --
PlayerMovementComponent::PlayerMovementComponent(Entity* p) : ActorMovementComponent(p), _offset({0,0})
{
    setSpeed(100.f);
}

void PlayerMovementComponent::setRotation(float rot)
{
    auto animation = _parent->GetCompatibleComponent<AnimationComponent>();

    if (!animation.empty())
    {
        animation[0]->setRotation(rot);
    }
}

void PlayerMovementComponent::update(double dt)
{
    int xdir = 0, ydir = 0;

    if (Keyboard::isKeyPressed(Keyboard::W))
    {
        _offset = Vector2f(0, (getBounds().getSize().y - 10));
        setRotation(0.0f);
        move(Vector2f(0, -_speed * dt));
        direction = {0,-1};
    }
    if (Keyboard::isKeyPressed(Keyboard::S))
    {
        _offset = Vector2f(0, -(getBounds().getSize().y -10));
        setRotation(180.0f);
        move(Vector2f(0, _speed * dt));
        direction = { 0, 1 };

    }
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        _offset = Vector2f((getBounds().getSize().x - 10), 0);
        setRotation(270.0f);
        move(Vector2f(-_speed * dt, 0));
        direction = { -1, 0 };
    }
    if (Keyboard::isKeyPressed(Keyboard::D)){

        _offset = Vector2f(-(getBounds().getSize().x -10), 0);
        setRotation(90.0f);
        move(Vector2f(_speed * dt, 0));
        direction = { 1, 0 };
    }

    if (Keyboard::isKeyPressed(Keyboard::Space))
    {
        if (firetimer > 0)
        {
            firetimer -= dt;
        }
        else
        {
            playerFireSound.playerFire(0, false);
            fire();
        }
    }

    ActorMovementComponent::update(dt);
}

void PlayerMovementComponent::render()
{
    ActorMovementComponent::render();
}

void PlayerMovementComponent::addDamage(float dam)
{
    damage += dam;
}

void PlayerMovementComponent::move(const Vector2f& p)
{
    auto pp = _parent->getPosition() + p;
    if (!isBlocked(pp))
    {
        _parent->setPosition(pp);
    }
}

void PlayerMovementComponent::fire()
{
    auto bullet = _parent->scene->makeEntity();
    Vector2f pos = _parent->getPosition() - _offset;

    auto spriteB = bullet->addComponent<SpriteComponent>();

    spriteB->setTexture(Resources::load<Texture>("playerBullet.png"));
    bullet->setPosition(pos);
    auto bulletcomp = bullet->addComponent<PlayerBullet>();
    bulletcomp->setDamage(damage);
    bulletcomp->setDirection(direction);



    bullet->setRotation(getRotation());
    firetimer = 0.2f;
}

float PlayerMovementComponent::getRotation()
{
    auto animation = _parent->GetCompatibleComponent<AnimationComponent>();

    if (!animation.empty())
    {
        return animation[0]->getRotation();
    }
    else
    {
        return 0.f;
    }
}

bool PlayerMovementComponent::isBlocked(Vector2f pos)
{
    if (ls::isWall(ls::getTileAt(pos, _offset)))
    {
        if(ls::getTileAt(pos, _offset) == ls::BROKEN || ls::getTileAt(pos, _offset) == ls::BROKEN_R)
        {
            vector<shared_ptr<Entity>> potTargets = Engine::findEntity("brokenHouse");
            for (auto t : potTargets)
            {
                auto sp = t->GetCompatibleComponent<SpriteComponent>();
                auto bounds = sp[0]->getSprite().getGlobalBounds();
                //cheating the left detection by making width much wider for now
                bounds = FloatRect(bounds.left - 50.f, bounds.top - 40, bounds.width + 120.f, bounds.height + 70.f);

                if (bounds.contains(pos))
                {
                    if (t->isAlive())
                    {
                        return true;
                    }
                }
            }
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}


// -- Enemy Component --
//direction order : right, down, up, left
static const Vector2i directions[] = { {1,0}, {0,1}, {0, -1}, {-1, 0} };
const float rotations[] = { 90.f, 180.f, 0.f, 270.f};

EnemyAiComponent::EnemyAiComponent(Entity* p) : ActorMovementComponent(p), _state(MOVING), _offset(Vector2f(0, 0)), _direction(Vector2f(0, 0)), gap(30.f)
{
    ChangeDirection(false);
    setSpeed(50.f);
    setRotation(rotations[index]);
};

void EnemyAiComponent::move(const Vector2f& pos)
{
    _parent->setPosition(pos);
}

void EnemyAiComponent::update(double dt)
{
    //amount to move
    const auto mva = (float)(dt * _speed);
    //Current position
    const Vector2f pos = _parent->getPosition();
    //Next position
    Vector2f newpos = pos + _direction * mva;
    Vector2f testPos = newpos + Vector2f((_offset.x/2)*-1, (_offset.y/2)*-1);
   // auto breakable;

    switch (_state)
    {
        case EnemyAiComponent::MOVING:
            if (validMove(newpos, _offset))
            {
                shared_ptr<Entity> player = Engine::findEntity("player")[0];
                if (ls::BROKEN == ls::getTileAt(testPos, _offset) || ls::BROKEN_R == ls::getTileAt(testPos, _offset))

                {
                    vector<shared_ptr<Entity>> potTargets = Engine::findEntity("brokenHouse");
                    for (auto t : potTargets)
                    {
                        auto sp = t->GetCompatibleComponent<SpriteComponent>();
                        auto bounds = sp[0]->getSprite().getGlobalBounds();
                        //cheating the left detection by making width much wider for now
                        bounds = FloatRect(bounds.left - 50.f, bounds.top - 50.f, bounds.width + 100, bounds.height + 50.f);

                        if (bounds.contains(testPos))
                        {
                            if (t->isAlive())
                            {
                                target = t;
                                fireTimer = 0.5f;
                                _state = AIMING;
                                blocked = true;
                                break;
                            }
                        }
                    }
                    if (!blocked)
                    {
                        move(newpos);
                    }

                }
              

                else if (PlayerInRange())
                {
                    target = player;
                    fireTimer = 0.5f;
                    _state = SHOTING;
                    break;
                }
                else
                {
                    move(newpos);
                }

            }
            else
            {
                ChangeDirection(false);
                _state = ROTATING;
            }
            break;
        case EnemyAiComponent::SHOTING:
            if (target->isAlive())
            {   
                shared_ptr<Entity> player = Engine::findEntity("player")[0];
                if (target == player && !PlayerInRange()) {
                     facePlayer();
                }else{
                auto breakable = target->GetCompatibleComponent<BreakableComponent>();
                if (!breakable.empty())
                {
                    if (!breakable[0]->isExploded())
                    {
                        fireTimer -= dt;
                        if (fireTimer <= 0)
                        {
                            aimTurrent();
                            enemyFireSound.enemyFire(1, false);
                            fire();
                            fireTimer = 3.f;
                        }
                    }
                }
            }
            }
            else
            {
                _state = MOVING;
                blocked = false;
            }
            break;
        case EnemyAiComponent::ROTATING:
            //left
            if (_direction == Vector2f(-1, 0) && getRotation() == 270.f)
            {
                setRotation(270.f);
                setSpeed(50);
                _state = MOVING;
            }
            //rigth
            else if (_direction == Vector2f(1, 0) && getRotation() == 90.f)
            {
                setRotation(90.f);
                setSpeed(50);
                _state = MOVING;
            }
            //up
            else if (_direction == Vector2f(0, -1) && (getRotation() == 360.f || getRotation() == 0.f))
            {
                setRotation(0.f);
                setSpeed(50);
                _state = MOVING;
            }
            //down
            else if (_direction == Vector2f(0, 1) && getRotation() == 180.f)
            {
                setRotation(180.f);
                setSpeed(50);
                _state = MOVING;
            }
            else
            {
                if (turnRight)
                {
                    rotate(0.5f);
                }
                else
                {
                    rotate(-0.5f);
                }
            }
            break;
        case EnemyAiComponent::AIMING:
            aimTurrent();
            _state = SHOTING;
            break;
        default:
            break;
    }
}

void EnemyAiComponent::ChangeDirection(bool setIndex)
{
    Vector2f newDir;
    Vector2f newPos;

    if (!setIndex){
        index = 0;
        do
        {
            index = rand() % 4;
            newDir = Vector2f(directions[index]);
        } while (newDir == _direction);

    }
    else {
        newDir = Vector2f(directions[index]);
    }

    switch (index)
    {
    case 0:
        //move right
        if (_direction == Vector2f(0, 1))
        {
            turnRight = false;
        }
        else
        {
            turnRight = true;
        }
        _offset = Vector2f(-getBounds().height, 0);
        break;
    case 1:
        //Move down
        //setRotation(0.f);
        if (_direction == Vector2f(-1, 0))
        {
            turnRight = false;
        }
        else
        {
            turnRight = true;
        }
        _offset = Vector2f(0, -getBounds().height);
        break;
    case 2:
        //Move Up
        //setRotation(0.f);
        if (_direction == Vector2f(1, 0))
        {
            turnRight = false;
        }
        else
        {
            turnRight = true;
        }
        _offset = Vector2f(0, getBounds().height);
        break;
    case 3:
        //Move left
        //setRotation(90.f);
        if (_direction == Vector2f(0, -1))
        {
            turnRight = false;
        }
        else
        {
            turnRight = true;
        }
        _offset = Vector2f(getBounds().height, 0);
        break;
    }

    _direction = newDir;
}

void EnemyAiComponent::facePlayer()
{
    shared_ptr<Entity> player = Engine::findEntity("player")[0];
    bool turnedSuccess = false;
    int posDirections[] = { 0, 1, 2, 3 };
    int previndex = index;
        for (int i : posDirections) {
            if (i != previndex) {
                index = i;
                ChangeDirection(true);
                if (PlayerInRange()) {
                    turnedSuccess = true;
                    break;
                }
            }

        }

        if (!turnedSuccess) {
            index = previndex;
            ChangeDirection(true);
            _state = MOVING;
            blocked = false;
        }
        else {
            _state = ROTATING;
            blocked = false;
        }
}


void EnemyAiComponent::resetState()
{
    _state = MOVING;
}

void EnemyAiComponent::setRotation(float rot)
{
    auto animation = _parent->GetCompatibleComponent<EnemyAnimationComp>();

    if (!animation.empty())
    {
        animation[0]->setRotation(rot);
    }
}

void EnemyAiComponent::rotate(float rot)
{
    auto animation = _parent->GetCompatibleComponent<EnemyAnimationComp>();

    if (!animation.empty())
    {
        animation[0]->rotate(rot);
    }
}

float EnemyAiComponent::getRotation()
{
    auto animation = _parent->GetCompatibleComponent<AnimationComponent>();

    if (!animation.empty())
    {
       return animation[0]->getRotation();
    }
    else
    {
        return 0.f;
    }
}

FloatRect ActorMovementComponent::getBounds()
{
    auto animation = _parent->GetCompatibleComponent<AnimationComponent>();

    if (!animation.empty())
    {
        return animation[0]->getSprite().getGlobalBounds();
    }
    else
    {
        return FloatRect(0, 0, 0, 0);
    }
}

void EnemyAiComponent::aimTurrent()
{
    //Aim turrent depending on tank direction it is facing
    //direction order : right, down, up, left
    float x1 = 0.f;
    float x2 = 0.f;
    float y1 = 0.f;
    float y2 = 0.f;

    tAngle = 0.f;
    Vector2f tilePos = target->getPosition();
   // turrentRight = turnTurrentRight(tilePos);
    x1 = _parent->getPosition().x;
    x2 = tilePos.x;
    y1 = _parent->getPosition().y;
    y2 = tilePos.y;

    turrentRight = turnTurrentRight(tilePos);
    switch (index) {
    case 0:
        //facing right
        tAngle = atan((y2 - y1)/(x2 - x1));
        break;
    case 1:
        //facing downwards
        tAngle = atan((x2 - x1) / (y2 - y1));
        break;
    case 2:
        //facing upwards
        tAngle = atan((x2 - x1) / (y2 - y1));
        break;
    case 3:
        //facing left
        tAngle = atan((y1 - y2) / (x1 - x2));
        break;
    }

    // angle times 10 is perfect for bullet trajectory but turrent is wrong angle
     tAngle = tAngle * 100;
    if (!turrentRight && tAngle > 0) {
        tAngle = -(tAngle);
    }
    else if(turrentRight && tAngle < 0) {
        tAngle = (tAngle) * -1;
    }

    setTurrentRotation(tAngle);

}

void EnemyAiComponent::setTurrentRotation(float rot)
{
    auto animation = _parent->GetCompatibleComponent<EnemyAnimationComp>();

    if (!animation.empty())
    {
        animation[0]->setTurretRotation(getRotation() + rot);
    }
}

float EnemyAiComponent::getTurrentRotation()
{
    auto animation = _parent->GetCompatibleComponent<EnemyAnimationComp>();

    if (!animation.empty())
    {
        return animation[0]->getTurretRotation();
    }
}

void EnemyAiComponent::fire()
{
    auto bullet = _parent->scene->makeEntity();
    bullet->setPosition(_parent->getPosition());
    auto bulletcomp = bullet->addComponent<EnemyBullet>();
    bulletcomp->setTarget(target);
    bulletcomp->setDamage(20.f);
    float angle = tAngle / 100;


    auto spriteB = bullet->addComponent<SpriteComponent>();

    spriteB->setTexture(Resources::load<Texture>("enemyBullet.png"));
    auto bounds = spriteB->getSprite().getGlobalBounds();
    spriteB->getSprite().setOrigin(Vector2f(bounds.getSize().x / 2, bounds.getSize().y));

    bullet->setRotation(getTurrentRotation());
   
    switch (index)
    {
    case 0:
        //facing right
            bulletcomp->setDirection(_direction + Vector2f(0, angle));
        break;
    case 1:
        //facing downwards
            bulletcomp->setDirection(_direction + Vector2f(-angle, 0));;
        break;
    case 2:
        //facing upwards
            bulletcomp->setDirection(_direction + Vector2f(angle, 0));
        break;
    case 3:
        //facing left
            bulletcomp->setDirection(_direction + Vector2f(0, -angle));
        break;
    }

}


bool EnemyAiComponent::turnTurrentRight(Vector2f targetpos) {
   
    Vector2f pos = _parent->getPosition();
    
    switch (index)
    {
    case 0:
        //facing right
        if (targetpos.y > pos.y) {
            return true;
        }
        else {
            return false;
        }
        break;
    case 1:
        //facing downwards
        if (targetpos.x > pos.x) {
            return false;
        }
        else {
            return true;
        }
        break;
    case 2:
        //facing upwards
        if (targetpos.x > pos.x) {
            return true;
        }
        else {
            return false;
        }
        break;
    case 3:
        //facing left
        if (targetpos.y < pos.y) {
            return true;
        }
        else {
            return false;
        }
        break;
    }
}

bool EnemyAiComponent::PlayerInRange()
{
    shared_ptr<Entity> player = Engine::findEntity("player")[0];
    FloatRect pBounds = player->GetCompatibleComponent<AnimationComponent>()[0]->getSprite().getGlobalBounds();
    FloatRect eBounds = _parent->GetCompatibleComponent<AnimationComponent>()[0]->getSprite().getGlobalBounds();
    switch (index)
    {
    case 0:
        //move right
        eBounds = FloatRect(eBounds.left, eBounds.top - 50.f, eBounds.width + 150.f, eBounds.height + 50.f);
        break;
    case 1:
        //Move down
        eBounds = FloatRect(eBounds.left - 50.f, eBounds.top, eBounds.width + 50.f, eBounds.height + 150.f);
        break;
    case 2:
        //Move Up
        eBounds = FloatRect(eBounds.left - 50.f, eBounds.top - 150.f, eBounds.width + 50.f, eBounds.height);
        break;
    case 3:
        //Move left
        eBounds = FloatRect(eBounds.left - 150.f, eBounds.top - 50.f, eBounds.width, eBounds.height + 50.f);
        break;
    }

    if (eBounds.intersects(pBounds)){
        return true;
    }
    else {
        return false;
    }
}


bool EnemyAiComponent::validMove(Vector2f pos, Vector2f offset) {
    return !(ls::isSolidWall(ls::getTileAt(pos, _offset)));
}

void EnemyAiComponent::notifyEnemy() {
    _state = SHOTING;

    facePlayer();
}