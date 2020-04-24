#pragma once
#include "engine.h"
#include "SFML/Graphics.hpp"
#include <SFML/Audio/Music.hpp>
#include "../components/cmp_music.h"

#define MAX_NUMBER_OF_WMSGS 2

// Create level1 scene from class Scene
class WinnerScene : public Scene 
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
        int GetPressedItem() { return selectedItemIndex5; }

        IntRect uvRect;

    private:
        int selectedItemIndex5;
        Font font;
        Text winnerMenu[MAX_NUMBER_OF_WMSGS];
        Text winnerText;
        Text winnerHighScoreText;

};
