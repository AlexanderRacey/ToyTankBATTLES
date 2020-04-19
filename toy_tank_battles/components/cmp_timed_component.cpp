#include "cmp_timed_component.h"

TimedComponent::TimedComponent(Entity* p, float timer) : Component(p), _timer(timer) {};

void TimedComponent::update(double dt){

  if (_timer > 0)
  {
    _timer -= dt;
  }else{
    _parent->setForDelete();
  }

}
