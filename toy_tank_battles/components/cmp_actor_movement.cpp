#include <system_resources.h>
#include <levelsystem.h>
#include "cmp_actor_movevent.h"

using namespace sf;
using namespace std;

void ActorMovementComponent::render() {}

void ActorMovementComponent::update(double dt) {}

ActorMovementComponent::ActorMovementComponent(Entity* p) : _speed(100.0f), Component(p) {}

bool ActorMovementComponent::validMove(const Vector2f& pos)
{
	return (LevelSystem::getTileAt(pos) != LevelSystem::WALL);
}

void ActorMovementComponent::move(const Vector2f& p)
{
	auto pp = _parent->getPosition() + p;
	if (validMove(pp))
	{
		_parent->setPosition(pp);
	}
}

void ActorMovementComponent::move(float x, float y)
{
	move(Vector2f(x, y));
}

float ActorMovementComponent::getSpeed() const
{
	return _speed;
}

void ActorMovementComponent::setSpeed(float speed)
{
	_speed = speed;
}

void ActorMovementComponent::increaseSpeed(float speed)
{
	_speed += speed;
}

PlayerMovementComponent::PlayerMovementComponent(Entity* p) : ActorMovementComponent(p) {}

// Player movement
void PlayerMovementComponent::update(double dt)
{
	int xdir = 0, ydir = 0;

	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		move(Vector2f(0, -_speed * dt));
	}
	if (Keyboard::isKeyPressed(Keyboard::S))
	{
		move(Vector2f(0, _speed * dt));
	}
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		move(Vector2f(-_speed * dt, 0));
	}
	if (Keyboard::isKeyPressed(Keyboard::D))
	{
		move(Vector2f(_speed * dt, 0));
	}

	ActorMovementComponent::update(dt);
}

void PlayerMovementComponent::render()
{
	ActorMovementComponent::render();
}