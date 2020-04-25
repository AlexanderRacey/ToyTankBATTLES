#pragma once
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "ecm.h"

using namespace sf;
using namespace std;

// Create sprite componemt from class Component
class SpriteComponent : public Component 
{
    protected:
        shared_ptr<Sprite> _sprite;
        shared_ptr<Texture> _texture;
    public:
        SpriteComponent() = delete;

        explicit SpriteComponent(Entity* p);
        void update(double dt) override;
        void render() override;
        FloatRect getBounds();

        Sprite& getSprite() const;

        void setTexture(shared_ptr<Texture> tex);

        template <typename... Targs> void setSprite(Targs... params) 
        {
            _sprite.reset(new sf::Sprite(params...));
        }
};

// Create shape component from class Component
class ShapeComponent : public Component 
{
    protected:
        shared_ptr<Shape> _shape;

    public:
        ShapeComponent() = delete;

        explicit ShapeComponent(Entity* p);

        void update(double dt) override;
        void render() override;
        Shape& getShape() const;
        template <typename T, typename... Targs> void setShape(Targs... params)
        {
            _shape.reset(new T(params...));
        }
};
