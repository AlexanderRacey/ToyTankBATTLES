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
        void setSpeed(float _speed);

        void move(const Vector2f&);
        void move(float x, float y);
        void increaseSpeed(float sp);

        void render() override;
        void update(double dt) override;
};

// Create player movement from class ActoMovementComponent
class PlayerMovementComponent : public ActorMovementComponent
{
    public:
        PlayerMovementComponent() = delete;
        explicit PlayerMovementComponent(Entity* p);

        void update(double dt) override;
        void render() override;
};