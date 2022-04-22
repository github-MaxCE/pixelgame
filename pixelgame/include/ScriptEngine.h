#pragma once
#include <cassert>
#include <unordered_map>
#include <angelscript/angelscript.h>
#include <angelscript/scriptstdstring/scriptstdstring.h>
#include <angelscript/scriptbuilder/scriptbuilder.h>
#include <olcPixelGameEngine.h>
#include "filemanager.h"

#define _SetArg(Type) SetArg##Type

namespace max::script
{
    template<class T>
    inline int SetArg(asIScriptContext*, uint32_t, T)
    {
        static_assert(1);
    }

    template<>
    inline int SetArg<void*>(asIScriptContext* ctx, uint32_t num, void* value)
    {
        return ctx->_SetArg(Address)(num, value);
    }
    template<>
    inline int SetArg<asDWORD>(asIScriptContext* ctx, uint32_t num, asDWORD value)
    {
        return ctx->_SetArg(DWord)(num, value);
    }
    template<>
    inline int SetArg<asQWORD>(asIScriptContext* ctx, uint32_t num, asQWORD value)
    {
        return ctx->_SetArg(QWord)(num, value);
    }
    template<>
    inline int SetArg<float>(asIScriptContext* ctx, uint32_t num, float value)
    {
        return ctx->_SetArg(Float)(num, value);
    }
    template<>
    inline int SetArg<double>(asIScriptContext* ctx, uint32_t num, double value)
    {
        return ctx->_SetArg(Double)(num, value);
    }
    template<>
    inline int SetArg<asBYTE>(asIScriptContext* ctx, uint32_t num, asBYTE value)
    {
        return ctx->_SetArg(Byte)(num, value);
    }
    template<>
    inline int SetArg<asWORD>(asIScriptContext* ctx, uint32_t num, asWORD value)
    {
        return ctx->_SetArg(Word)(num, value);
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
        asIScriptModule* I_GetModule(const char*);
        void I_ReleaseObject(asIScriptObject*);
        template<class... Args>
        inline int I_callfunc(const char* funcdecl, asIScriptModule* mod, Args&&... args)
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
                ((SetArg(ctx, num++, args)), ...);
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

        template<typename T>
        void Registerv2d(const char* t, std::string ast, const char* ns = "");

        static Engine& Get();
    public:

        static asIScriptContext* CreateContext();
        static asITypeInfo* GetType(const char*, asIScriptModule*);
        static asITypeInfo* GetType(const char*, const char*);
        static asIScriptModule* GetModule(const char*);
        static void ReleaseObject(asIScriptObject*);

        template<class... Args>
        static inline int callfunc(const char* funcdecl, const char* filename, Args&&... args) { return Get().I_callfunc(funcdecl, GetModule(filename), std::forward<Args>(args)...); }
        template<class... Args>
        static inline int callfunc(const char* funcdecl, asIScriptModule* mod, Args&&... args) { return Get().I_callfunc(funcdecl, mod, std::forward<Args>(args)...); }
    };
}

#undef _SetArg