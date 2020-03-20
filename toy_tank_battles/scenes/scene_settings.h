#pragma once
#include "engine.h"
#include "SFML/Graphics.hpp"

#define MAX_NUMBER_OF_SETTINGS 5

// Create settings scene from class Scene
class SettingsScene : public Scene 
{
    public:
      void Load() override;
      void UnLoad() override;
      void Update(const double& dt) override;
      void Render() override;

      void SetBackground();
      void MoveUp();
      void MoveDown();
      int GetPressedItem() { return selectedItemIndex2; }

      IntRect uvRect;

    private:
        int selectedItemIndex2;
        Font font;
        Text settingsMenu[MAX_NUMBER_OF_SETTINGS];
};
