#include "ScriptEngine.h"

namespace max::script
{
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
    static void Constructor(void* self, Args&&... args)
    {
        new(self) T(std::forward<Args>(args)...);
    }

    Engine::~Engine()
    {
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

    asIScriptModule* Engine::BuildScript(const char* filename)
    {
        if (engine->GetModule(filename) != nullptr) return engine->GetModule(filename);

        int r = builder.StartNewModule(engine, filename);
        if (r < 0)
        {
            printf("Unrecoverable error while starting a new module.\n");
            return nullptr;
        }
        r = builder.AddSectionFromFile((scriptpath() + filename).c_str());
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
        return BuildScript(filename);
    }

    asITypeInfo* Engine::I_GetType(const char* typedecl, asIScriptModule* mod)
    {
        return mod->GetTypeInfoByDecl(typedecl);
    }

    void Engine::I_ReleaseObject(asIScriptObject* obj, asITypeInfo* type)
    {
        engine->ReleaseScriptObject(obj, type);
    }

    template<typename T>
    void Engine::Registerv2d_t(const char* t, std::string ast)
    {
        std::string type;
        type = type + engine->GetDefaultNamespace() + "::" + t;

        // Register a primitive type, that doesn't need any special management of the content
        int r = engine->RegisterObjectType(t, sizeof(olc::v2d_generic<T>), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<olc::v2d_generic<T>>()); assert(r >= 0);
    
        // Register the object properties
        r = engine->RegisterObjectProperty(t, (ast + " x").c_str(), asOFFSET(olc::v2d_generic<T>, x)); assert(r >= 0);
        r = engine->RegisterObjectProperty(t, (ast + " y").c_str(), asOFFSET(olc::v2d_generic<T>, y)); assert(r >= 0);
    
        // Register the constructors
        r = engine->RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f()",                                             asFUNCTION(Constructor_def<olc::v2d_generic<T>>),                   asCALL_CDECL_OBJFIRST); assert(r >= 0);
        r = engine->RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, ("void f(const "+type+"& in)").c_str(),                 asFUNCTION(Constructor_copy<olc::v2d_generic<T>>),                  asCALL_CDECL_OBJFIRST); assert(r >= 0);
        r = engine->RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, ("void f(const "+ast+"&in, const "+ast+"&in)").c_str(), asFUNCTION((Constructor<olc::v2d_generic<T>, const T&, const T&>)), asCALL_CDECL_OBJFIRST); assert(r >= 0);
    }
    
    template<typename T>
    void Engine::Registerv2d(const char* t, std::string ast)
    {
        std::string type;
        type = type + engine->GetDefaultNamespace() + "::" + t;

        // Registering the operators
        int r = engine->RegisterObjectMethod(t, ("olc::vi2d opImplConv ()                   const"),         asMETHODPR(olc::v2d_generic<T>, operator olc::vi2d, ()                           const,            olc::vi2d),            asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, ("olc::vu2d opImplConv ()                   const"),         asMETHODPR(olc::v2d_generic<T>, operator olc::vu2d, ()                           const,            olc::vu2d),            asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, ("olc::vf2d opImplConv ()                   const"),         asMETHODPR(olc::v2d_generic<T>, operator olc::vf2d, ()                           const,            olc::vf2d),            asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, ("olc::vd2d opImplConv ()                   const"),         asMETHODPR(olc::v2d_generic<T>, operator olc::vd2d, ()                           const,            olc::vd2d),            asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opSub      (const "+type+"&in)  const").c_str(), asMETHODPR(olc::v2d_generic<T>, operator -,         (const olc::v2d_generic<T>&) const,            olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opMul      (const "+ast+"&in)   const").c_str(), asMETHODPR(olc::v2d_generic<T>, operator *,         (const T&)                   const,            olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opMul      (const "+type+"&in)  const").c_str(), asMETHODPR(olc::v2d_generic<T>, operator *,         (const olc::v2d_generic<T>&) const,            olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opDiv      (const "+ast+"&in)   const").c_str(), asMETHODPR(olc::v2d_generic<T>, operator /,         (const T&)                   const,            olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opDiv      (const "+type+"&in)  const").c_str(), asMETHODPR(olc::v2d_generic<T>, operator /,         (const olc::v2d_generic<T>&) const,            olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opAdd      ()                   const").c_str(), asMETHODPR(olc::v2d_generic<T>, operator +,         ()                           const,            olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
    if constexpr(!std::is_unsigned<T>::value)
        r = engine->RegisterObjectMethod(t, (type+"     opSub      ()                   const").c_str(), asMETHODPR(olc::v2d_generic<T>, operator -,         ()                           const,            olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, ("bool      opEquals   (const olc::vi2d&in) const"),         asMETHODPR(olc::v2d_generic<T>, operator==,         (const olc::v2d_generic<T>&) const,            bool),                 asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opMul_r    (const int32&in)     const").c_str(), asFUNCTIONPR(              olc::operator *,         (const int32_t&,  const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opMul_r    (const uint32&in)    const").c_str(), asFUNCTIONPR(              olc::operator *,         (const uint32_t&, const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opMul_r    (const float&in)     const").c_str(), asFUNCTIONPR(              olc::operator *,         (const float&,    const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opMul_r    (const double&in)    const").c_str(), asFUNCTIONPR(              olc::operator *,         (const double&,   const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opDiv_r    (const int32&in)     const").c_str(), asFUNCTIONPR(              olc::operator /,         (const int32_t&,  const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opDiv_r    (const uint32&in)    const").c_str(), asFUNCTIONPR(              olc::operator /,         (const uint32_t&, const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opDiv_r    (const float&in)     const").c_str(), asFUNCTIONPR(              olc::operator /,         (const float&,    const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"     opDiv_r    (const double&in)    const").c_str(), asFUNCTIONPR(              olc::operator /,         (const double&,   const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"&    opAddAssign(const "+type+"&in)").c_str(),        asMETHODPR(olc::v2d_generic<T>, operator+=,         (const olc::v2d_generic<T>&),                  olc::v2d_generic<T>&), asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"&    opSubAssign(const "+type+"&in)").c_str(),        asMETHODPR(olc::v2d_generic<T>, operator-=,         (const olc::v2d_generic<T>&),                  olc::v2d_generic<T>&), asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"&    opMulAssign(const "+ast+"&in)").c_str(),         asMETHODPR(olc::v2d_generic<T>, operator*=,         (const T&),                                    olc::v2d_generic<T>&), asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"&    opDivAssign(const "+ast+"&in)").c_str(),         asMETHODPR(olc::v2d_generic<T>, operator/=,         (const T&),                                    olc::v2d_generic<T>&), asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"&    opMulAssign(const "+type+"&in)").c_str(),        asMETHODPR(olc::v2d_generic<T>, operator*=,         (const olc::v2d_generic<T>&),                  olc::v2d_generic<T>&), asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"&    opDivAssign(const "+type+"&in)").c_str(),        asMETHODPR(olc::v2d_generic<T>, operator/=,         (const olc::v2d_generic<T>&),                  olc::v2d_generic<T>&), asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"&    opAssign   (const "+type+"&in)").c_str(),        asMETHODPR(olc::v2d_generic<T>, operator =,         (const olc::v2d_generic<T>&),                  olc::v2d_generic<T>&), asCALL_THISCALL); assert(r >= 0);
    
        // Registering the methods
        r = engine->RegisterObjectMethod(t, ("string str   ()                                                                       const"),         asMETHODPR(olc::v2d_generic<T>, str,    ()                                                   const, std::string),          asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (ast+"   mag   ()                                                                       const").c_str(), asMETHODPR(olc::v2d_generic<T>, mag,    ()                                                   const, double),               asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (ast+"   mag2  ()                                                                       const").c_str(), asMETHODPR(olc::v2d_generic<T>, mag2,   ()                                                   const, T),                    asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"  norm  ()                                                                       const").c_str(), asMETHODPR(olc::v2d_generic<T>, norm,   ()                                                   const, olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
    if constexpr (!std::is_unsigned<T>::value)
        r = engine->RegisterObjectMethod(t, (type+"  perp  ()                                                                       const").c_str(), asMETHODPR(olc::v2d_generic<T>, perp,   ()                                                   const, olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"  floor ()                                                                       const").c_str(), asMETHODPR(olc::v2d_generic<T>, floor,  ()                                                   const, olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"  ceil  ()                                                                       const").c_str(), asMETHODPR(olc::v2d_generic<T>, ceil,   ()                                                   const, olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"  max   (const "+type+"&in)                                                      const").c_str(), asMETHODPR(olc::v2d_generic<T>, max,    (const olc::v2d_generic<T>&)                         const, olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"  min   (const "+type+"&in)                                                      const").c_str(), asMETHODPR(olc::v2d_generic<T>, min,    (const olc::v2d_generic<T>&)                         const, olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (ast+"   dot   (const "+type+"&in)                                                      const").c_str(), asMETHODPR(olc::v2d_generic<T>, dot,    (const olc::v2d_generic<T>&)                         const, T),                    asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (ast+"   cross (const "+type+"&in)                                                      const").c_str(), asMETHODPR(olc::v2d_generic<T>, cross,  (const olc::v2d_generic<T>&)                         const, T),                    asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"  clamp (const "+ast+"&in, const "+ast+"&in, const "+ast+"&in, const "+ast+"&in) const").c_str(), asMETHODPR(olc::v2d_generic<T>, clamp,  (const T&, const T&, const T&, const T&)             const, olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"  xclamp(const "+ast+"&in, const "+ast+"&in)                                     const").c_str(), asMETHODPR(olc::v2d_generic<T>, xclamp, (const T&, const T&)                                 const, olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"  yclamp(const "+ast+"&in, const "+ast+"&in)                                     const").c_str(), asMETHODPR(olc::v2d_generic<T>, yclamp, (const T&, const T&)                                 const, olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"& clamp (const "+ast+"&in, const "+ast+"&in, const "+ast+"&in, const "+ast+"&in)").c_str(),       asMETHODPR(olc::v2d_generic<T>, clamp,  (const T&, const T&, const T&, const T&),                   olc::v2d_generic<T>&), asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"& xclamp(const "+ast+"&in, const "+ast+"&in)").c_str(),                                           asMETHODPR(olc::v2d_generic<T>, xclamp, (const T&, const T&),                                       olc::v2d_generic<T>&), asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"& yclamp(const "+ast+"&in, const "+ast+"&in)").c_str(),                                           asMETHODPR(olc::v2d_generic<T>, yclamp, (const T&, const T&),                                       olc::v2d_generic<T>&), asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"  cart  ()").c_str(),                                                                             asMETHODPR(olc::v2d_generic<T>, cart,   (),                                                         olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
        r = engine->RegisterObjectMethod(t, (type+"  polar ()").c_str(),                                                                             asMETHODPR(olc::v2d_generic<T>, polar,  (),                                                         olc::v2d_generic<T>),  asCALL_THISCALL); assert(r >= 0);
    }

    void Engine::Registerallv2d()
    {
        int r = engine->SetDefaultNamespace("olc"); assert(r >= 0);

        Registerv2d_t<int32_t>("vi2d", "int32");
        Registerv2d_t<uint32_t>("vu2d", "uint32");
        Registerv2d_t<float>("vf2d", "float");
        Registerv2d_t<double>("vd2d", "double");

        Registerv2d<int32_t>("vi2d", "int32");
        Registerv2d<uint32_t>("vu2d", "uint32");
        Registerv2d<float>("vf2d", "float");
        Registerv2d<double>("vd2d", "double");

        r = engine->SetDefaultNamespace(""); assert(r >= 0);
    }

    Engine::Engine()
    {
        int r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL); assert(r >= 0);
        RegisterStdString(engine);
        r = engine->RegisterGlobalFunction("void print(const string&in)", asFUNCTION(print), asCALL_CDECL); assert(r >= 0);

        Registerallv2d();
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

    asITypeInfo* Engine::GetType(const char* typedecl, const char* filename)
    {
        return Get().I_GetType(typedecl, Get().I_GetModule(filename));
    }

    asIScriptModule* Engine::GetModule(const char* filename)
    {
        return Get().I_GetModule(filename);
    }

    void Engine::ReleaseObject(asIScriptObject* obj, asITypeInfo* type)
    {
        Get().I_ReleaseObject(obj, type);
    }
}