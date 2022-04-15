#include "AngelScriptEngine.h"

namespace max::angelscript
{
    std::unordered_map<const char*, asITypeInfo*> Engine::types;

    template<typename T>
    static void Constructor_def(void* self)
    {
        new(self) T();
    }

    template<typename T>
    static void Constructor_copy(void* self, const T& other)
    {
        new(self) T(other);
    }

    template<typename T, typename... Args>
    static void Constructor(void* self, const Args&... args)
    {
        new(self) T(args...);
    }

    Engine::~Engine()
    {
        for (auto&& [name, mod] : types)
        {
            mod->Release();
        }
        engine->ShutDownAndRelease();
    }

    void MessageCallback(const asSMessageInfo* msg, void* param)
    {
        const char* type = "ERR";
        if (msg->type == asEMsgType::asMSGTYPE_WARNING)
            type = "WARN";
        else if (msg->type == asEMsgType::asMSGTYPE_INFORMATION)
            type = "INFO";
        printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
    }

    void print(const std::string& str)
    {
        printf("%s", str.c_str());
    }

    // Engine::
    asIScriptModule* Engine::BuildScript(const char* filename)
    {
        int r = builder.StartNewModule(engine, filename);
        if (r < 0)
        {
            printf("Unrecoverable error while starting a new module.\n");
            return nullptr;
        }
        r = builder.AddSectionFromFile((max::scriptpath() + filename).c_str());
        if (r < 0)
        {
            printf("Please correct the errors in the script and try again.\n");
            return nullptr;
        }
        r = builder.BuildModule();
        if (r < 0)
        {
            printf("Please correct the errors in the script and try again.\n");
            return nullptr;
        }

        return engine->GetModule(filename);
    }

    asIScriptContext* Engine::I_CreateContext()
    {
        return engine->CreateContext();
    }

    asIScriptModule* Engine::I_GetModule(const char* filename)
    {
        asIScriptModule* smod = engine->GetModule(filename);
        return smod == nullptr ? BuildScript(filename) : smod;
    }

    void Engine::I_ReleaseObject(asIScriptObject* obj)
    {
        engine->ReleaseScriptObject(obj, obj->GetObjectType());
    }

