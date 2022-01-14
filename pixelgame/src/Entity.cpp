#include "Entity.h"

std::list<max::Entity*> Entities = std::list<max::Entity*>();

namespace max
{
    Entity::Entity(bool emplace)
    {
        if (emplace == true) Entities.emplace_back(this);
    }

    namespace angelscript
    {
        asEntity::asEntity(const char* file, const char* objdecl, asIScriptEngine* _engine, CScriptBuilder* _builder, asIScriptContext* _ctx, bool emplace) :
            engine(_engine),
            builder(_builder),
            ctx(_ctx),
            max::Entity(emplace)
        {
            int r = builder->StartNewModule(engine, objdecl);
            if (r < 0) printf("Unrecoverable error while starting a new module.\n");
            // Get the object type

            r = builder->AddSectionFromFile((scriptpath() + file).c_str());
            if (r < 0) printf("Please correct the errors in the script and try again.\n");

            r = builder->BuildModule();
            if (r < 0) printf("Please correct the errors in the script and try again.\n");

            max::angelscript::callfactory(objdecl, type, obj, engine, ctx);
        }

        void asEntity::Start()
        {
            if (obj != nullptr)
            {
                asIScriptFunction* func = type->GetMethodByDecl("void Start()");
                max::angelscript::callmethod(func, obj, engine, ctx);
            }
        }

        void asEntity::FixedUpdate(float fElapsedTime)
        {
            if (obj != nullptr)
            {
                asIScriptFunction* func = type->GetMethodByDecl("void FixedUpdate(float fElapsedTime)");
                max::angelscript::callmethod(func, obj, engine, ctx, fElapsedTime);
            }
        }

        void asEntity::Update(float fElapsedTime)
        {
            if (obj != nullptr)
            {
                asIScriptFunction* func = type->GetMethodByDecl("void Update(float fElapsedTime)");
                max::angelscript::callmethod(func, obj, engine, ctx, fElapsedTime);
            }
        }

        void asEntity::End()
        {
            if (obj != nullptr)
            {
                asIScriptFunction* func = type->GetMethodByDecl("void End()");
                max::angelscript::callmethod(func, obj, engine, ctx);
                engine->ReleaseScriptObject(obj, type);
            }
        }
    }
}