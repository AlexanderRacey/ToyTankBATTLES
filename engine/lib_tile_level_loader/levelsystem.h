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

        enum TILES
        {
            EMPTY = ' ',
            START = 's',
            END = 'e',
            HOUSE = 'w',
            HOUSE_R = 'r',
            ENEMY = 'n',
            WAYPOINT = '+',
            PICKUP = 'p',
            BROKEN = 'b',
            BROKEN_R = 'q',
            WALL = 'l'
        };

        static Tile getTile(Vector2ul);
        static Tile getTileAt(Vector2f);
        static bool isOnGrid(Vector2f);
        static size_t getWidth();
        static size_t getHeight();
        static Vector2f getTilePosition(Vector2ul);
        static vector<Vector2ul> findTiles(Tile);
        static shared_ptr<sf::Texture> getTexture(LevelSystem::Tile t);
        static void loadTextures();
        static void setTexture(Tile t, shared_ptr<sf::Texture> tex);
        static void setOffset(const Vector2f& _offset);
        static const Vector2f& getOffset();
        static float getTileSize();
        static vector<unique_ptr<sf::Sprite>> _sprites;
        static bool isWall(Tile t);
    protected:
        static unique_ptr<Tile[]> _tiles;
        static size_t _width;
        static size_t _height;
        static Vector2f _offset;

        static void buildSprites();
        static float _tileSize; // for rendering
        static std::map<Tile, sf::Color> _colours;
        static std::map<Tile, shared_ptr<sf::Texture>> _textures;
    private:
        LevelSystem() = delete;

        ~LevelSystem() = delete;
};