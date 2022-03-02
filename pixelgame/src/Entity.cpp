#include "Entity.h"

namespace max
{
    Entity::Entity(bool emplace)
    {
        if (emplace == true) Entities.emplace_back(this);
    }

    std::list<max::Entity*> Entities;

    void DeleteAllEntities()
    {
        if (!Entities.empty())
        {
            for (auto& x : Entities)
            {
                delete x;
            }
            Entities.clear();
        }
    }
}