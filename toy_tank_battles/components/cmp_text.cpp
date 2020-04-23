// cmp_text.cpp
#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>

using namespace sf;
using namespace std;

void TextComponent::update(double dt) {
    if (_timed) {
        if (timer > 0) {
            timer -= dt;
        }
        else {
            _show = false;
        }
    }

    _text.setPosition(_parent->getPosition() + textOffset);

}

void TextComponent::render() { 
    
    if (_show) {
        Renderer::queue(&_text);
    }
}

TextComponent::TextComponent(Entity* const p, const string& str) : Component(p), _string(str) 
{
    _text.setString(_string);
    _font = Resources::get<Font>("OdibeeSans-Regular.ttf");
    _text.setFont(*_font);
    _text.setCharacterSize(20);

}

Text& TextComponent::getText() { return _text; }

void TextComponent::Flash() {
    if (_show) {
        _show = false;
    }
    else {
        _show = true;
    }
}

void TextComponent::SetText(const string& str, bool timed)
{
    if (timed) {
        timer = 0.7f;   
    }
    _timed = timed;

    _string = str;
    _text.setString(_string);
    _show = true;
    
}

void TextComponent::setPosition(Vector2f pos) {
    textOffset = pos;
}


void TextComponent::setColour(Color col) {
    _text.setColor(col);
}

void TextComponent::setShow(bool show) {
    _show = show;
}
