#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
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
#include "../components/cmp_music.h"
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

MusicPlayer s1;
MusicPlayer s2;
MusicPlayer s3;
MusicPlayer s4;

SoundBuffer effect1;
Sound sound1;
SoundBuffer effect2;
Sound sound2;

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
    Background = Resources::load<Texture>("background.png");
    float x2 = Engine::GetWindow().getSize().x;
    float y2 = Engine::GetWindow().getSize().x;
    Vector2u BackgroundSize = Background->getSize();
    Vector2u windowSizeLevel1 = Engine::GetWindow().getSize();
    float scaleX2 = (float)windowSizeLevel1.x / BackgroundSize.x;
    float scaleY2 = (float)windowSizeLevel1.y / BackgroundSize.y;
    BackgroundSprite = make_unique<Sprite>();
    BackgroundSprite->setTexture(*Background);
    BackgroundSprite->setPosition(0, 0);
    BackgroundSprite->setScale(scaleX2, scaleY2);
    BackgroundSprite->setOrigin(0, 0);
}

void MenuScene::Load()
{
    // Display menu loading to console
    cout << "Menu Load \n";
    {
        // Play menu music 
        s2.stop();
        s3.stop();
        s4.stop();
        s1.play1(0, true);
        s1.playing();

        // Load sound effects
        effect1.loadFromFile("res/sound/playerFire.ogg");
        sound1.setBuffer(effect1);
        sound1.setVolume(15.0f);
        effect2.loadFromFile("res/sound/enemyFire.ogg");
        sound2.setBuffer(effect2);
        sound2.setVolume(15.0f);

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
        menu[1].setString("How To Play");
        menu[1].setPosition(Vector2f((x3 / 2) - 80, (y3 / 2) + 80));

        menu[2].setFont(font);
        menu[2].setFillColor(Color(255, 127, 39, 255));
        menu[2].setString("High Scores");
        menu[2].setPosition(Vector2f((x3 / 2) - 80, (y3 / 2) + 120));

        menu[3].setFont(font);
        menu[3].setFillColor(Color(255, 127, 39, 255));
        menu[3].setString("Settings");
        menu[3].setPosition(Vector2f((x3 / 2) - 80, (y3 / 2) + 160));

        menu[4].setFont(font);
        menu[4].setFillColor(Color(255, 127, 39, 255));
        menu[4].setString("Quit");
        menu[4].setPosition(Vector2f((x3 / 2) - 80, (y3 / 2) + 200));

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
                sound2.play();
                Engine::ChangeScene(&level1);
                break;
            case 1:
                sound2.play();
                Engine::ChangeScene(&howtoplay);
                this_thread::sleep_for(chrono::milliseconds(170));
                break;
            case 2:
                sound2.play();
                Engine::ChangeScene(&highscores);
                break;
            case 3:
                sound2.play();
                Engine::ChangeScene(&settings);
                this_thread::sleep_for(chrono::milliseconds(170));
                break;
            case 4:
                sound2.play();
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
        Renderer::queue(BackgroundSprite.get());
        Renderer::queue(&titleSprite);
    }
    else 
    {
        BackgroundSprite->setColor(Color(255, 255, 255, 255));
        titleSprite.setColor(Color(255, 255, 255, 255));
        Renderer::queue(BackgroundSprite.get());
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
    BackgroundSprite.reset();
    Background.reset();
    Scene::UnLoad();
}

void MenuScene::MoveUp()
{
    if (selectedItemIndex - 1 >= 0)
    {
        sound1.play();
        menu[selectedItemIndex].setFillColor(Color(255, 127, 39, 255));
        selectedItemIndex--;
        menu[selectedItemIndex].setFillColor(Color(0, 168, 243, 255));
    }
}

void MenuScene::MoveDown()
{
    if (selectedItemIndex + 1 < MAX_MENU_ITEMS)
    {
        sound1.play();
        menu[selectedItemIndex].setFillColor(Color(255, 127, 39, 255));
        selectedItemIndex++;
        menu[selectedItemIndex].setFillColor(Color(0, 168, 243, 255));
    }
}
