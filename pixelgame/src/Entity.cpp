#include "Entity.h"

std::list<max::Entity*> Entities = std::list<max::Entity*>();

max::Entity::Entity(bool emplace)
{
    if (emplace == true) Entities.emplace_back(this);
}