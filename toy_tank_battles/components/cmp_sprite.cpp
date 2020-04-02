#include "cmp_sprite.h"
#include "system_renderer.h"

using namespace sf;
using namespace std;

SpriteComponent::SpriteComponent(Entity* p) : Component(p), _sprite(make_shared<sf::Sprite>()) {}

void SpriteComponent::update(double dt) 
{
    _sprite->setPosition(_parent->getPosition());
    _sprite->setRotation(_parent->getRotation());
}

void SpriteComponent::render() { Renderer::queue(_sprite.get()); }

Sprite& SpriteComponent::getSprite() const { return *_sprite; }

void SpriteComponent::setTexture(std::shared_ptr<sf::Texture> tex) {
    _texture = tex;
    _sprite->setTexture(*_texture);
}