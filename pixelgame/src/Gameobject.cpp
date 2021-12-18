#include "Gameobject.h"

olc::vi2d worldoffset;
olc::vi2d worldsize;

std::list<Rectobj*> Gameobjects[4]
{
    std::list<Rectobj*>(),
    std::list<Rectobj*>(),
    std::list<Rectobj*>(),
    std::list<Rectobj*>()
};

/*			Constructors			*/

    /*          Rectobj         */
Rectobj::Rectobj(int _layer, olc::vi2d _pos, olc::vi2d _size, olc::Pixel _col, std::string _name, bool _offset, bool _alpha, bool a) :
        pos(_pos),
        size(_size),
        offset(_offset),
        col(_col)
    {
        // bool a is to see if the rect obj is an obj or a base constructor. i'm not sure why i'm doing this i'm just making sure something isn't obvious
        std::string copy = _name;
        int i = 0;
        while (FindGameObject(copy) != nullptr)
        {
            copy = _name;
            i++;
            if (i != 0)
            {
                copy += (" (", i, ")");
            }
        }
        this->name = copy;
        if (a)
        {
            Gameobjects[layer].emplace_back(this);
        }
    }

    Rectobj::~Rectobj()
    {
        this->Childs.clear();
    }

    /*          FilledRectobj         */
    FilledRectobj::FilledRectobj(int _layer, olc::vi2d _pos, olc::vi2d _size, olc::Pixel _col, std::string _name, bool _offset, bool _alpha) :
        Rectobj(_layer, _pos, _size, _col, _name, _offset, _alpha)
    {
        Gameobjects[layer].emplace_back(this);
    }

    FilledRectobj::~FilledRectobj()
    {
        this->Childs.clear();
    }

    /*          Spriteobj         */
    Spriteobj::Spriteobj(int _layer, olc::vi2d _pos, olc::vi2d _size, olc::Pixel _col, olc::Sprite* _sprite, std::string _name, olc::GFX2D* _gfx2d, bool _offset, bool _alpha) :
        Rectobj(_layer, _pos, _size, _col, _name, _offset, _alpha),
        sprite(_sprite),
        gfx2d(_gfx2d)
    {
        Gameobjects[layer].emplace_back(this);
    }

    Spriteobj::~Spriteobj()
    {
        this->Childs.clear();
    }

/*			Drawing			*/
void Rectobj::Render(olc::PixelGameEngine* pge)
{
    olc::vi2d pos = offset ? this->pos + worldoffset : this->pos;
    pge->DrawRect(pos, this->size, this->col);
}

void FilledRectobj::Render(olc::PixelGameEngine* pge)
{
    olc::vi2d pos = offset ? this->pos + worldoffset : this->pos;
    pge->FillRect(pos, this->size+1, this->col);
}

void Spriteobj::Render(olc::PixelGameEngine* pge)
{
    olc::vi2d pos = offset ? this->pos + worldoffset : this->pos;
    // Draw Sprite using extension, first create a transformation stack
    olc::GFX2D::Transform2D t;

    // Scale the sprite
    t.Scale((float(size.x) / float(sprite->width-1)), float(size.y) / float(sprite->height));
    // Translate to 0,100
    t.Translate(pos.x, pos.y);

    if (alpha) pge->SetPixelMode(olc::Pixel::ALPHA);
    // Use extension to draw sprite with transform applied
    gfx2d->DrawSprite(sprite, t);
    pge->FillRect(pos, this->size + 1, this->col);

    pge->SetPixelMode(olc::Pixel::NORMAL);
}

void Rectobj::AddChild(Rectobj *child)
{
    this->Childs.emplace_back(child);
}

Rectobj* Rectobj::FindChild(std::string name)
{
    if (this->name == name)
    {
        return this;
    }
    else if(!this->Childs.empty())
    {
        for (auto const& a : this->Childs)
        {
            return a->FindChild(name);
        }
    }
    return nullptr;
}

Rectobj* FindGameObject(std::string name)
{
    for(auto a : Gameobjects)
    {
        if (!a.empty())
        {
            for (auto x : a)
            {
                return x->FindChild(name);
            }
        }
    }
    return nullptr;
}

void DeleteAllGameobjects()
{
    for (auto i : Gameobjects)
    {
        i.clear();
    }
}