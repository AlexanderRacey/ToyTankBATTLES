#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "engine.h"
#include "levelsystem.h"
#include "maths.h"
#include "system_renderer.h"
#include "system_resources.h"
#include "scene_menu.h"
#include "scene_settings.h"
#include "../add_entity.h"
#include "../game.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"

using namespace std;
using namespace sf;

// Resources
Sprite titleSprite;
Texture titleTexture;
Vector2f targetCoords;
Vector2u titleTextureSize;
Vector2u windowSizeMenu;

Sprite backgroundSprite;
Texture backgroundTexture;
Vector2u backgroundSize;

Clock clock1;

int fadeCounter = 0;

// Display menu title
void MenuScene::SetTitle()
{
    titleTexture = *Resources::load<Texture>("titleName.png");
    float x = Engine::GetWindow().getSize().x;
    float y = Engine::GetWindow().getSize().y;
    titleTextureSize = titleTexture.getSize();
    windowSizeMenu = Engine::GetWindow().getSize();
    float scaleX = (float)windowSizeMenu.x / titleTextureSize.x;
    float scaleY = (float)windowSizeMenu.y / titleTextureSize.y;
    targetCoords = { x, y };
    titleSprite.setTexture(titleTexture);
    titleSprite.setPosition(windowSizeMenu.x / 2, windowSizeMenu.y / 3);
    titleSprite.setOrigin(titleTextureSize.x / 2, titleTextureSize.y / 2);
}

// Display background
void MenuScene::SetBackground()
{
    backgroundTexture = *Resources::load<Texture>("background.png");
    float x2 = Engine::GetWindow().getSize().x;
    float y2 = Engine::GetWindow().getSize().x;
    backgroundSize = backgroundTexture.getSize();
    windowSizeMenu = Engine::GetWindow().getSize();
    float scaleX2 = (float)windowSizeMenu.x / backgroundSize.x;
    float scaleY2 = (float)windowSizeMenu.y / backgroundSize.y;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(0, 0);
    backgroundSprite.setScale(scaleX2, scaleY2);
    backgroundSprite.setOrigin(0, 0);
    setLoaded(true);
}

void MenuScene::Load()
{
    // Display menu loading to console
    cout << "Menu Load \n";
    {
        // Get size of window
        float x3 = Engine::getWindowSize().x;
        float y3 = Engine::getWindowSize().y;
        SetBackground();
        SetTitle();

        font.loadFromFile("res/fonts/OdibeeSans-Regular.ttf");

        // Create menu
        menu[0].setFont(font);
        menu[0].setFillColor(Color(0, 168, 243, 255));
        menu[0].setString("New Game");
        menu[0].setPosition(Vector2f((x3 / 2) - 80, (y3 / 2) + 40));

        menu[1].setFont(font);
        menu[1].setFillColor(Color(255, 127, 39, 255));
        menu[1].setString("High Scores");
        menu[1].setPosition(Vector2f((x3 / 2) - 80, (y3 / 2) + 80));

        menu[2].setFont(font);
        menu[2].setFillColor(Color(255, 127, 39, 255));
        menu[2].setString("Settings");
        menu[2].setPosition(Vector2f((x3 / 2) - 80, (y3 / 2) + 120));

        menu[3].setFont(font);
        menu[3].setFillColor(Color(255, 127, 39, 255));
        menu[3].setString("Quit");
        menu[3].setPosition(Vector2f((x3 / 2) - 80, (y3 / 2) + 160));

        // Refers to currently selected menu item
        selectedItemIndex = 0;
    }
    setLoaded(true);
}

void MenuScene::Update(const double& dt) 
{
    Scene::Update(dt);

    float x4 = Engine::getWindowSize().x;
    float y4 = Engine::getWindowSize().y;

    Event event;
    while (Engine::GetWindow().pollEvent(event))
    {
        if (event.type == Event::Resized)
        {
            UnLoad();
            Engine::GetWindow().setSize(Vector2u(event.size.width, event.size.height));
            Engine::GetWindow().display();
            Load();
        }
    }

    // Resets menu
    if (Keyboard::isKeyPressed(Keyboard::R)) 
    {
        fadeCounter = 0;
        UnLoad();
        Load();
    }

    // Menu selection
    if (Keyboard::isKeyPressed(Keyboard::Up))
    {
        MoveUp();
        this_thread::sleep_for(chrono::milliseconds(170));
    }
    else if (Keyboard::isKeyPressed(Keyboard::Down))
    {
        MoveDown();
        this_thread::sleep_for(chrono::milliseconds(170));
    }

    // Load next scene 
    else if (Keyboard::isKeyPressed(Keyboard::Enter)) 
    {
        switch (GetPressedItem())
        {
            case 0:
                Engine::ChangeScene(&level1);
                break;
            case 1:
                Engine::ChangeScene(&highscores);
                break;
            case 2:
                Engine::ChangeScene(&settings);
                this_thread::sleep_for(chrono::milliseconds(170));
                break;
            case 3:
                Engine::GetWindow().close();
                break;
        }
    }
}

// Render function
void MenuScene::Render()
{
    Scene::Render();

    if (fadeCounter <= 250)
    {
        titleSprite.setColor(Color(255, 255, 255, fadeCounter));
        fadeCounter++;
        Renderer::queue(&backgroundSprite);
        Renderer::queue(&titleSprite);
    }
    else 
    {
        backgroundSprite.setColor(Color(255, 255, 255, 255));
        titleSprite.setColor(Color(255, 255, 255, 255));
        Renderer::queue(&backgroundSprite);
        Renderer::queue(&titleSprite);

        for (int i = 0; i < MAX_MENU_ITEMS; i++)
        {
            Renderer::queue(&menu[i]);
        }
    }
}

// Unload scene function
void MenuScene::UnLoad()
{
    float x2 = Engine::GetWindow().getSize().x;
    float y2 = Engine::GetWindow().getSize().y;
    Engine::GetWindow().setView(View(FloatRect(0, 0, x2, y2)));
    Scene::UnLoad();
}

void MenuScene::MoveUp()
{
    if (selectedItemIndex - 1 >= 0)
    {
        menu[selectedItemIndex].setFillColor(Color(255, 127, 39, 255));
        selectedItemIndex--;
        menu[selectedItemIndex].setFillColor(Color(0, 168, 243, 255));
    }
}

void MenuScene::MoveDown()
{
    if (selectedItemIndex + 1 < MAX_MENU_ITEMS)
    {
        menu[selectedItemIndex].setFillColor(Color(255, 127, 39, 255));
        selectedItemIndex++;
        menu[selectedItemIndex].setFillColor(Color(0, 168, 243, 255));
    }
}
