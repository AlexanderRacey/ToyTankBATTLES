// LevelSystem Header file
#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "maths.h"

using namespace sf;
using namespace std;

#define ls LevelSystem

class LevelSystem 
{
    public:
        static void loadLevelFile(const string&, float tileSize = 100.0f);
        static void unload();
        static void render(RenderWindow& window);

        typedef unsigned char Tile;

        enum TILES {
            EMPTY = ' ',
            START = 's',
            END = 'e',
            WALL = 'w',
            ENEMY = 'n',
            WAYPOINT = '+',
            PICKUP = 'p',
            BROKEN = 'b'
        };

        static Tile getTile(Vector2ul);
        static Tile getTileAt(Vector2f);
        static bool isOnGrid(Vector2f);
        static size_t getWidth();
        static size_t getHeight();
        static Vector2f getTilePosition(Vector2ul);
        static vector<Vector2ul> findTiles(Tile);
        static Color getColor(Tile t);
        static sf::Texture getTexture(LevelSystem::Tile t);
        static void loadTextures();
        static void setTexture(Tile t, sf::Texture tex);
        static void setColor(Tile t, Color c);
        static void setOffset(const Vector2f& _offset);
        static const Vector2f& getOffset();
        static float getTileSize();
       // static vector<unique_ptr<sf::Sprite>> getSprites();
    protected:
        static unique_ptr<Tile[]> _tiles;
        static size_t _width;
        static size_t _height;
        static Vector2f _offset;
        static sf::Texture sand;
        static sf::Texture house1;
        static sf::Texture house2;
        static sf::Texture house3;
        static sf::Texture house4;
        static sf::Texture brokenH;
        static vector<unique_ptr<sf::Sprite>> _sprites;

        static void buildSprites();
        static float _tileSize; // for rendering
        static std::map<Tile, sf::Color> _colours;
        static std::map<Tile, sf::Texture> _textures;
    private:
        LevelSystem() = delete;

        ~LevelSystem() = delete;
};