// cmp_text.h
#pragma once
#include <SFML/Graphics/Text.hpp>
#include <ecm.h>

using namespace sf;
using namespace std;

class TextComponent : public Component 
{
    public:
      TextComponent() = delete;
      explicit TextComponent(Entity* p, const string& str = "");
      void update(double dt) override;

      void render() override;

      ~TextComponent() override = default;

      void SetText(const string& str);

    protected:
      shared_ptr<Font> _font;
      string _string;
      Text _text;
};
