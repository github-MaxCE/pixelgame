#pragma once
#include <list>
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class GameObject
{
public:
    olc::PixelGameEngine* pge;
    olc::vi2d size;
    std::string name;
    olc::vi2d pos;
    olc::Pixel col;
    bool offset;
    bool alpha;
    GameObject(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name, bool offset, bool alpha, olc::PixelGameEngine* pge);
    virtual ~GameObject();
    virtual void Render();
};

class FilledRect : virtual public GameObject
{
public:
    FilledRect(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name, bool offset, bool alpha, olc::PixelGameEngine* pge);
    ~FilledRect();
    virtual void Render();
};

class Sprite : virtual public GameObject
{
public:
    olc::Sprite* sprite;
    olc::GFX2D* gfx2d;
    Sprite(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, olc::Sprite* sprite, std::string name, olc::GFX2D* gfx2d, bool offset, bool alpha, olc::PixelGameEngine* pge);
    ~Sprite();
    virtual void Render();
};

extern GameObject* FindGameObject(std::string name);
extern void DeleteAllGameobjects();
extern std::list<GameObject*> Gameobjects[4];

extern olc::vi2d worldoffset;
extern olc::vi2d worldsize;