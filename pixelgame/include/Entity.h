#pragma once
#include <list>
#include "olcPixelGameEngine.h"
#include "Gameobject.h"
#include "worldmanager.h"

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

    namespace angelscript
    {
        class asEntity : public max::Entity
        {
        private:
            asIScriptEngine* engine;
            CScriptBuilder* builder;
            asIScriptContext* ctx;
            asIScriptObject* obj;
            asITypeInfo* type;

        public:
            asEntity(const char* file, const char* objdecl, asIScriptEngine* _engine, CScriptBuilder* _builder, asIScriptContext* _ctx, bool emplace = true);
            
            virtual void Start() override;
            virtual void FixedUpdate(float fElapsedTime) override;
            virtual void Update(float fElapsedTime) override;
            virtual void End() override;
        };
    }
}

extern std::list<max::Entity*> Entities;
