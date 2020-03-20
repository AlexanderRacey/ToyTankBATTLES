#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "levelsystem.h"
#include "maths.h"
#include "system_renderer.h"
#include "system_resources.h"
#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"

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

Texture tankTexture;
IntRect tankSource(0, 0, 400, 300);
Sprite titleTank(tankTexture, tankSource);
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

}

void MenuScene::Load()
{
    // Display menu loading to console
    cout << "Menu Load \n";
    {
        // Get size of window
        float x2 = Engine::getWindowSize().x;
        float y2 = Engine::getWindowSize().y;
        SetTitle();

        titleTank.setPosition(x2 - 400.0f, 200.0f);
        tankTexture.loadFromFile("res/img/titleTank.png");

        font.loadFromFile("res/fonts/OdibeeSans-Regular.ttf");

        // Create menu
        menu[0].setFont(font);
        menu[0].setFillColor(Color::Red);
        menu[0].setString("New Game");
        menu[0].setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) + 40));

        menu[1].setFont(font);
        menu[1].setFillColor(Color::White);
        menu[1].setString("High Scores");
        menu[1].setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) + 80));

        menu[2].setFont(font);
        menu[2].setFillColor(Color::White);
        menu[2].setString("Settings");
        menu[2].setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) + 120));

        menu[3].setFont(font);
        menu[3].setFillColor(Color::White);
        menu[3].setString("Quit");
        menu[3].setPosition(Vector2f((x2 / 2) - 80, (y2 / 2) + 160));

        // Refers to currently selected menu item
        selectedItemIndex = 0;
    }
    setLoaded(true);
}

void MenuScene::Update(const double& dt) 
{
    Scene::Update(dt);

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

    if (fadeCounter <= 250) {
        titleSprite.setColor(Color(255, 255, 255, fadeCounter));
        fadeCounter++;
        Renderer::queue(&titleSprite);
    }
    else 
    {
        titleSprite.setColor(Color(255, 255, 255, 255));
        Renderer::queue(&titleSprite );
        for (int i = 0; i < MAX_MENU_ITEMS; i++)
        {
            Renderer::queue(&menu[i]);
        }
    }
}

// Unload scene function
void MenuScene::UnLoad() {
    float x2 = Engine::GetWindow().getSize().x;
    float y2 = Engine::GetWindow().getSize().y;
    Engine::GetWindow().setView(View(FloatRect(0, 0, x2, y2)));
    Scene::UnLoad();
}

void MenuScene::MoveUp()
{
    if (selectedItemIndex - 1 >= 0)
    {
        menu[selectedItemIndex].setFillColor(Color::White);
        selectedItemIndex--;
        menu[selectedItemIndex].setFillColor(Color::Red);
    }
}

void MenuScene::MoveDown()
{
    if (selectedItemIndex + 1 < MAX_MENU_ITEMS)
    {
        menu[selectedItemIndex].setFillColor(Color::White);
        selectedItemIndex++;
        menu[selectedItemIndex].setFillColor(Color::Red);
    }
}
