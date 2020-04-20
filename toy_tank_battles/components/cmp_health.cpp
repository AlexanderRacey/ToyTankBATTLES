#include "cmp_health.h"

HealthComponent::HealthComponent(Entity* p, float health) : Component(p), _health(health) {};

void HealthComponent::setHealth(float health)
{
    _health = health;
}

void HealthComponent::deductHealth(float malus)
{
    _health -= malus;
}

float HealthComponent::getHealth()
{
    return _health;
}