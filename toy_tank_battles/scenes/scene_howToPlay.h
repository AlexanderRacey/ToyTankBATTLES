#pragma once
#include "engine.h"
#include "SFML/Graphics.hpp"

#define MAX_NUMBER_OF_INSTRUCTIONS 5

// Create how to play scene from class Scene
class HowToPlayScene : public Scene 
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
      int GetPressedItem() { return selectedItemIndex2; }

      IntRect uvRect;

    private:
        int selectedItemIndex2;
        Font font;
        Text settingsMenu[MAX_NUMBER_OF_INSTRUCTIONS];
};
