#pragma once
#include "ecm.h"

using namespace sf;
using namespace std;

//create breakable component from class Component
class BreakableComponent : public Component
{
protected:
    bool _exploded;
    float _timer;
public:
    BreakableComponent() = delete;
    explicit BreakableComponent(Entity* p);
    void update(double dt) override;
    void render() override {};
    void setExploded();
    bool isExploded();
};
