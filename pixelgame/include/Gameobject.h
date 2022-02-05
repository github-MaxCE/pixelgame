#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Player.h"
namespace max
{
    class map;

    class GameObject
    {
    protected:
        olc::PixelGameEngine* pge;
    private:
        max::map* world;
    public:
        olc::vi2d size, pos;
        std::string name;
        olc::Pixel col;
        bool offset;
        bool alpha;
        GameObject(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name, bool offset, bool alpha, bool emplace, olc::PixelGameEngine* pge);
        virtual ~GameObject();
        virtual void Render(max::Camera* camera);
    };

    class FilledRect : public max::GameObject
    {
    public:
        FilledRect(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name, bool offset, bool alpha, bool emplace, olc::PixelGameEngine* pge);
        virtual ~FilledRect() override;
        virtual void Render(max::Camera* camera) override;
    };

    class Sprite : public max::GameObject
    {
    public:
        olc::Sprite* sprite;
        olc::GFX2D* gfx2d;
        Sprite(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, olc::Sprite* sprite, std::string name, olc::GFX2D* gfx2d, bool offset, bool alpha, bool emplace, olc::PixelGameEngine* pge);
        virtual ~Sprite() override;
        virtual void Render(max::Camera* camera) override;
    };
}
