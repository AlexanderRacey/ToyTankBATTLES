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
      void SetTitle();
      //void SetPickups();
      void SetBackground();
};
