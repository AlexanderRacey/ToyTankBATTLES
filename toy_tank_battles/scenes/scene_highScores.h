#pragma once
#include "engine.h"

// Create high score scene from class Scene
class HighScoresScene : public Scene 
{
    public:
      void Load() override;
      void UnLoad() override;
      void Update(const double& dt) override;
      void Render() override;
};
#pragma once
