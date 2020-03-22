#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "ecm.h"

using namespace sf;
using namespace std;

// Create sprite componemt from class Component
class SpriteComponent : public Component 
{
    protected:
        shared_ptr<Sprite> _sprite;

    public:
        SpriteComponent() = delete;

        explicit SpriteComponent(Entity* p);
        void update(double dt) override;
        void render() override;

        Sprite& getSprite() const;

        template <typename... Targs> void setSprite(Targs... params) 
        {
            _sprite.reset(new sf::Sprite(params...));
        }
};
