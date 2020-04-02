#include "system_renderer.h"
#include "engine.h"
#include <queue>

using namespace std;
using namespace sf;

static queue<const Drawable*> sprites;
static queue<Sprite> _sprites;
static RenderWindow* rw;

void Renderer::initialise(sf::RenderWindow& r) { rw = &r; }

void Renderer::shutdown() 
{
    while (!sprites.empty())
        sprites.pop();

    while (!_sprites.empty())
        _sprites.pop();
}

void Renderer::update(const double&) {}

void Renderer::render() 
{
    if (rw == nullptr) 
    {
        throw("No render window set! ");
    }
    while (!sprites.empty()) 
    {

        rw->draw(*sprites.front());
        sprites.pop();
    }
    while (!_sprites.empty()) 
    {
        rw->draw(_sprites.front());
        _sprites.pop();
    }
}

void Renderer::queue(const Drawable* s) 
{
    sprites.push(s);
}

void Renderer::queueAnimation(const Sprite& sprite) 
{
    _sprites.push(sprite);
}