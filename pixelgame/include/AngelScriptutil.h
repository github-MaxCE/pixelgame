#pragma once
#include "angelscript/angelscript.h"
#include "angelscript/scriptstdstring/scriptstdstring.h"
#include "angelscript/scriptbuilder/scriptbuilder.h"

namespace max::angelscript
{
    template<class... Args>
    inline int callfunc(const char* funcdecl, asIScriptModule* module, asIScriptEngine* engine, Args... args)
    {
        // Create our context, prepare it, and then execute
        asIScriptContext* ctx = engine->CreateContext();

        asIScriptFunction* func = module->GetFunctionByDecl(funcdecl);

        if (func != 0)
        {
            ctx->Prepare(func);
            
            if constexpr (sizeof...(args) > 0)
            {
                int num = 0;
                ((ctx->SetArgObject(num++, &args)), ...);
            }

            int r = ctx->Execute();
            if (r != asEContextState::asEXECUTION_FINISHED)
            {
                if (r == asEContextState::asEXECUTION_EXCEPTION)
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
    inline int callmethod(const char* funcdecl, asIScriptObject* obj, asIScriptEngine* engine, Args... args)
    {
        // Create our context, prepare it, and then execute
        asIScriptContext* ctx = engine->CreateContext();

        asIScriptFunction* func = obj->GetObjectType()->GetMethodByDecl(funcdecl);

        if (func != 0)
        {
            ctx->Prepare(func);

            if constexpr (sizeof...(args) > 0)
            {
                int num = 0;
                ((ctx->SetArgDWord(num++, args)), ...);
            }

            ctx->SetObject(obj);

            int r = ctx->Execute();
            if (r != asEContextState::asEXECUTION_FINISHED)
            {
                if (r == asEContextState::asEXECUTION_EXCEPTION)
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
    inline int callfactory(const char* objdecl, asITypeInfo* type, asIScriptObject* obj, asIScriptEngine* engine, Args... args)
    {
        // Create our context, prepare it, and then execute
        asIScriptContext *ctx = engine->CreateContext();

        // Get the object type
        asIScriptModule* module = engine->GetModule(objdecl);
        type = module->GetTypeInfoByDecl(objdecl);

        // Get the factory function from the object type
        asIScriptFunction* factory = type->GetFactoryByDecl((std::string() + type->GetName() + " @" + type->GetName() + "()").c_str());

        if(factory != 0)
        {
            // Prepare the context to call the factory function
            ctx->Prepare(factory);

            if constexpr (sizeof...(args) > 0)
            {
                int num = 0;
                ((ctx->SetArgDWord(num++, args)), ...);
            }

            // Execute the call
            ctx->Execute();

            // Get the object that was created
            obj = *(asIScriptObject**)ctx->GetAddressOfReturnValue();

            obj->AddRef();

            ctx->Release();
            return 1;
        }
    }

    inline void MessageCallback(const asSMessageInfo* msg, void* param)
    {
        const char* type = "ERR ";
        if (msg->type == asEMsgType::asMSGTYPE_WARNING)
            type = "WARN";
        else if (msg->type == asEMsgType::asMSGTYPE_INFORMATION)
            type = "INFO";
        printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
    }

    inline void print(const std::string& str)
    {
        printf("%s", str.c_str());
    }
}