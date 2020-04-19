#pragma once
#include "engine.h"

using namespace sf;
using namespace std;

// Add entity class
class AddEntity
{
	public:
		static shared_ptr<Entity> makePlayer(Scene* scene, const Vector2f& pos);
		static shared_ptr<Entity> makeEnemy(Scene* scene, const Vector2f& pos);
		static shared_ptr<Entity> makeSmoke(Scene* scene, const Vector2f& pos);
		AddEntity() = delete;
};