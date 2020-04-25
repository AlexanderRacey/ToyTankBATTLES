#pragma once
#include "ecm.h"

using namespace sf;
using namespace std;

// Create pickup component from class component
class PickupComponent : public Component
{
    protected:
        int type;

    public:
        PickupComponent() = delete;
        explicit PickupComponent(Entity* p, int t);
        void update(double dt) override;
        void render() override;
};
