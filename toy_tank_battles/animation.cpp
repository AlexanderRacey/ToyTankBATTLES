#include "animation.h"
#include "system_renderer.h"
#include <cmath>

using namespace sf;
using namespace std;

// Create animation component
AnimationComponent::AnimationComponent(Entity* p, Vector2f size) : Component(p), set_size(size) 
{
	animationTimer = 0.0f;
	currentFrame.width = set_size.x;
	currentFrame.height = set_size.y;
	currentTexture = Vector2u(0, 0);
	frameTime = 0.2f;
	frameCount = 1;
	end = false;
	auto origin = set_size / 2.f;
	origin.x = floor(origin.x);
	origin.y = floor(origin.y);
	sprite.setOrigin(origin);
}

void AnimationComponent::update(double dt)
{
	end = false;
	animationTimer += dt;
	if (animationTimer > frameTime) 
	{
		animationTimer -= frameTime;
		currentTexture.x++;
		if (currentTexture.x >= frameCount) 
		{
			end = true;
			currentTexture.x = 0;
		}
	}

	currentFrame.left = currentTexture.x * set_size.x;
	currentFrame.top = currentTexture.y * set_size.y;
	sprite.setTextureRect(currentFrame);
	sprite.setPosition(_parent->getPosition());
}

void AnimationComponent::render() 
{
	Renderer::queueAnimation(sprite);
}

void AnimationComponent::setSpritesheet(const Texture& setSpritesheet) 
{
	spritesheet = setSpritesheet;
	sprite.setTexture(spritesheet);
}

void AnimationComponent::setFrameTime(float time) 
{
	frameTime = time;
}

void AnimationComponent::setFrameCount(int count) 
{
	frameCount = count;
}

void AnimationComponent::setSize(const Vector2f& size)
{
	set_size = size;
	currentFrame.left = set_size.x;
	currentFrame.top = set_size.y;

	auto origin = set_size / 2.f;
	origin.x = floor(origin.x);
	origin.y = floor(origin.y);
	sprite.setOrigin(origin);
}

void AnimationComponent::setScale(const Vector2f& scale)
{
	//set_size = size;
	sprite.setScale(scale);
}

void AnimationComponent::setRotation(float rot) {
	sprite.setRotation(rot);
}

void AnimationComponent::rotate(float rot) {
	sprite.rotate(rot);
}

float AnimationComponent::getRotation() {
	return sprite.getRotation();
}

const Vector2f& AnimationComponent::getSize() const 
{
	return set_size;
}

int AnimationComponent::getFrameCount() const 
{
	return frameCount;
}

float AnimationComponent::getFrameTime() const 
{
	return frameTime;
}

bool AnimationComponent::finished() const 
{
	return end;
}

const Sprite& AnimationComponent::getSprite() const 
{
	return sprite;
}