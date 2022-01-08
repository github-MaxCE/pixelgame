#pragma once
#include <functional>
#include "Entity.h"

class asEntity : public Entity
{
public:
    asEntity(const char* file, const char* objdecl, asIScriptEngine* _engine, CScriptBuilder* _builder, asIScriptObject* _asObject, asIScriptContext* _ctx, bool emplace = true) :
        engine(_engine),
        builder(_builder),
        asObject(_asObject),
        ctx(_ctx)
    {
        int r = builder->StartNewModule(engine, objdecl);
        if (r < 0) printf("Unrecoverable error while starting a new module.\n");
        // Get the object type

        r = builder->AddSectionFromFile((scriptpath() + file).c_str());
        if (r < 0) printf("Please correct the errors in the script and try again.\n");

        r = builder->BuildModule();
        if (r < 0) printf("Please correct the errors in the script and try again.\n");

        // Get the object type
        asIScriptModule* module = engine->GetModule(objdecl);
        asITypeInfo* type = module->GetTypeInfoByDecl(objdecl);
        if (type != 0)
        {
            // Get the factory function from the object type
            asIScriptFunction* factory = type->GetFactoryByDecl((std::string(objdecl) + " @" + objdecl + "()").c_str());


            ctx = engine->CreateContext();

            // Prepare the context to call the factory function
            ctx->Prepare(factory);

            // Execute the call
            ctx->Execute();

            // Get the object that was created
            asObject = *(asIScriptObject**)ctx->GetAddressOfReturnValue();

            // If you're going to store the object you must increase the reference,
            // otherwise it will be destroyed when the context is reused or destroyed.
            asObject->AddRef();

            ctx->Release();
        }
        if (emplace == true) Entities.emplace_back(this);
    }

    virtual void Start() override
    {
        if (asObject != nullptr)
        {
            asIScriptFunction* func = asObject->GetObjectType()->GetMethodByDecl("void Start()");
            if (func != 0)
            {
                // Create our context, prepare it, and then execute
                ctx = engine->CreateContext();

                ctx->Prepare(func);

                ctx->SetObject(asObject);

                int r = ctx->Execute();
                if (r != asEXECUTION_FINISHED)
                {
                    if (r == asEXECUTION_EXCEPTION)
                    {
                        printf("An exception '%s' occurred. Please correct the code and try again.\n", ctx->GetExceptionString());
                    }
                }
                // Clean up
                ctx->Release();
            }
        }
    }
    virtual void FixedUpdate(float fElapsedTime) override
    {
        if (asObject != nullptr)
        {
            int r;
            asIScriptFunction* func = asObject->GetObjectType()->GetMethodByDecl("void FixedUpdate()");
            if (func != 0)
            {
                // Create our context, prepare it, and then execute
                ctx = engine->CreateContext();

                ctx->Prepare(func);

                ctx->SetObject(asObject);

                int r = ctx->Execute();
                if (r != asEXECUTION_FINISHED)
                {
                    if (r == asEXECUTION_EXCEPTION)
                    {
                        printf("An exception '%s' occurred. Please correct the code and try again.\n", ctx->GetExceptionString());
                    }
                }
                // Clean up
                ctx->Release();
            }
        }
    }
    virtual void Update(float fElapsedTime) override
    {
        if (asObject != nullptr)
        {
            int r;
            asIScriptFunction* func = asObject->GetObjectType()->GetMethodByDecl("void Update()");
            if (func != 0)
            {
                // Create our context, prepare it, and then execute
                ctx = engine->CreateContext();

                ctx->Prepare(func);

                ctx->SetObject(asObject);

                r = ctx->Execute();
                if (r != asEXECUTION_FINISHED)
                {
                    if (r == asEXECUTION_EXCEPTION)
                    {
                        printf("An exception '%s' occurred. Please correct the code and try again.\n", ctx->GetExceptionString());
                    }
                }
                // Clean up
                ctx->Release();
            }
        }
    }
    virtual void End() override
    {
        if (asObject != nullptr)
        {
            int r;
            asIScriptFunction* func = asObject->GetObjectType()->GetMethodByDecl("void End()");
            if (func != 0)
            {
                // Create our context, prepare it, and then execute
                ctx = engine->CreateContext();

                ctx->Prepare(func);

                ctx->SetObject(asObject);

                r = ctx->Execute();
                if (r != asEXECUTION_FINISHED)
                {
                    if (r == asEXECUTION_EXCEPTION)
                    {
                        printf("An exception '%s' occurred. Please correct the code and try again.\n", ctx->GetExceptionString());
                    }
                }
                // Clean up
                ctx->Release();
            }
        }
    }

private:
    asIScriptEngine* engine;
    CScriptBuilder* builder;
    asIScriptObject* asObject;
    asIScriptContext* ctx;
};