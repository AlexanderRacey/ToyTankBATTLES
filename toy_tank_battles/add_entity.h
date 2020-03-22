#pragma once
#include "engine.h"

using namespace sf;
using namespace std;

// Add entity class
class AddEntity
{
	public:
		static shared_ptr<Entity> makePlayer(Scene* scene, const Vector2f& pos);
		static shared_ptr<Entity> makeFakePlayer1(Scene* scene, const Vector2f& pos, const Vector2f& distance, float time);
		static shared_ptr<Entity> makeFakePlayer2(Scene* scene, const Vector2f& pos, const Vector2f& distance, float time);
		AddEntity() = delete;
};