#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "worldmanager.h"

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
    GameObject(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name, bool offset, bool alpha, bool emplace, olc::PixelGameEngine* pge);
    virtual ~GameObject();
    virtual void Render();
};

class FilledRect : public GameObject
{
public:
    FilledRect(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name, bool offset, bool alpha, bool emplace, olc::PixelGameEngine* pge);
    virtual ~FilledRect() override;
    virtual void Render() override;
};

class Sprite : public GameObject
{
public:
    olc::Sprite* sprite;
    olc::GFX2D* gfx2d;
    Sprite(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, olc::Sprite* sprite, std::string name, olc::GFX2D* gfx2d, bool offset, bool alpha, bool emplace, olc::PixelGameEngine* pge);
    virtual ~Sprite() override;
    virtual void Render() override;
};


extern GameObject* FindGameObject(std::string& name);
extern void DeleteAllGameobjects();

extern std::list<GameObject*> Gameobjects[4];