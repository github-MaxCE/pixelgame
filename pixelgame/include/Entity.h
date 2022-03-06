#pragma once
#include <list>
#include "filemanager.h"
#include "angelscript/angelscript.h"
#include "angelscript/scriptstdstring/scriptstdstring.h"
#include "angelscript/scriptbuilder/scriptbuilder.h"

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

    extern std::list<max::Entity*> Entities;

    extern void DeleteAllEntities();
}