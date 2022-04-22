#pragma once
#include "ScriptEngine.h"
#include <cassert>

namespace max::script
{
	class object
	{
    protected:
        asIScriptObject* obj;
        asITypeInfo* type;

	public:
        template<class... Args>
        inline object(const char* typedecl, const char* filename, Args&& ...args) :
            type(Engine::GetType(typedecl, filename))
        {
            asIScriptContext* ctx = Engine::CreateContext();
            
            // Get the factory function from the object type
            asIScriptFunction* factory = type->GetFactoryByDecl((std::string() + type->GetName() + "@ " + type->GetName() + "()").c_str()); assert(factory != 0);

            // Prepare the context to call the factory function
            ctx->Prepare(factory);

            if constexpr(sizeof...(args) > 0)
            {
                int num = 0;
                ((SetArg(ctx, num++, args)), ...);
            }

            // Execute the call
            ctx->Execute();

            // Get the object that was created
            obj = *(asIScriptObject**) ctx->GetAddressOfReturnValue();

            obj->AddRef();
            ctx->Release();
        }

        template<class... Args>
        inline object(const char* typedecl, asIScriptModule* mod, Args&& ...args) :
            type(Engine::GetType(typedecl, mod))
        {
            asIScriptContext* ctx = Engine::CreateContext();

            // Get the factory function from the object type
            asIScriptFunction* factory = type->GetFactoryByDecl((std::string() + type->GetName() + "@ " + type->GetName() + "()").c_str()); assert(factory != 0);

            // Prepare the context to call the factory function
            ctx->Prepare(factory);

            if constexpr (sizeof...(args) > 0)
            {
                int num = 0;
                ((SetArg(ctx, num++, args)), ...);
            }

            // Execute the call
            ctx->Execute();

            // Get the object that was created
            obj = *(asIScriptObject**)ctx->GetAddressOfReturnValue();

            obj->AddRef();
            ctx->Release();
        }

        virtual ~object();

        template<class... Args>
        inline int call(const char* funcdecl, Args&& ...args)
        {
            // Create our context, prepare it, and then execute
            asIScriptContext* ctx = Engine::CreateContext();

            asIScriptFunction* func = type->GetMethodByDecl(funcdecl); assert(func != 0);

            ctx->Prepare(func);

            if constexpr (sizeof...(args) > 0)
            {
                int num = 0;
                ((SetArg(ctx, num++, args)), ...);
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
            return 0;
        }
	};
}