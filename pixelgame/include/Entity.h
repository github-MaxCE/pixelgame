#pragma once
#include <list>
#include "olcPixelGameEngine.h"
#include "Gameobject.h"
#include "worldmanager.h"
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
}

extern std::list<max::Entity*> Entities;


namespace max::angelscript
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
        template<class... Args>
        inline asEntity(const char* file, const char* objdecl, asIScriptEngine* _engine, CScriptBuilder* _builder, asIScriptContext* _ctx, bool emplace = true, Args... args) :
            engine(_engine),
            builder(_builder),
            ctx(_ctx),
            max::Entity(emplace)
        {
            if (engine->GetModule(objdecl) == 0)
            {
                int r = builder->StartNewModule(engine, objdecl);
                if (r < 0) printf("Unrecoverable error while starting a new module.\n");
                // Get the object type

                r = builder->AddSectionFromFile((scriptpath() + file).c_str());
                if (r < 0) printf("Please correct the errors in the script and try again.\n");

                r = builder->BuildModule();
                if (r < 0) printf("Please correct the errors in the script and try again.\n");
            }

            max::angelscript::callfactory(objdecl, type, obj, engine, args...);

            if (emplace == true) Entities.emplace_back(this);
        }

        virtual void Start() override;
        virtual void FixedUpdate(float fElapsedTime) override;
        virtual void Update(float fElapsedTime) override;
        virtual void End() override;
    };
}