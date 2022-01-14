#pragma once
#include "angelscript/angelscript.h"
#include "angelscript/scriptstdstring/scriptstdstring.h"
#include "angelscript/scriptbuilder/scriptbuilder.h"

namespace max::angelscript
{
    template<class... Args>
    extern int callfunc(asIScriptFunction* func, asIScriptEngine* engine, asIScriptContext* ctx, Args... args);
    template<class... Args>
    extern int callmethod(asIScriptFunction* func, asIScriptObject* obj, asIScriptEngine* engine, asIScriptContext* ctx, Args... args);
    template<class... Args>
    extern int callfactory(const char* objdecl, asITypeInfo* type, asIScriptObject* obj, asIScriptEngine* engine, asIScriptContext* ctx, Args... args);
    extern void MessageCallback(const asSMessageInfo* msg, void* param);
    extern void print(const std::string& str);
}