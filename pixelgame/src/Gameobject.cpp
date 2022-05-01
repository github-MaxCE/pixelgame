#include "GameObject.h"
#include "Player.h"
#include "map.h"
#include "mathutils.h"

namespace max
{
    GameObject::GameObject(olc::vi2d pos, olc::vu2d size, olc::Pixel col, const char* name, bool offset, bool alpha, olc::PixelGameEngine* pge, map* world) :
        pos(pos),
        size(size),
        offset(offset),
        col(col)
    {
        std::string copy = name;
        int i = 0;
        while (world->FindGameObject(copy) != nullptr)
        {
            copy = name;
            i++;
            if (i != 0)
            {
                copy += " (" + std::to_string(i) + ")";
            }
        }
        this->name = copy;
        this->pge = pge;
    }

    GameObject::~GameObject()
    {}

    FilledRect::FilledRect(olc::vi2d pos, olc::vu2d size, olc::Pixel col, const char* name, bool offset, bool alpha, olc::PixelGameEngine* pge, map* world) :
        GameObject(pos, size, col, name, offset, alpha, pge, world)
    {}

    FilledRect::~FilledRect()
    {}

    Sprite::Sprite(olc::vi2d pos, olc::vu2d size, olc::Pixel col, olc::Sprite* sprite, const char* name, olc::GFX2D* gfx2d, bool offset, bool alpha, olc::PixelGameEngine* pge, map* world) :
        GameObject(pos, size, col, name, offset, alpha, pge, world),
        sprite(sprite),
        gfx2d(gfx2d)
    {}

    Sprite::~Sprite()
    {}

    void GameObject::Render(Camera* camera)
    {
        olc::vi2d npos = offset ? pos + camera->worldoffset : pos;
        pge->DrawRect(npos, size, this->col);
    }

    void FilledRect::Render(Camera* camera)
    {
        olc::vi2d npos = offset ? pos + camera->worldoffset : pos;
        pge->FillRect(npos, size + 1, col);
    }

    void Sprite::Render(Camera* camera)
    {
        // Draw Sprite using extension, first create a transformation stack
        olc::GFX2D::Transform2D t;

        // Scale the sprite
        t.Scale(size.x / (sprite->width - 1), size.y / (sprite->height));
        // Translate to 0,100
        t.Translate(pos.x, pos.y);

        if (alpha) pge->SetPixelMode(olc::Pixel::ALPHA);
        // Use extension to draw sprite with transform applied
        gfx2d->DrawSprite(sprite, t);
        pge->FillRect(pos, size + 1, col);

        pge->SetPixelMode(olc::Pixel::NORMAL);
    }
}
