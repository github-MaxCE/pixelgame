#pragma once
#include <list>
#include <utility>
#include "filemanager.h"
#include "angelscript/angelscript.h"
#include "angelscript/scriptstdstring/scriptstdstring.h"
#include "angelscript/scriptbuilder/scriptbuilder.h"
#include "Entity.h"
#include "ScriptObject.h"

namespace max::script
{
    class Entity : public max::Entity, public object
    {
    public:
        template<class... Args>
        inline Entity(const char* typedecl, const char* filename, bool emplace = true, Args&& ...args) :
            max::Entity(emplace),
            object(typedecl, filename, std::forward<Args>(args)...)
        {}

        virtual ~Entity() = default;
    
        virtual void Start() override;
        virtual void FixedUpdate(float fElapsedTime) override;
        virtual void Update(float fElapsedTime) override;
        virtual void End() override;
    };
}