// scene_menu.h
#pragma once
#include "engine.h"

// Create menu scene from class Scene
class MenuScene : public Scene 
{
    public:
      MenuScene() = default;
      ~MenuScene() override = default;

      void Load() override;

      void Update(const double& dt) override;
};
