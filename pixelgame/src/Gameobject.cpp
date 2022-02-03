#include "GameObject.h"

std::list<max::GameObject*> GameObjects[4]
{
    std::list<max::GameObject*>(),
    std::list<max::GameObject*>(),
    std::list<max::GameObject*>(),
    std::list<max::GameObject*>()
};

/*			Constructors			*/

    /*          max::GameObject         */
    max::GameObject::GameObject(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name, bool offset, bool alpha, bool emplace, olc::PixelGameEngine* pge) :
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
                copy += (" (", i, ")");
            }
        }
        this->name = copy;
        this->pge = pge;
        if(emplace == true) GameObjects[layer].emplace_back(this);
    }

    max::GameObject::~GameObject()
    {
        
    }

    /*          max::FilledRect         */
    max::FilledRect::FilledRect(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name, bool offset, bool alpha, bool emplace, olc::PixelGameEngine* pge) :
        max::GameObject(layer, pos, size, col, name, offset, alpha, emplace, pge)
    {
        this->pge = pge;
    }

    max::FilledRect::~FilledRect()
    {
        
    }

    /*          Sprite         */
    max::Sprite::Sprite(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, olc::Sprite* sprite, std::string name, olc::GFX2D* gfx2d, bool offset, bool alpha, bool emplace, olc::PixelGameEngine* pge) :
        max::GameObject(layer, pos, size, col, name, offset, alpha, emplace, pge),
        sprite(sprite),
        gfx2d(gfx2d)
    {
        this->pge = pge;
    }

    max::Sprite::~Sprite()
    {
        
    }

    /*			Drawing			*/
void max::GameObject::Render(max::Camera* camera)
{
    olc::vi2d pos = offset ? this->pos + camera->worldoffset : this->pos;
    pge->DrawRect(pos, this->size, this->col);
}

void max::FilledRect::Render(max::Camera* camera)
{
    olc::vi2d pos = offset ? this->pos + camera->worldoffset : this->pos;
    pge->FillRect(pos, this->size+1, this->col);
}

void max::Sprite::Render(max::Camera* camera)
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
    pge->FillRect(pos, this->size + 1, this->col);

    pge->SetPixelMode(olc::Pixel::NORMAL);
}