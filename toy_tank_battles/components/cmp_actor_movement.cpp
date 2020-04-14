#include <system_resources.h>
#include <levelsystem.h>
#include "cmp_actor_movement.h"
#include "cmp_bullet.h"
#include "cmp_physics.h"
#include "../animation.h"
#include "../add_entity.h"

using namespace sf;

void ActorMovementComponent::update(double dt) {};

bool ActorMovementComponent::validMove(const sf::Vector2f& pos)
{
    return (!ls::isWall(ls::getTileAt(pos)));
};

ActorMovementComponent::ActorMovementComponent(Entity* p)
    : _speed(50.0f), Component(p) {};


void ActorMovementComponent::move(const sf::Vector2f& pos) {
    if (validMove(pos)) {
        _parent->setPosition(pos);
    }
};

void ActorMovementComponent::move(float x, float y) {
    move(Vector2f(x, y));
}

float ActorMovementComponent::getSpeed() const { return _speed; }
void ActorMovementComponent::setSpeed(float speed) { _speed = speed; }


// -- Player movement
PlayerMovementComponent::PlayerMovementComponent(Entity* p) : ActorMovementComponent(p) {
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

void PlayerMovementComponent::fire(float rotation) 
{
    auto bullet = _parent->scene->makeEntity();
    bullet->setPosition(_parent->getPosition());
    bullet->addComponent<BulletComponent>();

    auto animation = bullet->addComponent<AnimationComponent>(Vector2f(10.f, 17.f));
    Texture bulletTexture = *Resources::load<Texture>("playerBullet.png");
    animation->setSpritesheet(bulletTexture);
    animation->setFrameCount(1);
    animation->setFrameTime(0.06f);

    float inverse = fmod((rotation + 180.f), 360);  //Sets rotation of bullet to be inverse of ship rotation, using fancy maths.
    bullet->setRotation(inverse);

    /*auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(8.f, 8.f));
    p->setRestitution(.4f);
    p->setFriction(.005f);
    p->impulse(rotate(Vector2f(0, 15.f), -_parent->getRotation()));*/
}

void PlayerMovementComponent::update(double dt)
{
    int xdir = 0, ydir = 0;

    if (Keyboard::isKeyPressed(Keyboard::W))
    {
        setRotation(0.0f);
        move(Vector2f(0, -_speed * dt));
    }
    if (Keyboard::isKeyPressed(Keyboard::S))
    {
        setRotation(180.0f);
        move(Vector2f(0, _speed * dt));
    }
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        setRotation(270.0f);
        move(Vector2f(-_speed * dt, 0));
    }
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        setRotation(90.0f);
        move(Vector2f(_speed * dt, 0));
    }
    if (Keyboard::isKeyPressed(Keyboard::Space))
    {
        float rotation = _parent->getRotation();
        fire(rotation);
    }

    ActorMovementComponent::update(dt);
}

void PlayerMovementComponent::render()
{
    ActorMovementComponent::render();
}

void PlayerMovementComponent::move(const Vector2f& p)
{
    auto pp = _parent->getPosition() + p;
    if (validMove(pp))
    {
        _parent->setPosition(pp);
    }
}


// -- Enemy Component --
static const Vector2i directions[] = { {1,0}, {0,1}, {0, -1}, {-1, 0} };

EnemyAiComponent::EnemyAiComponent(Entity* p) : ActorMovementComponent(p), _state(MOVING), _offset(Vector2f(0, 0)), _direction(Vector2f(0, 0)), gap(20.f) 
{
    ChangeDirection();
    setSpeed(50.f);
};

void EnemyAiComponent::move(const sf::Vector2f& pos) 
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
    Vector2f testPos = newpos + _offset;

    switch (_state)
    {
        case EnemyAiComponent::MOVING:
            if (validMove(testPos)) 
            {
                move(newpos);
            }
            else
            {
                ChangeDirection();
                //setSpeed(150);
                _state = ROTATING;
            }
            break;
        case EnemyAiComponent::SHOTING:
            break;
        case EnemyAiComponent::ROTATING:
            //left
            if (_direction == Vector2f(-1, 0) && getRotation() == 270.f)
            {
                setSpeed(50);
                _state = MOVING;
            }
            //rigth
            else if (_direction == Vector2f(1, 0) && getRotation() == 90.f)
            {
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
        case EnemyAiComponent::ROTATED:
            break;
        default:
            break;
    }
    // move(newpos);
}

void EnemyAiComponent::ChangeDirection()
{
    Vector2f newDir;
    Vector2f newPos;
    int loc = 0;
    do
    {
        loc = rand() % 4;
        newDir = Vector2f(directions[loc]);
    } while (newDir == _direction);

    switch (loc)
    {
        case 0:
            //move right
            //setRotation(90.f);
            if (_direction == Vector2f(0, 1)) 
            {
                turnRight = false;
            }
            else 
            {
                turnRight = true;
            }
            _offset = Vector2f(getBounds().getSize().x + gap, 0);
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
            _offset = Vector2f(0, getBounds().getSize().y + gap);
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
            _offset = Vector2f(0, 0);
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
            _offset = Vector2f(0, 0);
            break;
    }

    _direction = newDir;
}

void EnemyAiComponent::resetState()
{
    _state = MOVING;
}

void EnemyAiComponent::setRotation(float rot)
{
    auto animation = _parent->GetCompatibleComponent<AnimationComponent>();

    if (!animation.empty())
    {
        animation[0]->setRotation(rot);
    }
}

void EnemyAiComponent::rotate(float rot)
{
    auto animation = _parent->GetCompatibleComponent<AnimationComponent>();

    if (!animation.empty()) 
    {
        animation[0]->rotate(rot);
    }
}

float EnemyAiComponent::getRotation() 
{
    auto animation = _parent->GetCompatibleComponent<AnimationComponent>();

    if (!animation.empty()) {
       return animation[0]->getRotation();
    }
    else {
        return 0.f;
    }
}

FloatRect EnemyAiComponent::getBounds() 
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
