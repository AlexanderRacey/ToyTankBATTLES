#pragma once
#include "engine.h"
#include "SFML/Graphics.hpp"

// Variable for items on menu
#define MAX_MENU_ITEMS 4

// Create menu scene from class Scene
class MenuScene : public Scene 
{
    public:
      void Load() override;
      void UnLoad() override;
      void Update(const double& dt) override;
      void Render() override;
      void SetTitle();
      void SetBackground();
      void MoveUp();
      void MoveDown();
      int GetPressedItem() { return selectedItemIndex; }

      IntRect uvRect;

    private:
        int selectedItemIndex;
        Font font;
        Text menu[MAX_MENU_ITEMS];
};
