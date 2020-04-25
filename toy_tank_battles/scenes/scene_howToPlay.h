#pragma once
#include "engine.h"
#include "SFML/Graphics.hpp"

#define MAX_NUMBER_OF_INSTRUCTIONS 1

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
      int GetPressedItem() { return selectedItemIndex3; }

      IntRect uvRect;

    private:
        int selectedItemIndex3;
        Font font;
        Text instructionMenu[MAX_NUMBER_OF_INSTRUCTIONS];
        Text storyText;
        Text instructionList;
};
