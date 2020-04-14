#pragma once
#include "ecm.h"
#include "engine.h"

class ActorMovementComponent : public Component {
protected:
	bool validMove(const sf::Vector2f&);
	float _speed;

public:
	explicit ActorMovementComponent(Entity* p);
	ActorMovementComponent() = delete;

	float getSpeed() const;
	void setSpeed(float speed);

	void move(const sf::Vector2f& pos);
	void move(float x, float y);

	void render() override {}
	void update(double dt) override;
};

class PlayerMovementComponent : public ActorMovementComponent
{
public:
	PlayerMovementComponent() = delete;
	explicit PlayerMovementComponent(Entity* p);

	void move(const sf::Vector2f& pos);
	void update(double dt) override;
	void render() override;
};

class EnemyAiComponent : public ActorMovementComponent {
private:
	Vector2f _direction;
	Vector2f _offset;
	enum state { MOVING, SHOTING, ROTATING, ROTATED };
	state _state;
	float gap;
	bool turnRight = false;

public:
	explicit EnemyAiComponent(Entity* p);
	EnemyAiComponent() = delete;
	void ChangeDirection();
	void resetState();
	void setRotation(float rot);
	void rotate(float rot);
	float getRotation();
	void move(const sf::Vector2f& pos);
	void update(double dt);
	sf::FloatRect getBounds();
};