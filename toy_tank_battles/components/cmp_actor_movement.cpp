#include <system_resources.h>
#include <levelsystem.h>
#include "cmp_actor_movement.h"
#include "cmp_bullet.h"
#include "cmp_physics.h"
#include "../animation.h"
#include "../add_entity.h"
#include "cmp_breakable.h"

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
//direction order : right, down, up, left
static const Vector2i directions[] = { {1,0}, {0,1}, {0, -1}, {-1, 0} };
const float rotations[] = { 90.f, 180.f, 0.f, 270.f};


EnemyAiComponent::EnemyAiComponent(Entity* p) : ActorMovementComponent(p), _state(MOVING), _offset(Vector2f(0, 0)), _direction(Vector2f(0, 0)), gap(30.f) 
{
    ChangeDirection();
    setSpeed(50.f);
    setRotation(rotations[index]);
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
   // auto breakable;

    switch (_state)
    {
        case EnemyAiComponent::MOVING:
            if (validMove(testPos)) 
            {
                move(newpos);
            }
            else if (ls::BROKEN == ls::getTileAt(testPos) ||
                ls::BROKEN_R == ls::getTileAt(testPos)) {
                vector<shared_ptr<Entity>> potTargets = Engine::findEntity("brokenHouse");
                for (auto t : potTargets) {
                    auto sp = t->GetCompatibleComponent<SpriteComponent>();
                    auto bounds = sp[0]->getSprite().getGlobalBounds();
                    //cheating the left detection by making width much wider for now
                    bounds = FloatRect(bounds.left - 50.f, bounds.top -50.f, bounds.width + 120.f, bounds.height + 50.f);
                    
                    if (bounds.contains(testPos)) {
                        if (t->isAlive()) {
                            target = t;
                            fireTimer = 0.5f;
                            _state = AIMING;
                            blocked = true;
                            break;
                        }
                    }
                    if (!blocked) {
                        move(newpos);
                  }
                }
       
            }
            else
            {
                ChangeDirection();
                _state = ROTATING;
            }
            break;
        case EnemyAiComponent::SHOTING:
            if (target->isAlive()) {
             auto breakable = target->GetCompatibleComponent<BreakableComponent>();
                if (!breakable.empty()) {
                    if (!breakable[0]->isExploded()) {
                        fireTimer -= dt;
                        if (fireTimer <= 0) {
                            fire();
                            fireTimer = 3.f;
                            }
                    }
                  //  else {
                   //     blocked
                   // }
                    }
            }else{
     //       fire();
            _state = MOVING;
            blocked = false;
            }
            break;
        case EnemyAiComponent::WAIT:
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
        case EnemyAiComponent::AIMING:
            aimTurrent(testPos);
            _state = SHOTING;
            break;
        default:
            break;
    }
}

void EnemyAiComponent::ChangeDirection()
{
    Vector2f newDir;
    Vector2f newPos;
    index = 0;
    do
    {
        index = rand() % 4;
        newDir = Vector2f(directions[index]);
    } while (newDir == _direction);

    switch (index)
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

void EnemyAiComponent::aimTurrent(Vector2f Pos) {
    //Aim turrent depending on tank direction it is facing
    //direction order : right, down, up, left
    float m1 = 0.f;
    float m2 = 0.f;
    float form = 0.f;
    tAngle = 0.f;
    Vector2f tilePos = ls::getTilePosAt(Pos);
    switch (index) {
    case 0:
        //facing right
        m1 = _parent->getPosition().x;
        m2 = tilePos.x;
        form = (m2 - m1) / (1 + m2 * m1);
        tAngle = -(atan(form) * 10000);
        setTurrentRotation(tAngle);
        break;
    case 1:
        //facing downwards
        m1 = _parent->getPosition().y;
        m2 = tilePos.y;
        form = (m2 - m1) / (1 + m2 * m1);
        tAngle = atan(form) * 10000;
        setTurrentRotation(tAngle);
     
        break;
    case 2:
        //facing upwards
        m1 = _parent->getPosition().y;
        m2 = tilePos.y;
        form = (m2 - m1) / (1 + m2 * m1);
        tAngle = -(atan(form) * 10000);
        setTurrentRotation(tAngle);
        break;
    case 3:
        //facing left
        m1 = _parent->getPosition().x;
        m2 = tilePos.x;
        form = (m2 - m1) / (1 + m2 * m1);
        tAngle = -(atan(form) * 10000);
        setTurrentRotation(tAngle);
        break;
    }

}

void EnemyAiComponent::setTurrentRotation(float rot) {
    auto animation = _parent->GetCompatibleComponent<EnemyAnimationComp>();

    if (!animation.empty())
    {
        animation[0]->rotateTurret(rot);
    }
}

float EnemyAiComponent::getTurrentRotation() {
    auto animation = _parent->GetCompatibleComponent<EnemyAnimationComp>();

    if (!animation.empty())
    {
        return animation[0]->getTurretRotation();
    }
}

void EnemyAiComponent::fire() {

    auto bullet = _parent->scene->makeEntity();
    bullet->setPosition(_parent->getPosition());
    auto bulletcomp = bullet->addComponent<BulletComponent>();
    bulletcomp->setTarget(target);
    switch (index)
    {
    case 0:
        //facing right
        bulletcomp->setDirection(_direction);
        break;
    case 1:
        //facing downwards
        bulletcomp->setDirection(_direction);
        break;
    case 2:
        //facing upwards
        bulletcomp->setDirection(_direction);
        break;
    case 3:
        //facing left
        bulletcomp->setDirection(_direction);
        break;
    }
   

    auto spriteB = bullet->addComponent<SpriteComponent>();
   // Texture bulletTexture = Resources::load<Texture>("enemyBullet.png");
    spriteB->setTexture(Resources::load<Texture>("enemyBullet.png"));
    spriteB->getSprite().setScale(Vector2f(0.6, 0.6));
    auto bounds = spriteB->getSprite().getGlobalBounds();
    spriteB->getSprite().setOrigin(Vector2f(bounds.getSize().x / 2, bounds.getSize().y));
    // float inverse = fmod((getTurrentRotation() + 180.f), 360);  //Sets rotation of bullet to be inverse of ship rotation, using fancy maths.
    bullet->setRotation(getTurrentRotation());
}
