#pragma once
#include <list>
#include "olcPixelGameEngine.h"
#include "Gameobject.h"
#include "worldmanager.h"

interface Entity;

extern std::list<Entity*> Entities;

interface Entity
{
public:
    Entity(bool emplace = true)
    {
        if (emplace == true) Entities.emplace_back(this);
    }

    virtual void Start() {}
    virtual void FixedUpdate(float fElapsedTime) {}
    virtual void Update(float fElapsedTime) {}
    virtual void End() {}
};
