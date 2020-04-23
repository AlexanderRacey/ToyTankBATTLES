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
        float _speed;
        float _damage;
        Vector2f direction;
        shared_ptr<Entity> _target;
        bool targetset = false;

    public:
        void update(double dt) override;
        void render() override {};
        explicit BulletComponent(Entity* p, float lifetime = 3.f, float speed = 1.0f, float damage = 1.f);
        BulletComponent() = delete;

        void setDirection(Vector2f dir);
        void move(double dt);
        void setTarget(shared_ptr<Entity> tar);
      //  bool checkCollision();
        void setDamage(float dam);
        void addDamage(float dam);
};


class PlayerBullet : public BulletComponent
{
    public:
        void update(double dt) override;
        explicit PlayerBullet(Entity* p, float lifetime = 3.f, float speed = 1.f, float damage = 1.f, Vector2f _offset = {0,0});
        PlayerBullet() = delete;

        bool checkCollision();
        void setDirection(Vector2f dir);
    private:
        Vector2f _offset;
};

class EnemyBullet : public BulletComponent
{
public:
    void update(double dt) override;
    explicit EnemyBullet(Entity* p, float lifetime = 3.f, float speed = 1.f, float damage = 1.f, Vector2f _offset = { 0,0 });
    EnemyBullet() = delete;

    bool checkCollision();
    void setDirection(Vector2f dir);
private:
    Vector2f _offset;
};

// Create base bullet component from class Component
class BaseBulletComponent : public Component
{
    protected:
        float _lifetime;
        float _speed;
        char _type;

    public:
        void update(double dt) override;
        void render() override {}
        explicit BaseBulletComponent(Entity* p, float lifetime = 3.f, float speed = 700);
        BaseBulletComponent() = delete;

        float getSpeed();
        void setSpeed(float speed);

        char getType();
        void setType(char type);
};