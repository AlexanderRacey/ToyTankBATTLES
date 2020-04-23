#pragma once
#include "engine.h"
#include "SFML/Graphics.hpp"
#include <SFML/Audio/Music.hpp>
#include "../components/cmp_music.h"

#define MAX_NUMBER_OF_GOMSGS 2

// Create level1 scene from class Scene
class GameOverScene : public Scene 
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
      int GetPressedItem() { return selectedItemIndex4; }

      IntRect uvRect;

    private:
        int selectedItemIndex4;
        Font font;
        Text gameOverMenu[MAX_NUMBER_OF_GOMSGS];
        Text gameOverHighScoreText;
};
