#pragma once
#include <SFML/Graphics/Text.hpp>
#include <ecm.h>

using namespace sf;
using namespace std;

// Create text componemt from class Component
class TextComponent : public Component 
{
    public:
      TextComponent() = delete;
      explicit TextComponent(Entity* p, const string& str = "");
      void update(double dt) override;
      void render() override;

      ~TextComponent() override = default;

      void setPosition(Vector2f pos);
      void setColour(Color col);
      void SetText(const string& str, bool timed);
      void setShow(bool show);
      Text& getText();
      void Flash();

    protected:
      shared_ptr<Font> _font;
      string _string;
      Text _text;
      Color _color;
      bool _show = false;
      bool _timed = false;
      float timer = 0;
      Vector2f textOffset;
};
