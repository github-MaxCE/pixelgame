#include "AngelScriptutil.h"

namespace max::angelscript
{
    template<class... Args>
    int callfunc(asIScriptFunction* func, asIScriptEngine* engine, asIScriptContext* ctx, Args... args)
    {
        if (func != 0)
        {
            // Create our context, prepare it, and then execute
            ctx = engine->CreateContext();

            ctx->Prepare(func);

            if constexpr (sizeof...(args) != 0)
            {
                int num = 0;
                ((ctx->SetArgDWord(num++, args)), ...);
            }

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
            return 1;
        }
        return 0;
    }

    template<class... Args>
    int callmethod(asIScriptFunction* func, asIScriptObject* obj, asIScriptEngine* engine, asIScriptContext* ctx, Args... args)
    {
        if (func != 0)
        {
            // Create our context, prepare it, and then execute
            ctx = engine->CreateContext();

            ctx->Prepare(func);

            ctx->SetObject(obj);

            if constexpr (sizeof...(args) != 0)
            {
                int num = 0;
                ((ctx->SetArgDWord(num++, args)), ...);
            }

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
            return 1;
        }
        return 0;
    }

    template<class... Args>
    int callfactory(const char* objdecl, asITypeInfo* type, asIScriptObject* obj, asIScriptEngine* engine, asIScriptContext* ctx, Args... args)
    {
        // Get the object type
        asIScriptModule* module = engine->GetModule(objdecl);
        type = module->GetTypeInfoByDecl(objdecl);

        // Get the factory function from the object type
        asIScriptFunction* factory = type->GetFactoryByDecl((std::string() + type->GetName() + " @" + type->GetName() + "()").c_str());

        ctx = engine->CreateContext();

        // Prepare the context to call the factory function
        ctx->Prepare(factory);

        if constexpr (sizeof...(args) != 0)
        {
            int num = 0;
            ((ctx->SetArgDWord(num++, args)), ...);
        }

        // Execute the call
        ctx->Execute();

        // Get the object that was created
        obj = *(asIScriptObject**)ctx->GetAddressOfReturnValue();

        // If you're going to store the object you must increase the reference,
        // otherwise it will be destroyed when the context is reused or destroyed.
        obj->AddRef();

        ctx->Release();
        return 1;
    }

    void MessageCallback(const asSMessageInfo* msg, void* param)
    {
        const char* type = "ERR ";
        if (msg->type == asMSGTYPE_WARNING)
            type = "WARN";
        else if (msg->type == asMSGTYPE_INFORMATION)
            type = "INFO";
        printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
    }

    void max::angelscript::print(const std::string& str)
    {
        printf("%s", str.c_str());
    }
}