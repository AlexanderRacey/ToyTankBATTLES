// cmp_text.cpp
#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>

using namespace sf;
using namespace std;

void TextComponent::update(double dt) {}

void TextComponent::render() { Renderer::queue(&_text); }

TextComponent::TextComponent(Entity* const p, const string& str) : Component(p), _string(str) 
{
    _text.setString(_string);
    _font = Resources::get<Font>("OdibeeSans-Regular.ttf");
    _text.setFont(*_font);
}

void TextComponent::SetText(const string& str)
{
  _string = str;
  _text.setString(_string);
}
