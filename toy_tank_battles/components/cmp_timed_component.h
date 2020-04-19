#pragma once
#include <ecm.h>

class TimedComponent : public Component {
  private:
    float _timer;
  public:
    void update(double dt) override;
    void render() override {};
    explicit TimedComponent(Entity* p, float _timer = 3.f);
    TimedComponent() = delete;
};
