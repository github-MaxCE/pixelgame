#pragma once
#include <vector>

namespace max
{
    class Entity
    {
    public:
        Entity(bool emplace = true);

        virtual void Start() = 0;
        virtual void FixedUpdate(float fElapsedTime) = 0;
        virtual void Update(float fElapsedTime) = 0;
        virtual void End() = 0;
    };

    extern std::vector<max::Entity*> Entities;

    extern void DeleteAllEntities();
}