#pragma once
#include <ecm.h>
#include "cmp_physics.h"

using namespace sf;
using namespace std;

// Create bullet component from class Component
class BulletComponent : public Component
{
    protected:
        float _lifetime;
        float _size;
        float _impulse;
        float _speed;
        float _damage;
        Vector2f direction;
        shared_ptr<Entity> _target;
        bool targetset = false;

    public:
        void update(double dt) override;
        void render() override {};
        explicit BulletComponent(Entity* p, float lifetime = 3.f, float size = 1.f, float impulse = 1.f, float speed = 1.f, float damage = 1.f);
        BulletComponent() = delete;
        void setDirection(Vector2f dir);
        void move(double dt);
        void setTarget(shared_ptr<Entity> tar);
        bool checkCollision();
};