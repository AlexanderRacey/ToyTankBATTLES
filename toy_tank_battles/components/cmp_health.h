#pragma once
#include <ecm.h>

using namespace sf;
using namespace std;

class HealthComponent : public Component
{
  protected:
    float _health;
    float timer = 0.0;
    bool invincible = false;
  public:
    void update(double dt) override ;
    void render() override {};
    explicit HealthComponent(Entity* p, float health = 100.f);
    HealthComponent() = delete;
    void setHealth(float health);
    void addHealth(float health);
    void deductHealth(float malus);
    void setInvincible();
    float getHealth();

};
