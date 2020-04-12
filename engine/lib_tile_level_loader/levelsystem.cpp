// LevelSyatem Class file
#include "levelsystem.h"
#include <fstream>
#include <iostream>
#include "../src/system_resources.h"

using namespace std;
using namespace sf;


shared_ptr<Texture> sand;
shared_ptr<Texture> house1;
shared_ptr<Texture> house2;
shared_ptr<Texture> house3;
shared_ptr<Texture> house4;
shared_ptr<Texture> house5;
//shared_ptr<Texture> broken;
//shared_ptr<Texture> broken2;
shared_ptr<Texture> wall;

vector<shared_ptr<Texture>> houses;

map<LevelSystem::Tile, shared_ptr<Texture>> LevelSystem::_textures
{
};

vector<unique_ptr<Sprite>> LevelSystem::_sprites;

shared_ptr<Texture> LevelSystem::getTexture(LevelSystem::Tile t)
{
    if (t == HOUSE || t == HOUSE_R) {
        return houses[rand() % 5];
    }
    else {
        auto it = _textures.find(t);
        if (it == _textures.end())
        {
            _textures[t] = Resources::load<Texture>("sand2.png");
        }
        return _textures[t];
    }
}

void LevelSystem::loadTextures() 
{
    sand = Resources::load<Texture>("sand2.png");
    house1 = Resources::load<Texture>("BlueHouse1.png");
    house2 = Resources::load<Texture>("BlueHouse2.png");
    house3 = Resources::load<Texture>("OrangeHouse.png");
    house4 = Resources::load<Texture>("PurpleHouse.png");
    house5 = Resources::load<Texture>("YellowHouse.png");
   // broken = Resources::load<Texture>("BrokenHouse.png");
    //broken2 = Resources::load<Texture>("brokenHouse2.png");
    wall = Resources::load<Texture>("GreyWall.png");
    houses = { house1, house2, house3, house4, house5 };
    _textures = { {EMPTY, sand }, {HOUSE, house1}, {WALL, wall} };

}

void LevelSystem::setTexture(LevelSystem::Tile t, shared_ptr<sf::Texture> tex)
{
    _textures[t] = tex;
}


unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;

float LevelSystem::_tileSize(100.f);
Vector2f LevelSystem::_offset(0.0f, 30.0f);
// Vector2f LevelSystem::_offset(0,0);

void LevelSystem::loadLevelFile(const string& path, float tileSize)
{
    _tileSize = tileSize;
    size_t w = 0, h = 0;
    string buffer;
    ls::loadTextures();
    //sand = Resources::load<Texture>("sand.png");

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

void LevelSystem::buildSprites()
{
    _sprites.clear();
    for (size_t y = 0; y < LevelSystem::getHeight(); ++y) 
    {
        for (size_t x = 0; x < LevelSystem::getWidth(); ++x) 
        {
            auto s = make_unique<sf::Sprite>();
            s->setTexture(*ls::getTexture(getTile({ x, y })));
            if (getTile({ x, y }) == HOUSE_R) {
                if (x == 0) {
                    s->setRotation(270.f);
                }
                else {
                    s->setRotation(90.f);
                }  
            }
            if (getTile({ x, y }) == HOUSE)
                if (y == ls::getHeight() - 1) {
                    s->setRotation(180.f);
            }
            s->setPosition(getTilePosition({ x,y }));
            s->setTextureRect(IntRect(0, 0, _tileSize, _tileSize));
            s->setOrigin(_tileSize/2, _tileSize/2);
            _sprites.push_back(move(s));
        }
    }
}

void LevelSystem::render(RenderWindow& window)
{
    for (auto& t : _sprites)
    {
        window.draw(*t);
    }
}

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

sf::Vector2f LevelSystem::getTilePosition(Vector2ul p)
{
    return (Vector2f(p.x, p.y) * _tileSize) + _offset;
}

vector<Vector2ul> LevelSystem::findTiles(LevelSystem::Tile type) 
{
    auto v = vector<sf::Vector2ul>();
    for (size_t i = 0; i < _width * _height; ++i) 
    {
        if (_tiles[i] == type) 
        {
            v.push_back({ i % _width, i / _width });
        }
    }

    return v;
}

LevelSystem::Tile LevelSystem::getTileAt(Vector2f v) 
{
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0) 
    {
        throw string("Tile out of range ");
    }
    return getTile(Vector2ul((v - _offset) / (_tileSize)));
}

bool LevelSystem::isOnGrid(sf::Vector2f v) 
{
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0)
    {
        return false;
    }
    auto p = Vector2ul((v - _offset) / (_tileSize));
    if (p.x > _width || p.y > _height) 
    {
        return false;
    }
    return true;
}

void LevelSystem::setOffset(const Vector2f& _offset) 
{
    LevelSystem::_offset = _offset + Vector2f(_tileSize/2, _tileSize/2);
    buildSprites();
}

void LevelSystem::unload() {
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

bool ls::isWall(Tile t) {
    return (t == HOUSE || t == HOUSE_R || t == WALL || t == BROKEN || t == BROKEN_R);
}