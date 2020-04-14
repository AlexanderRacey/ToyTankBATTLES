#pragma once
#include <ecm.h>

using namespace sf;
using namespace std;

// Create actor movement component from class Component
class ActorMovementComponent : public Component
{
    protected:
        bool validMove(const Vector2f&);
        float _speed;

    public:
        explicit ActorMovementComponent(Entity* p);
        ActorMovementComponent() = delete;

        float getSpeed() const;
        void setSpeed(float speed);


        void move(const Vector2f& pos);
        void move(float x, float y);
        void increaseSpeed(float sp);

        void render() override {}
        void update(double dt) override;
};

// Create player movement from class ActoMovementComponent
class PlayerMovementComponent : public ActorMovementComponent
{
    public:
    PlayerMovementComponent() = delete;
    explicit PlayerMovementComponent(Entity* p);
	void setRotation(float rot);

	void move(const sf::Vector2f& pos);
	void update(double dt) override;
	void render() override;
};

//enemy Ai movement
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
