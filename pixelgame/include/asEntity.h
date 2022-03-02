#pragma once
#include <list>
#include "filemanager.h"
#include "AngelScriptUtils.h"
#include "angelscript/angelscript.h"
#include "angelscript/scriptstdstring/scriptstdstring.h"
#include "angelscript/scriptbuilder/scriptbuilder.h"
#include "Entity.h"

namespace max::angelscript
{
    class Entity : public max::Entity
    {
    private:
        asIScriptEngine* engine;
        CScriptBuilder* builder;
        asIScriptModule* mod;
        asITypeInfo* type;
        asIScriptObject* obj;
    
    public:
        template<class... Args>
        inline Entity(const char* filename, const char* objdecl, asIScriptEngine* engine, CScriptBuilder* builder, bool emplace = true, Args... args) :
            engine(engine),
            builder(builder),
            max::Entity(emplace)
        {
            int r = builder->StartNewModule(engine, objdecl);
            if (r < 0) printf("Unrecoverable error while starting a new module.\n");
            // Get the object type
    
            r = builder->AddSectionFromFile((scriptpath() + filename).c_str());
            if (r < 0) printf("Please correct the errors in the script and try again.\n");
    
            r = builder->BuildModule();
            if (r < 0) printf("Please correct the errors in the script and try again.\n");

            mod = max::angelscript::BuildScript(filename, builder, engine);
            type = max::angelscript::GetType(mod, objdecl);
            obj = max::angelscript::callfactory(objdecl, type, engine, args...);
        }
    
        ~Entity();
    
        virtual void Start() override;
        virtual void FixedUpdate(float fElapsedTime) override;
        virtual void Update(float fElapsedTime) override;
        virtual void End() override;
    };
}