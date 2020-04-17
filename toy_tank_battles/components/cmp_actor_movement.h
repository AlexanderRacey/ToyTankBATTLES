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

// Create player movement from class ActorMovementComponent
class PlayerMovementComponent : public ActorMovementComponent
{
    public:
		PlayerMovementComponent() = delete;
		explicit PlayerMovementComponent(Entity* p);
		void setRotation(float rot);

		void fire(float rotation);
		void move(const Vector2f& pos);
		void update(double dt) override;
		void render() override;
};


// Create enemy ai component from class ActorMovementComponent
class EnemyAiComponent : public ActorMovementComponent 
{
	private:
		Vector2f _direction;
		Vector2f _offset;
		int index;
		enum state { MOVING, SHOTING, WAIT, ROTATING, AIMING };
		state _state;
		float gap;
		bool turnRight = false;
		shared_ptr<Entity> target;
		float tAngle;

	public:
		explicit EnemyAiComponent(Entity* p);
		EnemyAiComponent() = delete;
		void ChangeDirection();
		void resetState();
		void setRotation(float rot);
		void rotate(float rot);
		float getRotation();
		void move(const Vector2f& pos);
		void update(double dt);
		FloatRect getBounds();
		void aimTurrent(Vector2f pos);
		void setTurrentRotation(float rot);
		void fire();
		float getTurrentRotation();
};
