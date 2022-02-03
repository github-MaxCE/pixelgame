#include "Entity.h"
namespace max
{
    std::list<max::Entity*> Entities = std::list<max::Entity*>();

    Entity::Entity(bool emplace)
    {
        if (emplace == true) Entities.emplace_back(this);
    }
}