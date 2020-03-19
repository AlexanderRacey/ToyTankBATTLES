// scene_level1.h
#pragma once
#include "engine.h"

// Create level scene from class Scene
class Level1Scene : public Scene 
{
    public:
      void Load() override;
      void UnLoad() override;
      void Update(const double& dt) override;
      void Render() override;
      void SetTitle();
      void SetBackground();
};
