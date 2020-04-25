// LevelSystem Class file
#include "levelsystem.h"
#include <fstream>
#include <iostream>
#include "../src/system_resources.h"

using namespace std;
using namespace sf;


unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;

float LevelSystem::_tileSize(90.f);
Vector2f LevelSystem::_offset(0.0f, 0.0f);

//Texture references
shared_ptr<Texture> sand;
shared_ptr<Texture> house1;
shared_ptr<Texture> house2;
shared_ptr<Texture> house3;
shared_ptr<Texture> house4;
shared_ptr<Texture> house5;
shared_ptr<Texture> wall;

//seperate array for different house textures
vector<shared_ptr<Texture>> houses;

//base textures to fill tile sprites
map<LevelSystem::Tile, shared_ptr<Texture>> LevelSystem::_textures {};

//Sprite array holding sprite representationn of tiles after level is loaded
vector<unique_ptr<Sprite>> LevelSystem::_sprites;

//get Textures based on tile types
shared_ptr<Texture> LevelSystem::getTexture(LevelSystem::Tile t)
{
    if (t == HOUSE || t == HOUSE_R)
    {
        return houses[rand() % 5];
    }
    else 
    {
        auto it = _textures.find(t);
        if (it == _textures.end())
        {
            _textures[t] = Resources::load<Texture>("sand2.png");
        }
        return _textures[t];
    }
}

//sets up texture arrays
void LevelSystem::loadTextures() 
{
    sand = Resources::load<Texture>("sand2.png");
    house1 = Resources::load<Texture>("BlueHouse1.png");
    house2 = Resources::load<Texture>("BlueHouse2.png");
    house3 = Resources::load<Texture>("OrangeHouse.png");
    house4 = Resources::load<Texture>("PurpleHouse.png");
    house5 = Resources::load<Texture>("YellowHouse.png");
    wall = Resources::load<Texture>("GreyWall.png");
    houses = { house1, house2, house3, house4, house5 };
    _textures = { {EMPTY, sand }, {HOUSE, house1}, {WALL, wall} };
}

//for new texture setup
void LevelSystem::setTexture(LevelSystem::Tile t, shared_ptr<Texture> tex)
{
    _textures[t] = tex;
}

//loads tile array and sprites from text file, containing level information
void LevelSystem::loadLevelFile(const string& path, float tileSize)
{
    _tileSize = tileSize;
    size_t w = 0, h = 0;
    string buffer;
    ls::loadTextures();
    _offset = _offset + Vector2f(_tileSize / 2, _tileSize / 2);

    // Load in file to buffer
    ifstream f(path);
    if (f.good())
    {
        f.seekg(0, ios::end);
        buffer.resize(f.tellg());
        f.seekg(0);
        f.read(&buffer[0], buffer.size());
        f.close();
    }
    else 
    {
        throw string("Couldn't open level file: ") + path;
    }

    vector<Tile> temp_tiles;
    int widthCheck = 0;
    for (int i = 0; i < buffer.size(); ++i) 
    {
        const char c = buffer[i];
        if (c == '\0')
        {
            break;
        }
        if (c == '\n') 
        { // newline
            if (w == 0) 
            {  // if we haven't written width yet set width
                w = i;       
            }
            else if (w != (widthCheck - 1))
            {
                throw string("non uniform width:" + to_string(h) + " ") + path;
            }
            widthCheck = 0;
            h++; // increment height
        }
        else 
        {
            temp_tiles.push_back((Tile)c);
        }
        ++widthCheck;
    }

    if (temp_tiles.size() != (w * h)) 
    {
        throw string("Can't parse level file") + path;
    }
    _tiles = make_unique<Tile[]>(w * h);
    _width = w; // set static class vars
    _height = h;
    copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
    cout << "Level " << path << " Loaded. " << w << "x" << h << endl;

}

