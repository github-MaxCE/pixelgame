#pragma once
#include <list>
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class Rectobj
{
public:
    int layer;
    olc::vi2d size;
    std::string name;
    olc::vi2d pos;
    olc::Pixel col;
    Rectobj* Parent;
    std::list<Rectobj*> Childs;
    bool offset;
    bool alpha;
    Rectobj(int _layer, olc::vi2d _pos, olc::vi2d _size, olc::Pixel _col, std::string _name, bool _offset, bool _alpha, bool a = false);
    virtual ~Rectobj();
    void AddChild(Rectobj* child);
    Rectobj* FindChild(std::string name);
    virtual void Render(olc::PixelGameEngine* pge);
};

class FilledRectobj : public Rectobj
{
public:
    FilledRectobj(int _layer, olc::vi2d _pos, olc::vi2d _size, olc::Pixel _col, std::string _name, bool _offset, bool _alpha);
    ~FilledRectobj();
    virtual void Render(olc::PixelGameEngine* pge);
};

class Spriteobj : public Rectobj
{
public:
    olc::Sprite* sprite;
    olc::GFX2D* gfx2d;
    Spriteobj(int _layer, olc::vi2d _pos, olc::vi2d _size, olc::Pixel _col, olc::Sprite* _sprite, std::string _name, olc::GFX2D* _gfx2d, bool _offset, bool _alpha);
    ~Spriteobj();
    virtual void Render(olc::PixelGameEngine* pge);
};

extern Rectobj* FindGameObject(std::string name);
extern void DeleteAllGameobjects();
extern std::list<Rectobj*> Gameobjects[4];

extern olc::vi2d worldoffset;
extern olc::vi2d worldsize;