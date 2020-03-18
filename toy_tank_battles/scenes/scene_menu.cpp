// scene_menu.cpp
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"

using namespace std;
using namespace sf;

void MenuScene::Load()
{
  cout << "Menu Load \n";
  {
    auto txt = makeEntity();
    auto t = txt->addComponent<TextComponent>("Toy Tank BATTLES\nPress Space to Start");
  }
  setLoaded(true);
}

void MenuScene::Update(const double& dt) 
{
  // cout << "Menu Update "<<dt<<"\n";

  /*if (Keyboard::isKeyPressed(Keyboard::Space)) 
  {
    Engine::ChangeScene(&level1);
  }*/

  Scene::Update(dt);
}
