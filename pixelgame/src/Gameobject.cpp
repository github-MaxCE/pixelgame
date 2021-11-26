#include "Gameobject.h"

std::list<Gameobject*> Gameobjects[4]
{
    std::list<Gameobject*>(),
    std::list<Gameobject*>(),
    std::list<Gameobject*>(),
    std::list<Gameobject*>()
};


/*			Constructors			*/

    /*          Gameobject          */
    Gameobject::~Gameobject()
    {
        this->Childs.clear();
    }

    /*          Rectobj         */
    Rectobj::Rectobj(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name)
    {
        std::string copy = name;
        int i = 0;
        while (FindGameObject(copy))
        {
            copy = name;
            i++;
            if (i != 0)
            {
                copy += (" (", i, ")");
            }
        }
        this->pos = pos;
        this->size = size;
        this->color = col;
        this->name = copy;
        Gameobjects[layer].emplace_back(this);
    }

    Rectobj::~Rectobj()
    {
        this->Childs.clear();
    }

    /*          FilledRectobj         */
    FilledRectobj::FilledRectobj(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name)
    {
        std::string copy = name;
        int i = 0;
        while (FindGameObject(copy))
        {
            copy = name;
            i++;
            if (i != 0)
            {
                copy += (" (", i, ")");
            }
        }
        this->pos = pos;
        this->size = size;
        this->color = col;
        this->name = copy;
        Gameobjects[layer].emplace_back(this);
    }

    FilledRectobj::~FilledRectobj()
    {
        this->Childs.clear();
    }

    /*          Spriteobj         */
    Spriteobj::Spriteobj(int layer, olc::vi2d pos, olc::vi2d size, bool alpha, olc::Sprite* sprite, std::string name)
    {
        std::string copy = name;
        int i = 0;
        while (FindGameObject(copy))
        {
            copy = name;
            i++;
            if (i != 0)
            {
                copy += (" (", i, ")");
            }
        }
        this->size = size;
        this->pos = pos;
        this->alpha = alpha;
        this->sprite = sprite;
        this->name = copy;
        Gameobjects[layer].emplace_back(this);
    }

    Spriteobj::~Spriteobj()
    {
        this->Childs.clear();
    }

/*			Drawing			*/
void Rectobj::Render(olc::PixelGameEngine* pge)
{
    pge->DrawRect(this->pos, this->size, this->color);
}

void FilledRectobj::Render(olc::PixelGameEngine* pge)
{
    pge->FillRect(this->pos, this->size, this->color);
}

void Spriteobj::Render(olc::PixelGameEngine* pge)
{
    pge->DrawSprite(this->pos, this->sprite);
}

void Gameobject::AddChild(Gameobject *child)
{
    this->Childs.emplace_back(child);
}

Gameobject* Gameobject::FindChild(std::string name)
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

Gameobject* FindGameObject(std::string name)
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