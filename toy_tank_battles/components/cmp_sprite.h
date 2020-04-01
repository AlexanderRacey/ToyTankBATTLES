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
        std::shared_ptr<sf::Texture> _texture;
    public:
        SpriteComponent() = delete;

        explicit SpriteComponent(Entity* p);
        void update(double dt) override;
        void render() override;

        Sprite& getSprite() const;

        void setTexture(std::shared_ptr<sf::Texture> tex);
        /*
        template <typename... Targs> void setSprite(Targs... params) 
        {
            _sprite.reset(new sf::Sprite(params...));
        }*/
};