//Builds sprites array based of tiles array
void LevelSystem::buildSprites()
{
    _sprites.clear();

    for (size_t y = 0; y < LevelSystem::getHeight(); ++y) 
    {
        for (size_t x = 0; x < LevelSystem::getWidth(); ++x) 
        {
            auto s = make_unique<Sprite>();
            s->setTexture(*ls::getTexture(getTile({ x, y })));
            //if house tile is rotated, rotate sprite
            if (getTile({ x, y }) == HOUSE_R) 
            {
                //If House is on left boundary rotate so that it is facing right
                if (x == 0) 
                {
                    s->setRotation(270.f);
                }
                //otherwise rotate facing left
                else 
                {
                    s->setRotation(90.f);
                }  
            }
            //If house is located on lowest border rotate so that it is facing upwarts
            if (getTile({ x, y }) == HOUSE)
            {
                if (y == ls::getHeight() - 1)
                {
                    s->setRotation(180.f);
                }
            }
            s->setPosition(getTilePosition({ x,y }));
            s->setTextureRect(IntRect(0, 0, _tileSize, _tileSize));
            s->setOrigin(_tileSize/2, _tileSize/2);
            _sprites.push_back(move(s));
        }
    }
}

//render sprites
void LevelSystem::render(RenderWindow& window)
{
    for (auto& t : _sprites)
    {
        window.draw(*t);
    }
}

//returns tile based on location in tiles array
LevelSystem::Tile LevelSystem::getTile(Vector2ul p) 
{
    if (p.x > _width || p.y > _height)
    {
        throw string("Tile out of range: ") + to_string(p.x) + "," + to_string(p.y) + ")";
    }
    return _tiles[(p.y * _width) + p.x];
}

size_t LevelSystem::getWidth() { return _width; }

size_t LevelSystem::getHeight() { return _height; }

//return tile position if tile array position is known
Vector2f LevelSystem::getTilePosition(Vector2ul p)
{
    return (Vector2f(p.x, p.y) * _tileSize) + _offset;
}

//returns array of all types existing in tile array of one type
vector<Vector2ul> LevelSystem::findTiles(LevelSystem::Tile type) 
{
    auto v = vector<Vector2ul>();
    for (size_t i = 0; i < _width * _height; ++i) 
    {
        if (_tiles[i] == type) 
        {
            v.push_back({ i % _width, i / _width });
        }
    }
    return v;
}

//returns tile at position
LevelSystem::Tile LevelSystem::getTileAt(Vector2f v) 
{
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0) 
    {
        return NOTVALID;
    }
    else 
    {
        return getTile(Vector2ul((v - _offset) / (_tileSize)));
    }
}

//returns tile at position and allows to specify offset
//used for checking in changing directions
LevelSystem::Tile LevelSystem::getTileAt(Vector2f v,  Vector2f offset)
{
    auto off = _offset - Vector2f(_tileSize / 2, _tileSize / 2);
    off = off + offset;
    auto a = v - off;
    if (a.x < 0 || a.y < 0) {

        throw string("Tile out of range ");
    }
    return getTile(Vector2ul((v - off) / (_tileSize)));
}

//get tile position at location
Vector2f LevelSystem::getTilePosAt(Vector2f v)
{
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0)
    {
        throw string("Tile out of range ");
    }
    else 
    {
        return getTilePosition(Vector2ul((v - _offset) / (_tileSize)));
    }
}


//sets offset and rebuilds sprites to include new offset when setting postion
void LevelSystem::setOffset(const Vector2f& offset) 
{
    LevelSystem::_offset = offset + Vector2f(_tileSize/2, _tileSize/2);
    buildSprites();
}

void LevelSystem::unload()
{
    cout << "LevelSystem unloading\n";
    _textures.clear();
    _sprites.clear();
    _tiles.reset();
    _width = 0;
    _height = 0;
    _offset = { 0, 0 };
}

const Vector2f& LevelSystem::getOffset() { return _offset; }

float LevelSystem::getTileSize() { return _tileSize; }

//returns if tile is wall, something blocking entity
bool LevelSystem::isWall(Tile t)
{
    return (t == HOUSE || t == HOUSE_R || t == WALL || t == BROKEN || t == BROKEN_R || t == NOTVALID);
}

//returns if tile is a wall that can not be broken
bool LevelSystem::isSolidWall(Tile t)
{
    return (t == HOUSE || t == HOUSE_R || t == WALL || t == NOTVALID);
}