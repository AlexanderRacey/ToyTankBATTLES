#include "cmp_health.h"

void HealthComponent::update(double dt)
{
    if (invincible) {
        if (timer > 0) {
            timer -= dt;
        }
        else {
            invincible = false;
        }
    }
}

HealthComponent::HealthComponent(Entity* p, float health) : Component(p), _health(health) {};

void HealthComponent::setHealth(float health)
{
    _health = health;
}

void HealthComponent::deductHealth(float malus)
{
    if (!invincible) {
        _health -= malus;
    }
}

void HealthComponent::addHealth(float bonus)
{
        _health += bonus;
}

void HealthComponent::setInvincible()
{
    invincible = true;
    timer = 2.f;
}

float HealthComponent::getHealth()
{
    return _health;
}