#pragma once
#include "angelscript/angelscript.h"
#include "angelscript/scriptstdstring/scriptstdstring.h"
#include "angelscript/scriptbuilder/scriptbuilder.h"
#include "filemanager.h"
#define _SetArg(Type, val) ctx->SetArg##Type(num, val)

namespace max::angelscript
{
    template<class T>
    inline int SetArg(asIScriptContext* ctx, unsigned int num, T value)
    {
        return _SetArg(Object, &value);
    }

    template<>
    inline int SetArg<void*>(asIScriptContext* ctx, unsigned int num, void* value)
    {
        return _SetArg(Address, value);
    }

    template<>
    inline int SetArg<asDWORD>(asIScriptContext * ctx, unsigned int num, asDWORD value)
    {
        return _SetArg(DWord, value);
    }

    template<>
    inline int SetArg<asQWORD>(asIScriptContext * ctx, unsigned int num, asQWORD value)
    {
        return _SetArg(QWord, value);
    }

    template<>
    inline int SetArg<float>(asIScriptContext * ctx, unsigned int num, float value)
    {
        return _SetArg(Float, value);
    }

    template<>
    inline int SetArg<double>(asIScriptContext * ctx, unsigned int num, double value)
    {
        return _SetArg(Double, value);
    }

    template<>
    inline int SetArg<asBYTE>(asIScriptContext * ctx, unsigned int num, asBYTE value)
    {
        return _SetArg(Byte, value);
    }

    template<>
    inline int SetArg<asWORD>(asIScriptContext * ctx, unsigned int num, asWORD value)
    {
        return _SetArg(Word, value);
    }

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
                ((max::angelscript::SetArg(ctx, num++, args)), ...);
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

    extern asIScriptModule* BuildScript(const char* filename, CScriptBuilder* builder, asIScriptEngine* engine);

    extern asITypeInfo* GetType(asIScriptModule* mod, const char* objdecl);

    template<class... Args>
    inline asIScriptObject* callfactory(const char* objdecl, asITypeInfo* type, asIScriptEngine* engine, Args... args)
    {
        asIScriptContext* ctx = engine->CreateContext();

        // Get the factory function from the object type
        asIScriptFunction* factory = type->GetFactoryByDecl((std::string() + objdecl + "@ " + objdecl + "()").c_str());

        if(factory != 0)
        {
            // Prepare the context to call the factory function
            ctx->Prepare(factory);

            if constexpr (sizeof...(args) > 0)
            {
                int num = 0;
                ((max::angelscript::SetArg(ctx, num++, args)), ...);
            }

            // Execute the call
            ctx->Execute();

            // Get the object that was created
            asIScriptObject* obj = *(asIScriptObject**)ctx->GetAddressOfReturnValue();

            obj->AddRef();
            type->Release();

            ctx->Release();

            return obj;
        }
    }

    template<class... Args>
    inline int callmethod(const char* funcdecl, asIScriptObject* obj, asITypeInfo* type, asIScriptEngine* engine, Args... args)
    {
        // Create our context, prepare it, and then execute
        asIScriptContext* ctx = engine->CreateContext();

        asIScriptFunction* func = type->GetMethodByDecl(funcdecl);

        if (func == nullptr)
            return 0;

        ctx->Prepare(func);

        if constexpr (sizeof...(args) > 0)
        {
            int num = 0;
            ((max::angelscript::SetArg(ctx, num++, args)), ...);
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

    inline void MessageCallback(const asSMessageInfo* msg, void* param)
    {
        const char* type = "ERR";
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

#undef _SetArg