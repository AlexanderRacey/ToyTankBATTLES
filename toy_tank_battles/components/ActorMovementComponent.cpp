#include "ActorMovementComponent.h"
#include "levelsystem.h"
#include "cmp_sprite.h"
#include "../animation.h"

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


static const Vector2i directions[] = { {1,0}, {0,1}, {0, -1}, {-1, 0} };

EnemyAiComponent::EnemyAiComponent(Entity* p)
    : ActorMovementComponent(p), _state(MOVING), _offset(Vector2f(0,0)), _direction(Vector2f(0,0)) , gap(20.f) {
    ChangeDirection();
};

void EnemyAiComponent::move(const sf::Vector2f& pos) {

    _parent->setPosition(pos);

}


void EnemyAiComponent::update(double dt) {
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
        if (validMove(testPos)) {
            move(newpos);
        }
        else {
           
            _state = ROTATING;
        }
        break;
    case EnemyAiComponent::SHOTING:
        break;
    case EnemyAiComponent::ROTATING:
        ChangeDirection();
        _state = MOVING;
        break;
    case EnemyAiComponent::ROTATED:
        break;
    default:
        break;
    }

   // move(newpos);

}

void EnemyAiComponent::ChangeDirection() {
    Vector2f newDir;
    Vector2f newPos;
    int loc = 0;
    do {
        loc = rand() % 4;
        newDir = Vector2f(directions[loc]);
    } while (newDir == _direction);

    switch (loc)
    {
    case 0:
        //move right
        setRotation(90.f);
        _offset = Vector2f(getBounds().getSize().x + gap, 0);
        break;
    case 1:
        //Move down
        setRotation(0.f);
        _offset = Vector2f(0, getBounds().getSize().y + gap);
        break;
    case 2:
        //Move Up
        setRotation(0.f);
        _offset = Vector2f(0, 0);
        break;
    case 3:
        //Move left
        setRotation(90.f);
        _offset = Vector2f(0, 0);
        break;
    }

    _direction = newDir;
}

void EnemyAiComponent::resetState() {
    _state = MOVING;
}

void EnemyAiComponent::setRotation(float rot) {
    auto animation = _parent->GetCompatibleComponent<AnimationComponent>();

    if (!animation.empty()) {
        animation[0]->setRotation(rot);
    }
}

FloatRect EnemyAiComponent::getBounds() {

    auto animation = _parent->GetCompatibleComponent<AnimationComponent>();
    
    if (!animation.empty()) {
        return animation[0]->getSprite().getGlobalBounds();
    }
    else {
        return FloatRect(0, 0, 0, 0);
    }
}