    Engine::Engine()
    {
        int r = engine->SetMessageCallback(asFUNCTION(max::angelscript::MessageCallback), 0, asCALL_CDECL); assert(r >= 0);
        RegisterStdString(engine);
        r = engine->RegisterGlobalFunction("void print(const string&in)", asFUNCTION(max::angelscript::print), asCALL_CDECL); assert(r >= 0);

        r = engine->SetDefaultNamespace("olc"); assert(r >= 0);

        // Register a primitive type, that doesn't need any special management of the content
        r = engine->RegisterObjectType("vi2d", sizeof(olc::vi2d), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<olc::vi2d>()); assert(r >= 0);
        r = engine->RegisterObjectType("vu2d", sizeof(olc::vu2d), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<olc::vu2d>()); assert(r >= 0);
        r = engine->RegisterObjectType("vf2d", sizeof(olc::vf2d), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<olc::vf2d>()); assert(r >= 0);
        r = engine->RegisterObjectType("vd2d", sizeof(olc::vd2d), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<olc::vd2d>()); assert(r >= 0);

        // Register the object properties
        r = engine->RegisterObjectProperty("vi2d", "int32 x", asOFFSET(olc::vi2d, x)); assert(r >= 0);
        r = engine->RegisterObjectProperty("vi2d", "int32 y", asOFFSET(olc::vi2d, y)); assert(r >= 0);

        r = engine->RegisterObjectProperty("vu2d", "uint32 x", asOFFSET(olc::vu2d, x)); assert(r >= 0);
        r = engine->RegisterObjectProperty("vu2d", "uint32 y", asOFFSET(olc::vu2d, y)); assert(r >= 0);

        r = engine->RegisterObjectProperty("vf2d", "float x", asOFFSET(olc::vf2d, x)); assert(r >= 0);
        r = engine->RegisterObjectProperty("vf2d", "float y", asOFFSET(olc::vf2d, y)); assert(r >= 0);

        r = engine->RegisterObjectProperty("vd2d", "double x", asOFFSET(olc::vd2d, x)); assert(r >= 0);
        r = engine->RegisterObjectProperty("vd2d", "double y", asOFFSET(olc::vd2d, y)); assert(r >= 0);

        // Register the constructors
        r = engine->RegisterObjectBehaviour("vi2d", asBEHAVE_CONSTRUCT, "void f()",                               asFUNCTION(v2d_Constructor_def<olc::vi2d>), asCALL_CDECL_OBJFIRST); assert(r >= 0);
        r = engine->RegisterObjectBehaviour("vi2d", asBEHAVE_CONSTRUCT, "void f(const vi2d&in)",                  asFUNCTION(v2d_Constructor_copy<olc::vi2d>), asCALL_CDECL_OBJFIRST); assert(r >= 0);
        r = engine->RegisterObjectBehaviour("vi2d", asBEHAVE_CONSTRUCT, "void f(const int32&in, const int32&in)", asFUNCTION(v2d_Constructor<olc::vi2d, int32_t, int32_t>), asCALL_CDECL_OBJFIRST); assert(r >= 0);

        r = engine->RegisterObjectBehaviour("vu2d", asBEHAVE_CONSTRUCT, "void f()",                                 asFUNCTION(v2d_Constructor_def<olc::vu2d>), asCALL_CDECL_OBJFIRST); assert(r >= 0);
        r = engine->RegisterObjectBehaviour("vu2d", asBEHAVE_CONSTRUCT, "void f(const olc::vu2d&in)",               asFUNCTION(v2d_Constructor_copy<olc::vu2d>), asCALL_CDECL_OBJFIRST); assert(r >= 0);
        r = engine->RegisterObjectBehaviour("vu2d", asBEHAVE_CONSTRUCT, "void f(const uint32&in, const uint32&in)", asFUNCTION(v2d_Constructor<olc::vu2d, uint32_t, uint32_t>), asCALL_CDECL_OBJFIRST); assert(r >= 0);

        r = engine->RegisterObjectBehaviour("vf2d", asBEHAVE_CONSTRUCT, "void f()",                               asFUNCTION(v2d_Constructor_def<olc::vd2d>), asCALL_CDECL_OBJFIRST); assert(r >= 0);
        r = engine->RegisterObjectBehaviour("vf2d", asBEHAVE_CONSTRUCT, "void f(const olc::vf2d&in)",             asFUNCTION(v2d_Constructor_copy<olc::vd2d>), asCALL_CDECL_OBJFIRST); assert(r >= 0);
        r = engine->RegisterObjectBehaviour("vf2d", asBEHAVE_CONSTRUCT, "void f(const float&in, const float&in)", asFUNCTION(v2d_Constructor<olc::vd2d, double, double>), asCALL_CDECL_OBJFIRST); assert(r >= 0);

        r = engine->RegisterObjectBehaviour("vd2d", asBEHAVE_CONSTRUCT, "void f()",                                 asFUNCTION(v2d_Constructor_def<olc::vf2d>), asCALL_CDECL_OBJFIRST); assert(r >= 0);
        r = engine->RegisterObjectBehaviour("vd2d", asBEHAVE_CONSTRUCT, "void f(const olc::vd2d&in)",               asFUNCTION(v2d_Constructor_copy<olc::vf2d>), asCALL_CDECL_OBJFIRST); assert(r >= 0);
        r = engine->RegisterObjectBehaviour("vd2d", asBEHAVE_CONSTRUCT, "void f(const double&in, const double&in)", asFUNCTION(v2d_Constructor<olc::vf2d, float, float>), asCALL_CDECL_OBJFIRST); assert(r >= 0);
        
        // Registering the operators
        engine->RegisterObjectMethod("vi2d", "olc::vi2d  opImplConv ()                                const", asMETHODPR  (olc::vi2d, operator olc::vi2d, ()                 const,            olc::vi2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vu2d  opImplConv ()                                const", asMETHODPR  (olc::vi2d, operator olc::vu2d, ()                 const,            olc::vu2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vf2d  opImplConv ()                                const", asMETHODPR  (olc::vi2d, operator olc::vf2d, ()                 const,            olc::vf2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vd2d  opImplConv ()                                const", asMETHODPR  (olc::vi2d, operator olc::vd2d, ()                 const,            olc::vd2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "const string     opImplConv ()                                const", asMETHODPR  (olc::vi2d, str,                ()                 const,      const std::string), asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d  opSub      (const olc::vi2d&in)                const", asMETHODPR  (olc::vi2d, operator -,         (const olc::vi2d&) const,            olc::vi2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d  opMul      (const int32&in)                    const", asMETHODPR  (olc::vi2d, operator *,         (const int32_t&)   const,            olc::vi2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d  opMul      (const olc::vi2d&in)                const", asMETHODPR  (olc::vi2d, operator *,         (const olc::vi2d&) const,            olc::vi2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d  opDiv      (const int32&in)                    const", asMETHODPR  (olc::vi2d, operator /,         (const int32_t&)   const,            olc::vi2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d  opDiv      (const olc::vi2d&in)                const", asMETHODPR  (olc::vi2d, operator /,         (const olc::vi2d&) const,            olc::vi2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d  opAdd      ()                                const", asMETHODPR  (olc::vi2d, operator +,         ()                 const,            olc::vi2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d  opSub      ()                                const", asMETHODPR  (olc::vi2d, operator -,         ()                 const,            olc::vi2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "bool       opEquals   (const olc::vi2d&in)                const", asMETHODPR  (olc::vi2d, operator==,         (const olc::vi2d&) const,            bool),        asCALL_THISCALL);
        //engine->RegisterObjectMethod("vi2d", "olc::vi2d  opMul_r    (const float&in,  const olc::vi2d&in) const", asFUNCTIONPR(olc::      operator *,         (const int32_t&, const olc::vi2d&),  olc::vi2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vi2d", "olc::vi2d  opMul_r    (const float&in,  const olc::vi2d&in) const", asFUNCTIONPR(olc::      operator *,         (const uint32_t&, const olc::vi2d&), olc::vi2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vi2d", "olc::vi2d  opMul_r    (const float&in,  const olc::vi2d&in) const", asFUNCTIONPR(olc::      operator *,         (const float&, const olc::vi2d&),    olc::vi2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vi2d", "olc::vi2d  opMul_r    (const float&in,  const olc::vi2d&in) const", asFUNCTIONPR(olc::      operator *,         (const double&, const olc::vi2d&),   olc::vi2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vi2d", "olc::vi2d  opDiv_r    (const float&in,  const olc::vi2d&in) const", asFUNCTIONPR(olc::      operator /,         (const int32_t&, const olc::vi2d&),  olc::vi2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vi2d", "olc::vi2d  opDiv_r    (const float&in,  const olc::vi2d&in) const", asFUNCTIONPR(olc::      operator /,         (const uint32_t&, const olc::vi2d&), olc::vi2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vi2d", "olc::vi2d  opDiv_r    (const float&in,  const olc::vi2d&in) const", asFUNCTIONPR(olc::      operator /,         (const float&, const olc::vi2d&),    olc::vi2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vi2d", "olc::vi2d  opDiv_r    (const float&in,  const olc::vi2d&in) const", asFUNCTIONPR(olc::      operator /,         (const double&, const olc::vi2d&),   olc::vi2d),   asCALL_CDECL_OBJLAST);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d& opAddAssign(const olc::vi2d&in)",                      asMETHODPR  (olc::vi2d, operator+=,         (const olc::vi2d&),                  olc::vi2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d& opSubAssign(const olc::vi2d&in)",                      asMETHODPR  (olc::vi2d, operator-=,         (const olc::vi2d&),                  olc::vi2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d& opMulAssign(const int32&in)",                          asMETHODPR  (olc::vi2d, operator*=,         (const int32_t&),                    olc::vi2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d& opDivAssign(const int32&in)",                          asMETHODPR  (olc::vi2d, operator/=,         (const int32_t&),                    olc::vi2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d& opMulAssign(const olc::vi2d&in)",                      asMETHODPR  (olc::vi2d, operator*=,         (const olc::vi2d&),                  olc::vi2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d& opDivAssign(const olc::vi2d&in)",                      asMETHODPR  (olc::vi2d, operator/=,         (const olc::vi2d&),                  olc::vi2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d& opAssign   (const olc::vi2d&in)",                      asMETHODPR  (olc::vi2d, operator =,         (const olc::vi2d&),                  olc::vi2d&),  asCALL_THISCALL);

        engine->RegisterObjectMethod("vu2d", "olc::vi2d    opImplConv   ()                               const", asMETHODPR  (olc::vu2d, operator olc::vi2d, ()                 const,            olc::vi2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d    opImplConv   ()                               const", asMETHODPR  (olc::vu2d, operator olc::vu2d, ()                 const,            olc::vu2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vf2d    opImplConv   ()                               const", asMETHODPR  (olc::vu2d, operator olc::vf2d, ()                 const,            olc::vf2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vd2d    opImplConv   ()                               const", asMETHODPR  (olc::vu2d, operator olc::vd2d, ()                 const,            olc::vd2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "const string       opImplConv   ()                               const", asMETHODPR  (olc::vu2d, str,                ()                 const,      const std::string), asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d    opSub        (const vu2d&in)                  const", asMETHODPR  (olc::vu2d, operator -,         (const olc::vu2d&) const,            olc::vu2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d    opMul        (const uint32&in)                const", asMETHODPR  (olc::vu2d, operator *,         (const uint32_t&)  const,            olc::vu2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d    opMul        (const vu2d&in)                  const", asMETHODPR  (olc::vu2d, operator *,         (const olc::vu2d&) const,            olc::vu2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d    opDiv        (const uint32&in)                const", asMETHODPR  (olc::vu2d, operator /,         (const uint32_t&)  const,            olc::vu2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d    opDiv        (const vu2d&in)                  const", asMETHODPR  (olc::vu2d, operator /,         (const olc::vu2d&) const,            olc::vu2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d    opAdd        ()                               const", asMETHODPR  (olc::vu2d, operator +,         ()                 const,            olc::vu2d),   asCALL_THISCALL);
        //engine->RegisterObjectMethod("vu2d", "olc::vu2d  opSub      ()                           const", asMETHODPR  (olc::vu2d, operator -,         ()                 const,            olc::vu2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "bool         opEquals     (const vu2d&in)                  const", asMETHODPR  (olc::vu2d, operator==,         (const olc::vu2d&) const,            bool),        asCALL_THISCALL);
        //engine->RegisterObjectMethod("vu2d", "olc::vu2d    opMul_r      (const float&in,  const vu2d&in) const", asFUNCTIONPR(olc::      operator *,         (const int32_t&, const olc::vu2d&),  olc::vu2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vu2d", "olc::vu2d    opMul_r      (const float&in,  const vu2d&in) const", asFUNCTIONPR(olc::      operator *,         (const uint32_t&, const olc::vu2d&), olc::vu2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vu2d", "olc::vu2d    opMul_r      (const float&in,  const vu2d&in) const", asFUNCTIONPR(olc::      operator *,         (const float&, const olc::vu2d&),    olc::vu2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vu2d", "olc::vu2d    opMul_r      (const float&in,  const vu2d&in) const", asFUNCTIONPR(olc::      operator *,         (const double&, const olc::vu2d&),   olc::vu2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vu2d", "olc::vu2d    opDiv_r      (const float&in,  const vu2d&in) const", asFUNCTIONPR(olc::      operator /,         (const int32_t&, const olc::vu2d&),  olc::vu2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vu2d", "olc::vu2d    opDiv_r      (const float&in,  const vu2d&in) const", asFUNCTIONPR(olc::      operator /,         (const uint32_t&, const olc::vu2d&), olc::vu2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vu2d", "olc::vu2d    opDiv_r      (const float&in,  const vu2d&in) const", asFUNCTIONPR(olc::      operator /,         (const float&, const olc::vu2d&),    olc::vu2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vu2d", "olc::vu2d    opDiv_r      (const float&in,  const vu2d&in) const", asFUNCTIONPR(olc::      operator /,         (const double&, const olc::vu2d&),   olc::vu2d),   asCALL_CDECL_OBJLAST);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d& opAddAssign(const vu2d&in)",                          asMETHODPR  (olc::vu2d, operator+=,         (const olc::vu2d&),                  olc::vu2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d& opSubAssign(const vu2d&in)",                          asMETHODPR  (olc::vu2d, operator-=,         (const olc::vu2d&),                  olc::vu2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d& opMulAssign(const uint32&in)",                        asMETHODPR  (olc::vu2d, operator*=,         (const uint32_t&),                   olc::vu2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d& opDivAssign(const uint32&in)",                        asMETHODPR  (olc::vu2d, operator/=,         (const uint32_t&),                   olc::vu2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d& opMulAssign(const vu2d&in)",                          asMETHODPR  (olc::vu2d, operator*=,         (const olc::vu2d&),                  olc::vu2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d& opDivAssign(const vu2d&in)",                          asMETHODPR  (olc::vu2d, operator/=,         (const olc::vu2d&),                  olc::vu2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d& opAssign   (const vu2d&in)",                          asMETHODPR  (olc::vu2d, operator =,         (const olc::vu2d&),                  olc::vu2d&),  asCALL_THISCALL);

        engine->RegisterObjectMethod("vf2d", "olc::vi2d  opImplConv ()                           const", asMETHODPR  (olc::vf2d, operator olc::vi2d, ()                 const,            olc::vi2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vu2d  opImplConv ()                           const", asMETHODPR  (olc::vf2d, operator olc::vu2d, ()                 const,            olc::vu2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d  opImplConv ()                           const", asMETHODPR  (olc::vf2d, operator olc::vf2d, ()                 const,            olc::vf2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vd2d  opImplConv ()                           const", asMETHODPR  (olc::vf2d, operator olc::vd2d, ()                 const,            olc::vd2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "const string     opImplConv ()                           const", asMETHODPR  (olc::vf2d, str,                ()                 const,      const std::string), asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d  opSub      (const vf2d&in)                const", asMETHODPR  (olc::vf2d, operator -,         (const olc::vf2d&) const,            olc::vf2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d  opMul      (const int32&in)               const", asMETHODPR  (olc::vf2d, operator *,         (const float&)     const,            olc::vf2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d  opMul      (const vf2d&in)                const", asMETHODPR  (olc::vf2d, operator *,         (const olc::vf2d&) const,            olc::vf2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d  opDiv      (const int32&in)               const", asMETHODPR  (olc::vf2d, operator /,         (const float&)     const,            olc::vf2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d  opDiv      (const vf2d&in)                const", asMETHODPR  (olc::vf2d, operator /,         (const olc::vf2d&) const,            olc::vf2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d  opAdd      ()                           const", asMETHODPR  (olc::vf2d, operator +,         ()                 const,            olc::vf2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d  opSub      ()                           const", asMETHODPR  (olc::vf2d, operator -,         ()                 const,            olc::vf2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "bool       opEquals   (const vf2d&in)                const", asMETHODPR  (olc::vf2d, operator==,         (const olc::vf2d&) const,            bool),        asCALL_THISCALL);
        //engine->RegisterObjectMethod("vf2d", "olc::vf2d  opMul_r    (const float&in,  const vf2d&in) const", asFUNCTIONPR(olc::      operator *,         (const int32_t&, const olc::vf2d&),  olc::vf2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vf2d", "olc::vf2d  opMul_r    (const float&in,  const vf2d&in) const", asFUNCTIONPR(olc::      operator *,         (const uint32_t&, const olc::vf2d&), olc::vf2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vf2d", "olc::vf2d  opMul_r    (const float&in,  const vf2d&in) const", asFUNCTIONPR(olc::      operator *,         (const float&, const olc::vf2d&),    olc::vf2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vf2d", "olc::vf2d  opMul_r    (const float&in,  const vf2d&in) const", asFUNCTIONPR(olc::      operator *,         (const double&, const olc::vf2d&),   olc::vf2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vf2d", "olc::vf2d  opDiv_r    (const float&in,  const vf2d&in) const", asFUNCTIONPR(olc::      operator /,         (const int32_t&, const olc::vf2d&),  olc::vf2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vf2d", "olc::vf2d  opDiv_r    (const float&in,  const vf2d&in) const", asFUNCTIONPR(olc::      operator /,         (const uint32_t&, const olc::vf2d&), olc::vf2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vf2d", "olc::vf2d  opDiv_r    (const float&in,  const vf2d&in) const", asFUNCTIONPR(olc::      operator /,         (const float&, const olc::vf2d&),    olc::vf2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vf2d", "olc::vf2d  opDiv_r    (const float&in,  const vf2d&in) const", asFUNCTIONPR(olc::      operator /,         (const double&, const olc::vf2d&),   olc::vf2d),   asCALL_CDECL_OBJLAST);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d& opAddAssign(const vf2d&in)",                      asMETHODPR  (olc::vf2d, operator+=,         (const olc::vf2d&),                  olc::vf2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d& opSubAssign(const vf2d&in)",                      asMETHODPR  (olc::vf2d, operator-=,         (const olc::vf2d&),                  olc::vf2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d& opMulAssign(const int32&in)",                     asMETHODPR  (olc::vf2d, operator*=,         (const float&),                      olc::vf2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d& opDivAssign(const int32&in)",                     asMETHODPR  (olc::vf2d, operator/=,         (const float&),                      olc::vf2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d& opMulAssign(const vf2d&in)",                      asMETHODPR  (olc::vf2d, operator*=,         (const olc::vf2d&),                  olc::vf2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d& opDivAssign(const vf2d&in)",                      asMETHODPR  (olc::vf2d, operator/=,         (const olc::vf2d&),                  olc::vf2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d& opAssign   (const vf2d&in)",                      asMETHODPR  (olc::vf2d, operator =,         (const olc::vf2d&),                  olc::vf2d&),  asCALL_THISCALL);

        engine->RegisterObjectMethod("vd2d", "olc::vi2d  opImplConv ()                           const",      asMETHODPR  (olc::vd2d, operator olc::vi2d, ()                 const,            olc::vi2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vu2d  opImplConv ()                           const",      asMETHODPR  (olc::vd2d, operator olc::vu2d, ()                 const,            olc::vu2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vf2d  opImplConv ()                           const",      asMETHODPR  (olc::vd2d, operator olc::vf2d, ()                 const,            olc::vf2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d  opImplConv ()                           const",      asMETHODPR  (olc::vd2d, operator olc::vd2d, ()                 const,            olc::vd2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "const string     opImplConv ()                           const",      asMETHODPR  (olc::vd2d, str,                ()                 const,      const std::string), asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d  opSub      (const olc::vd2d&in)           const",      asMETHODPR  (olc::vd2d, operator -,         (const olc::vd2d&) const,            olc::vd2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d  opMul      (const int32&in)               const",      asMETHODPR  (olc::vd2d, operator *,         (const double&)    const,            olc::vd2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d  opMul      (const olc::vd2d&in)           const",      asMETHODPR  (olc::vd2d, operator *,         (const olc::vd2d&) const,            olc::vd2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d  opDiv      (const int32&in)               const",      asMETHODPR  (olc::vd2d, operator /,         (const double&)    const,            olc::vd2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d  opDiv      (const olc::vd2d&in)           const",      asMETHODPR  (olc::vd2d, operator /,         (const olc::vd2d&) const,            olc::vd2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d  opAdd      ()                           const",      asMETHODPR  (olc::vd2d, operator +,         ()                 const,            olc::vd2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d  opSub      ()                           const",      asMETHODPR  (olc::vd2d, operator -,         ()                 const,            olc::vd2d),   asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "bool  opEquals   (const olc::vd2d&in)                const",      asMETHODPR  (olc::vd2d, operator==,         (const olc::vd2d&) const,            bool),        asCALL_THISCALL);
        //engine->RegisterObjectMethod("vd2d", "olc::vd2d  opMul_r    (const float&in,  const olc::vd2d&in) const", asFUNCTIONPR(olc::      operator *,         (const int32_t&, const olc::vd2d&),  olc::vd2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vd2d", "olc::vd2d  opMul_r    (const float&in,  const olc::vd2d&in) const", asFUNCTIONPR(olc::      operator *,         (const uint32_t&, const olc::vd2d&), olc::vd2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vd2d", "olc::vd2d  opMul_r    (const float&in,  const olc::vd2d&in) const", asFUNCTIONPR(olc::      operator *,         (const float&, const olc::vd2d&),    olc::vd2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vd2d", "olc::vd2d  opMul_r    (const float&in,  const olc::vd2d&in) const", asFUNCTIONPR(olc::      operator *,         (const double&, const olc::vd2d&),   olc::vd2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vd2d", "olc::vd2d  opDiv_r    (const float&in,  const olc::vd2d&in) const", asFUNCTIONPR(olc::      operator /,         (const int32_t&, const olc::vd2d&),  olc::vd2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vd2d", "olc::vd2d  opDiv_r    (const float&in,  const olc::vd2d&in) const", asFUNCTIONPR(olc::      operator /,         (const uint32_t&, const olc::vd2d&), olc::vd2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vd2d", "olc::vd2d  opDiv_r    (const float&in,  const olc::vd2d&in) const", asFUNCTIONPR(olc::      operator /,         (const float&, const olc::vd2d&),    olc::vd2d),   asCALL_CDECL_OBJLAST);
        //engine->RegisterObjectMethod("vd2d", "olc::vd2d  opDiv_r    (const float&in,  const olc::vd2d&in) const", asFUNCTIONPR(olc::      operator /,         (const double&, const olc::vd2d&),   olc::vd2d),   asCALL_CDECL_OBJLAST);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d& opAddAssign(const olc::vd2d&in)",                      asMETHODPR  (olc::vd2d, operator+=,         (const olc::vd2d&),                  olc::vd2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d& opSubAssign(const olc::vd2d&in)",                      asMETHODPR  (olc::vd2d, operator-=,         (const olc::vd2d&),                  olc::vd2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d& opMulAssign(const int32&in)",                          asMETHODPR  (olc::vd2d, operator*=,         (const double&),                     olc::vd2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d& opDivAssign(const int32&in)",                          asMETHODPR  (olc::vd2d, operator/=,         (const double&),                     olc::vd2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d& opMulAssign(const olc::vd2d&in)",                      asMETHODPR  (olc::vd2d, operator*=,         (const olc::vd2d&),                  olc::vd2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d& opDivAssign(const olc::vd2d&in)",                      asMETHODPR  (olc::vd2d, operator/=,         (const olc::vd2d&),                  olc::vd2d&),  asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d& opAssign   (const olc::vd2d&in)",                      asMETHODPR  (olc::vd2d, operator =,         (const olc::vd2d&),                  olc::vd2d&),  asCALL_THISCALL);

        // Registering the methods
        engine->RegisterObjectMethod("vi2d", "const string    str       ()                           const", asMETHODPR(olc::vi2d, str,    ()                               const, const std::string), asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "int32     mag       ()                           const", asMETHODPR(olc::vi2d, mag,    ()                               const, int32_t),           asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "int32     mag2      ()                           const", asMETHODPR(olc::vi2d, mag2,   ()                               const, int32_t),           asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d norm      ()                           const", asMETHODPR(olc::vi2d, norm,   ()                               const, olc::vi2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d perp      ()                           const", asMETHODPR(olc::vi2d, perp,   ()                               const, olc::vi2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d floor     ()                           const", asMETHODPR(olc::vi2d, floor,  ()                               const, olc::vi2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d ceil      ()                           const", asMETHODPR(olc::vi2d, ceil,   ()                               const, olc::vi2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d max       (const olc::vi2d&in)           const", asMETHODPR(olc::vi2d, max,    (const olc::vi2d&)               const, olc::vi2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d min       (const olc::vi2d&in)           const", asMETHODPR(olc::vi2d, min,    (const olc::vi2d&)               const, olc::vi2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "int32     dot       (const olc::vi2d&in)           const", asMETHODPR(olc::vi2d, dot,    (const olc::vi2d&)               const, int32_t),           asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "int32     cross     (const olc::vi2d&in)           const", asMETHODPR(olc::vi2d, cross,  (const olc::vi2d&)               const, int32_t),           asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d xclamp    (const int32&in, const int32&in) const", asMETHODPR(olc::vi2d, xclamp, (const int32_t&, const int32_t&) const, olc::vi2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d yclamp    (const int32&in, const int32&in) const", asMETHODPR(olc::vi2d, yclamp, (const int32_t&, const int32_t&) const, olc::vi2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d xclamp    (const int32&in, const int32&in)",       asMETHODPR(olc::vi2d, xclamp, (const int32_t&, const int32_t&),       olc::vi2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d yclamp    (const int32&in, const int32&in)",       asMETHODPR(olc::vi2d, yclamp, (const int32_t&, const int32_t&),       olc::vi2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d cart      ()",                                 asMETHODPR(olc::vi2d, cart,   (),                                     olc::vi2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vi2d", "olc::vi2d polar     ()",                                 asMETHODPR(olc::vi2d, polar,  (),                                     olc::vi2d),         asCALL_THISCALL);

        engine->RegisterObjectMethod("vu2d", "const string    str       ()                             const", asMETHODPR(olc::vu2d, str,    ()                                 const, const std::string), asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "uint32    mag       ()                             const", asMETHODPR(olc::vu2d, mag,    ()                                 const, uint32_t),          asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "uint32    mag2      ()                             const", asMETHODPR(olc::vu2d, mag2,   ()                                 const, uint32_t),          asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d norm      ()                             const", asMETHODPR(olc::vu2d, norm,   ()                                 const, olc::vu2d),         asCALL_THISCALL);
        //engine->RegisterObjectMethod("vu2d", "olc::vu2d perp      ()                             const", asMETHODPR(olc::vu2d, perp,   ()                                 const, olc::vu2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d floor     ()                             const", asMETHODPR(olc::vu2d, floor,  ()                                 const, olc::vu2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d ceil      ()                             const", asMETHODPR(olc::vu2d, ceil,   ()                                 const, olc::vu2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d max       (const olc::vu2d&in)             const", asMETHODPR(olc::vu2d, max,    (const olc::vu2d&)                 const, olc::vu2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d min       (const olc::vu2d&in)             const", asMETHODPR(olc::vu2d, min,    (const olc::vu2d&)                 const, olc::vu2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "uint32    dot       (const olc::vu2d&in)             const", asMETHODPR(olc::vu2d, dot,    (const olc::vu2d&)                 const, uint32_t),          asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "uint32    cross     (const olc::vu2d&in)             const", asMETHODPR(olc::vu2d, cross,  (const olc::vu2d&)                 const, uint32_t),          asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d xclamp    (const uint32&in, const uint32&in) const", asMETHODPR(olc::vu2d, xclamp, (const uint32_t&, const uint32_t&) const, olc::vu2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d yclamp    (const uint32&in, const uint32&in) const", asMETHODPR(olc::vu2d, yclamp, (const uint32_t&, const uint32_t&) const, olc::vu2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d xclamp    (const uint32&in, const uint32&in)",       asMETHODPR(olc::vu2d, xclamp, (const uint32_t&, const uint32_t&),       olc::vu2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d yclamp    (const uint32&in, const uint32&in)",       asMETHODPR(olc::vu2d, yclamp, (const uint32_t&, const uint32_t&),       olc::vu2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d cart      ()",                                   asMETHODPR(olc::vu2d, cart,   (),                                       olc::vu2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vu2d", "olc::vu2d polar     ()",                                   asMETHODPR(olc::vu2d, polar,  (),                                       olc::vu2d),         asCALL_THISCALL);

        engine->RegisterObjectMethod("vf2d", "const string    str       ()                           const", asMETHODPR(olc::vf2d, str,    ()                           const, const std::string), asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "float     mag       ()                           const", asMETHODPR(olc::vf2d, mag,    ()                           const, float),             asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "float     mag2      ()                           const", asMETHODPR(olc::vf2d, mag2,   ()                           const, float),             asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d norm      ()                           const", asMETHODPR(olc::vf2d, norm,   ()                           const, olc::vf2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d perp      ()                           const", asMETHODPR(olc::vf2d, perp,   ()                           const, olc::vf2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d floor     ()                           const", asMETHODPR(olc::vf2d, floor,  ()                           const, olc::vf2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d ceil      ()                           const", asMETHODPR(olc::vf2d, ceil,   ()                           const, olc::vf2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d max       (const olc::vf2d&in)           const", asMETHODPR(olc::vf2d, max,    (const olc::vf2d&)           const, olc::vf2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d min       (const olc::vf2d&in)           const", asMETHODPR(olc::vf2d, min,    (const olc::vf2d&)           const, olc::vf2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "float     dot       (const olc::vf2d&in)           const", asMETHODPR(olc::vf2d, dot,    (const olc::vf2d&)           const, float),             asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "float     cross     (const olc::vf2d&in)           const", asMETHODPR(olc::vf2d, cross,  (const olc::vf2d&)           const, float),             asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d xclamp    (const float&in, const float&in) const", asMETHODPR(olc::vf2d, xclamp, (const float&, const float&) const, olc::vf2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d yclamp    (const float&in, const float&in) const", asMETHODPR(olc::vf2d, yclamp, (const float&, const float&) const, olc::vf2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d xclamp    (const float&in, const float&in)",       asMETHODPR(olc::vf2d, xclamp, (const float&, const float&),       olc::vf2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d yclamp    (const float&in, const float&in)",       asMETHODPR(olc::vf2d, yclamp, (const float&, const float&),       olc::vf2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d cart      ()",                                 asMETHODPR(olc::vf2d, cart,   (),                                 olc::vf2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vf2d", "olc::vf2d polar     ()",                                 asMETHODPR(olc::vf2d, polar,  (),                                 olc::vf2d),         asCALL_THISCALL);

        engine->RegisterObjectMethod("vd2d", "const string    str       ()                             const", asMETHODPR(olc::vd2d, str,    ()                             const, const std::string), asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "double    mag       ()                             const", asMETHODPR(olc::vd2d, mag,    ()                             const, double),            asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "double    mag2      ()                             const", asMETHODPR(olc::vd2d, mag2,   ()                             const, double),            asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d norm      ()                             const", asMETHODPR(olc::vd2d, norm,   ()                             const, olc::vd2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d perp      ()                             const", asMETHODPR(olc::vd2d, perp,   ()                             const, olc::vd2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d floor     ()                             const", asMETHODPR(olc::vd2d, floor,  ()                             const, olc::vd2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d ceil      ()                             const", asMETHODPR(olc::vd2d, ceil,   ()                             const, olc::vd2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d max       (const olc::vd2d&in)             const", asMETHODPR(olc::vd2d, max,    (const olc::vd2d&)             const, olc::vd2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d min       (const olc::vd2d&in)             const", asMETHODPR(olc::vd2d, min,    (const olc::vd2d&)             const, olc::vd2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "double    dot       (const olc::vd2d&in)             const", asMETHODPR(olc::vd2d, dot,    (const olc::vd2d&)             const, double),            asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "double    cross     (const olc::vd2d&in)             const", asMETHODPR(olc::vd2d, cross,  (const olc::vd2d&)             const, double),            asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d xclamp    (const double&in, const double&in) const", asMETHODPR(olc::vd2d, xclamp, (const double&, const double&) const, olc::vd2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d yclamp    (const double&in, const double&in) const", asMETHODPR(olc::vd2d, yclamp, (const double&, const double&) const, olc::vd2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d xclamp    (const double&in, const double&in)",       asMETHODPR(olc::vd2d, xclamp, (const double&, const double&),       olc::vd2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d yclamp    (const double&in, const double&in)",       asMETHODPR(olc::vd2d, yclamp, (const double&, const double&),       olc::vd2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d cart      ()",                                   asMETHODPR(olc::vd2d, cart,   (),                                   olc::vd2d),         asCALL_THISCALL);
        engine->RegisterObjectMethod("vd2d", "olc::vd2d polar     ()",                                   asMETHODPR(olc::vd2d, polar,  (),                                   olc::vd2d),         asCALL_THISCALL);
    }

    Engine& Engine::Get()
    {
        static Engine inst;
        return inst;
    }

    asIScriptContext* Engine::CreateContext()
    {
        return Get().I_CreateContext();
    }

    asITypeInfo* Engine::GetType(const char* typedecl, asIScriptModule* mod)
    {
        if (types.find(typedecl) == types.end())
            return types[typedecl] = mod->GetTypeInfoByDecl(typedecl);

        return types[typedecl];
    
    }

    asITypeInfo* Engine::GetType(const char* typedecl, const char* filename)
    {
        if (types.find(typedecl) == types.end())
            return types[typedecl] = Get().I_GetModule(filename)->GetTypeInfoByDecl(typedecl);


        return types[typedecl];
    }

    asIScriptModule* Engine::GetModule(const char* filename)
    {
        return Get().I_GetModule(filename);
    }

    void Engine::ReleaseObject(asIScriptObject* obj)
    {
        Get().I_ReleaseObject(obj);
    }
}
