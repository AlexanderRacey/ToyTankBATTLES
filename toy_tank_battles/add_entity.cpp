#include <SFML/Graphics/Transformable.hpp>
#include <levelsystem.h>
#include "add_entity.h"
#include "system_resources.h"

using namespace sf;
using namespace std;

shared_ptr<Entity> AddEntity::makePlayer(Scene* scene, const Vector2f& pos)
{
	auto player = scene->makeEntity();

	return player;
}