#include "Gameobject.h"

olc::vi2d worldoffset;
olc::vi2d worldsize;

std::list<GameObject*> Gameobjects[4]
{
    std::list<GameObject*>(),
    std::list<GameObject*>(),
    std::list<GameObject*>(),
    std::list<GameObject*>()
};

/*			Constructors			*/

    /*          GameObject         */
    GameObject::GameObject(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name, bool offset, bool alpha, olc::PixelGameEngine* pge) :
        pos(pos),
        size(size),
        offset(offset),
        col(col)
    {
        std::string copy = name;
        int i = 0;
        while (FindGameObject(copy) != nullptr)
        {
            copy = name;
            i++;
            if (i != 0)
            {
                copy += (" (", i, ")");
            }
        }
        this->name = copy;
        Gameobjects[layer].emplace_back(this);
        this->pge = pge;
    }

    GameObject::~GameObject()
    {
        
    }

    /*          FilledRect         */
    FilledRect::FilledRect(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name, bool offset, bool alpha, olc::PixelGameEngine* pge) :
        GameObject(layer, pos, size, col, name, offset, alpha, pge)
    {
        this->pge = pge;
    }

    FilledRect::~FilledRect()
    {
        
    }

    /*          Sprite         */
    Sprite::Sprite(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, olc::Sprite* sprite, std::string name, olc::GFX2D* gfx2d, bool offset, bool alpha, olc::PixelGameEngine* pge) :
        GameObject(layer, pos, size, col, name, offset, alpha, pge),
        sprite(sprite),
        gfx2d(gfx2d)
    {
        this->pge = pge;
    }

    Sprite::~Sprite()
    {
        
    }

    /*			Drawing			*/
void GameObject::Render()
{
    olc::vi2d pos = offset ? this->pos + worldoffset : this->pos;
    pge->DrawRect(pos, this->size, this->col);
}

void FilledRect::Render()
{
    olc::vi2d pos = offset ? this->pos + worldoffset : this->pos;
    pge->FillRect(pos, this->size+1, this->col);
}

void Sprite::Render()
{
    // Draw Sprite using extension, first create a transformation stack
    olc::GFX2D::Transform2D t;

    // Scale the sprite
    t.Scale((size.x / sprite->width-1), size.y / sprite->height);
    // Translate to 0,100
    t.Translate(pos.x, pos.y);

    if (alpha) pge->SetPixelMode(olc::Pixel::ALPHA);
    // Use extension to draw sprite with transform applied
    gfx2d->DrawSprite(sprite, t);
    pge->FillRect(pos, this->size + 1, this->col);

    pge->SetPixelMode(olc::Pixel::NORMAL);
}

GameObject* FindGameObject(std::string name)
{
    for(auto a : Gameobjects)
    {
        for (auto x : a)
        {
            if (x->name == name) return x;
        }
    }
    return nullptr;
}

void DeleteAllGameobjects()
{
    for (auto i : Gameobjects)
    {
        for (auto x : i)
        {
            delete x;
        }
        i.clear();
    }
}