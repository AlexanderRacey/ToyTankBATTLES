#pragma once
#include "engine.h"
#include "SFML/Graphics.hpp"

#define MAX_NUMBER_OF_HIGHSCORES 1

// Create high score scene from class Scene
class HighScoresScene : public Scene
{
    public:
        void Load() override;
        void UnLoad() override;
        void Update(const double& dt) override;
        void Render() override;

        void SetTitle();
        void SetBackground();
        int GetPressedItem() { return selectedItemIndex6; }

        IntRect uvRect;

    private:
        int selectedItemIndex6;
        Font font;
        Text highScoreMenu[MAX_NUMBER_OF_HIGHSCORES];
        Text highScoreText;
};