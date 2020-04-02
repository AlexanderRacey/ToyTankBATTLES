#pragma once
#include "engine.h"
#include "SFML/Graphics.hpp"

// Create level2 scene from class Scene
class Level2Scene : public Scene 
{
    public:
      void Load() override;
      void UnLoad() override;
      void Update(const double& dt) override;
      void Render() override;
      void SetTitle();
      void SetBackground();
};
