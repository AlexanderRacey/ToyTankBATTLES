#pragma once
#include <ecm.h>
#include "cmp_sprite.h"

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
		FloatRect getBounds();
        void render() override {}
        void update(double dt) override;
};


// Create player movement from class ActorMovementComponent
class PlayerMovementComponent : public ActorMovementComponent
{
	protected:
		//bool validMove(const Vector2f&);
		int _points;

	private:
		Vector2f direction = { 0,-1 };
		Vector2f _offset;
		float firetimer = 0;
		float damage = 20.f;
		bool isBlocked(Vector2f pos);

    public:
		PlayerMovementComponent() = delete;
		explicit PlayerMovementComponent(Entity* p);
		void setRotation(float rot);
		float getRotation();
	
		void move(const Vector2f& pos);
		void update(double dt) override;
		void render() override;
		void addDamage(float dam);
		void fire();
		int getPoints() const;
		void setPoints(int pts);
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
		bool turrentRight = false;
		bool blocked = false;
		shared_ptr<Entity> target;
		float tAngle;
		float fireTimer = 0.f;
		bool turnTurrentRight(Vector2f pos);

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
		void aimTurrent();
		void setTurrentRotation(float rot);
		void fire();
		float getTurrentRotation();
};