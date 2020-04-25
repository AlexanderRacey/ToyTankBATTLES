#pragma once
#include "engine.h"
#include "SFML/Graphics.hpp"

// Create level1 scene from class Scene
class Level1Scene : public Scene
{
    public:
        void Load() override;
        void UnLoad() override;
        void Update(const double& dt) override;
        void Render() override;
        //void SetTitle();
        void SetPickups();
        void SetBreakables();
        void SetBackground();

    private:
        shared_ptr<Sprite> _BackgroundSprite;
        shared_ptr<Texture> _Background;

    protected:
        Font font;
        int _playerScore = 0;
        int _playerHealth = 100;
   
        Text HUDtext;
};