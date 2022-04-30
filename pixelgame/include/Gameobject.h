#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

namespace max
{
    class Camera;
    class map;

    class GameObject
    {
    protected:
        olc::PixelGameEngine* pge;
    public:
        olc::vu2d size;
        olc::vi2d pos;
        std::string name;
        olc::Pixel col;
        bool offset;
        bool alpha;
        GameObject(olc::vi2d, olc::vu2d, olc::Pixel, const char*, bool, bool, olc::PixelGameEngine*, max::map*);
        virtual ~GameObject();
        virtual void Render(max::Camera*);
    };

    class FilledRect : public max::GameObject
    {
    public:
        FilledRect(olc::vi2d, olc::vu2d, olc::Pixel, const char*, bool, bool, olc::PixelGameEngine*, max::map*);
        virtual ~FilledRect();
        virtual void Render(max::Camera*) override;
    };

    class Sprite : public max::GameObject
    {
    public:
        olc::Sprite* sprite;
        olc::GFX2D* gfx2d;
        Sprite(olc::vi2d, olc::vu2d, olc::Pixel, olc::Sprite*, const char*, olc::GFX2D*, bool, bool, olc::PixelGameEngine*, max::map*);
        virtual ~Sprite();
        virtual void Render(max::Camera*) override;
    };
}
