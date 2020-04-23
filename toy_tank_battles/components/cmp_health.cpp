#include "cmp_health.h"
#include "cmp_text.h"

void HealthComponent::update(double dt)
{
    if (invincible) 
    {
        if (timer > 0) 
        {
            timer -= dt;
        }
        else 
        {
            invincible = false;
            _parent->GetCompatibleComponent<TextComponent>()[0]->SetText("Not Invincible", true);
            _parent->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Magenta);
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
    int healthmalus = 0;
    if (!invincible) {
        _health -= malus;
        healthmalus = round(malus);
    }
    _parent->GetCompatibleComponent<TextComponent>()[0]->SetText("Health -" + to_string(healthmalus), true);
    _parent->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Black);
}

void HealthComponent::addHealth(float bonus)
{
    int healthbonus = 0;
    if (_health < 100.f) 
    {
        _health += bonus;
        int healthbonus = round(bonus);
    }
    _parent->GetCompatibleComponent<TextComponent>()[0]->SetText("Health + 1"+ to_string(healthbonus), true);
    _parent->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Magenta);
    
}

void HealthComponent::setInvincible()
{
    invincible = true;
    timer = 5.f;
    _parent->GetCompatibleComponent<TextComponent>()[0]->SetText("Invincible !!", true);
    _parent->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Magenta);
}

float HealthComponent::getHealth()
{
    return _health;
}