#include "cmp_sprite.h"
#include "system_renderer.h"

using namespace sf;
using namespace std;

// -- Sprite --
// Set texture to sprite
void SpriteComponent::setTexture(shared_ptr<Texture> tex)
{
    _texture = tex;
    _sprite->setTexture(*_texture);
    _sprite->setTextureRect(IntRect(0, 0, _texture->getSize().x, _texture->getSize().y));
}

SpriteComponent::SpriteComponent(Entity* p) : Component(p), _sprite(make_shared<sf::Sprite>()) 
{
}

void SpriteComponent::update(double dt) 
{
    _sprite->setPosition(_parent->getPosition());
    _sprite->setRotation(_parent->getRotation());
}

void SpriteComponent::render() 
{
    Renderer::queue(_sprite.get());
}

Sprite& SpriteComponent::getSprite() const 
{ 
    return *_sprite; 
}

FloatRect SpriteComponent::getBounds() {
    return _sprite->getGlobalBounds();
}


// -- Shape --
void ShapeComponent::update(double dt)
{
    _shape->setPosition(_parent->getPosition());
    _shape->setRotation(_parent->getRotation());
}

void ShapeComponent::render() 
{
    Renderer::queue(_shape.get()); 
}

Shape& ShapeComponent::getShape() const 
{
    return *_shape;
}

ShapeComponent::ShapeComponent(Entity* p) : Component(p), _shape(make_shared<sf::CircleShape>()) 
{
}