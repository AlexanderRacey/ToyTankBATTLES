#pragma once
#include "cmp_sprite.h"
#include "ecm.h"
#include <Box2D/Dynamics/b2Body.h>

using namespace sf;
using namespace std;

// Create physics component from class Component
class PhysicsComponent : public Component
{
    protected:
        b2Body* _body;
        const bool _dynamic;
        b2Fixture* _fixture;

    public:
        PhysicsComponent(Entity* p, bool dyn, const Vector2f& size);

        b2Fixture* const getFixture() const;
        bool isTouching(const PhysicsComponent& pc) const;
        bool isTouching(const PhysicsComponent& pc, b2Contact const* bc) const;
        vector<const b2Contact*> getTouching() const;
        const Vector2f getVelocity() const;
        void setRestitution(float r);
        void setFriction(float r);
        void setMass(float m);
        void update(double dt) override;
        void render() override;
        void impulse(const Vector2f& i);
        void dampen(const Vector2f& s);
        void setVelocity(const Vector2f& v);
        void teleport(const Vector2f& v);
        void setGravityScale(int32 gs);
        ~PhysicsComponent() override;
};