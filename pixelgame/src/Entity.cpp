#include "Entity.h"

namespace max
{
    Entity::Entity(bool emplace)
    {
        if (emplace == true) Entities.emplace_back(this);
    }

    std::list<max::Entity*> Entities;
}