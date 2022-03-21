#pragma once
#include <unordered_map>
#include <cassert>
#include "filemanager.h"
#include "angelscript/angelscript.h"
#include "angelscript/scriptstdstring/scriptstdstring.h"
#include "angelscript/scriptbuilder/scriptbuilder.h"
#include "olcPixelGameEngine.h"

#define _SetArg(_ctx, Type, num, val) ctx->SetArg##Type(num, val)

namespace max::angelscript
{
    template<class T>
    inline int SetArg(asIScriptContext* ctx, uint32_t num, T value)
    {
        return _SetArg(ctx, Object, num, &value);
    }
    template<>
    inline int SetArg<void*>(asIScriptContext* ctx, uint32_t num, void* value)
    {
        return _SetArg(ctx, Address, num, value);
    }
    template<>
    inline int SetArg<asDWORD>(asIScriptContext* ctx, uint32_t num, asDWORD value)
    {
        return _SetArg(ctx, DWord, num, value);
    }
    template<>
    inline int SetArg<asQWORD>(asIScriptContext* ctx, uint32_t num, asQWORD value)
    {
        return _SetArg(ctx, QWord, num, value);
    }
    template<>
    inline int SetArg<float>(asIScriptContext* ctx, uint32_t num, float value)
    {
        return _SetArg(ctx, Float, num, value);
    }
    template<>
    inline int SetArg<double>(asIScriptContext* ctx, uint32_t num, double value)
    {
        return _SetArg(ctx, Double, num, value);
    }
    template<>
    inline int SetArg<asBYTE>(asIScriptContext* ctx, uint32_t num, asBYTE value)
    {
        return _SetArg(ctx, Byte, num, value);
    }
    template<>
    inline int SetArg<asWORD>(asIScriptContext* ctx, uint32_t num, asWORD value)
    {
        return _SetArg(ctx, Word, num, value);
    }

    void MessageCallback(const asSMessageInfo* msg, void* param);
    void print(const std::string& str);

    class Engine
    {
        asIScriptEngine* engine = asCreateScriptEngine();
        CScriptBuilder builder;
        static std::unordered_map<const char*, asITypeInfo*> types;

        Engine();
        virtual ~Engine();


        asIScriptModule* BuildScript(const char* filename);

        Engine(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) = delete;

        asIScriptContext* I_CreateContext();
        asIScriptModule* I_GetModule(const char* filename);
        void I_ReleaseObject(asIScriptObject* obj);

        template<class... Args>
        inline int I_callfunc(const char* funcdecl, asIScriptModule* mod, Args&& ...args)
        {
            // Create our context, prepare it, and then execute
            asIScriptContext* ctx = CreateContext();

            asIScriptFunction* func = mod->GetFunctionByDecl(funcdecl);
            if(func == 0) printf("%s %s %s", "no", funcdecl, "function. skipping\n");
            assert(func != 0);

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
                    return 1;
                }
            }

            // Clean up
            ctx->Release();
            return 0;
        }

        static Engine& Get();
    public:

        static asIScriptContext* CreateContext();
        static asITypeInfo* GetType(const char* typedecl, asIScriptModule* mod);
        static asITypeInfo* GetType(const char* typedecl, const char* filename);
        static asIScriptModule* GetModule(const char* filename);
        static void ReleaseObject(asIScriptObject* obj);

        template<class... Args>
        static inline int callfunc(const char* funcdecl, const char* filename, Args&& ...args) { return Get().I_callfunc(funcdecl, GetModule(filename), std::forward<Args>(args)...); }
        template<class... Args>
        static inline int callfunc(const char* funcdecl, asIScriptModule* mod, Args&& ...args) { return Get().I_callfunc(funcdecl, mod, std::forward<Args>(args)...); }
    };
}

#undef _SetArg