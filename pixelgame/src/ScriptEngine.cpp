#include "ScriptEngine.h"

namespace max::script
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
    static void Constructor(void* self, Args&&... args)
    {
        new(self) T(std::forward<Args>(args)...);
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
        printf("%s", str.c_str(), asCALL_THISCALL);
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
        asIScriptModule* smod = engine->GetModule(filename);
        return smod == nullptr ? BuildScript(filename) : smod;
    }

    void Engine::I_ReleaseObject(asIScriptObject* obj)
    {
        engine->ReleaseScriptObject(obj, obj->GetObjectType());
    }
    
    template<typename T>
    void Engine::Registerv2d(const char* t, std::string ast, const char* ns)
    {
        std::string type;
        type = type + ns + "::" + t;
    
        auto registermethod = [engine = this->engine, t](std::string decl, const asSFuncPtr& funcptr, asDWORD callconv)
        {
            int r = engine->RegisterObjectMethod(t, decl.c_str(), funcptr, callconv); assert(r >= 0);
        };

        int r = engine->SetDefaultNamespace(ns); assert(r >= 0);
    
        // Register a primitive type, that doesn't need any special management of the content
        r = engine->RegisterObjectType(t, sizeof(olc::v2d_generic<T>), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<olc::v2d_generic<T>>()); assert(r >= 0);
    
        // Register the object properties
        r = engine->RegisterObjectProperty(t, (ast + " x").c_str(), asOFFSET(olc::v2d_generic<T>, x)); assert(r >= 0);
        r = engine->RegisterObjectProperty(t, (ast + " y").c_str(), asOFFSET(olc::v2d_generic<T>, y)); assert(r >= 0);
    
        // Register the constructors
        r = engine->RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f()",                                                     asFUNCTION(Constructor_def<olc::v2d_generic<T>>),                   asCALL_CDECL_OBJFIRST); assert(r >= 0);
        r = engine->RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, ("void f(const " + type + "& in)").c_str(),                     asFUNCTION(Constructor_copy<olc::v2d_generic<T>>),                  asCALL_CDECL_OBJFIRST); assert(r >= 0);
        r = engine->RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, ("void f(const " + ast + "&in, const " + ast + "&in)").c_str(), asFUNCTION((Constructor<olc::v2d_generic<T>, const T&, const T&>)), asCALL_CDECL_OBJFIRST); assert(r >= 0);
    
        // Registering the operators
        registermethod("olc::vi2d opImplConv ()                   const", asMETHODPR(olc::v2d_generic<T>, operator olc::vi2d, ()                           const,            olc::vi2d),            asCALL_THISCALL);
        registermethod("olc::vu2d opImplConv ()                   const", asMETHODPR(olc::v2d_generic<T>, operator olc::vu2d, ()                           const,            olc::vu2d),            asCALL_THISCALL);
        registermethod("olc::vf2d opImplConv ()                   const", asMETHODPR(olc::v2d_generic<T>, operator olc::vf2d, ()                           const,            olc::vf2d),            asCALL_THISCALL);
        registermethod("olc::vd2d opImplConv ()                   const", asMETHODPR(olc::v2d_generic<T>, operator olc::vd2d, ()                           const,            olc::vd2d),            asCALL_THISCALL);
        registermethod(type+"     opSub      (const "+type+"&in)  const", asMETHODPR(olc::v2d_generic<T>, operator -,         (const olc::v2d_generic<T>&) const,            olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(type+"     opMul      (const "+ast+"&in)   const", asMETHODPR(olc::v2d_generic<T>, operator *,         (const T&)                   const,            olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(type+"     opMul      (const "+type+"&in)  const", asMETHODPR(olc::v2d_generic<T>, operator *,         (const olc::v2d_generic<T>&) const,            olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(type+"     opDiv      (const "+ast+"&in)   const", asMETHODPR(olc::v2d_generic<T>, operator /,         (const T&)                   const,            olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(type+"     opDiv      (const "+type+"&in)  const", asMETHODPR(olc::v2d_generic<T>, operator /,         (const olc::v2d_generic<T>&) const,            olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(type+"     opAdd      ()                   const", asMETHODPR(olc::v2d_generic<T>, operator +,         ()                           const,            olc::v2d_generic<T>),  asCALL_THISCALL);
    if constexpr(!std::is_unsigned<T>::value)
        registermethod(type+"     opSub      ()                   const", asMETHODPR(olc::v2d_generic<T>, operator -,         ()                           const,            olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod("bool      opEquals   (const olc::vi2d&in) const", asMETHODPR(olc::v2d_generic<T>, operator==,         (const olc::v2d_generic<T>&) const,            bool),                 asCALL_THISCALL);
        registermethod(type+"     opMul_r    (const int32&in)     const", asFUNCTIONPR(              olc::operator *,         (const int32_t&,  const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST);
        registermethod(type+"     opMul_r    (const uint32&in)    const", asFUNCTIONPR(              olc::operator *,         (const uint32_t&, const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST);
        registermethod(type+"     opMul_r    (const float&in)     const", asFUNCTIONPR(              olc::operator *,         (const float&,    const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST);
        registermethod(type+"     opMul_r    (const double&in)    const", asFUNCTIONPR(              olc::operator *,         (const double&,   const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST);
        registermethod(type+"     opDiv_r    (const int32&in)     const", asFUNCTIONPR(              olc::operator /,         (const int32_t&,  const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST);
        registermethod(type+"     opDiv_r    (const uint32&in)    const", asFUNCTIONPR(              olc::operator /,         (const uint32_t&, const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST);
        registermethod(type+"     opDiv_r    (const float&in)     const", asFUNCTIONPR(              olc::operator /,         (const float&,    const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST);
        registermethod(type+"     opDiv_r    (const double&in)    const", asFUNCTIONPR(              olc::operator /,         (const double&,   const olc::v2d_generic<T>&), olc::v2d_generic<T>),  asCALL_CDECL_OBJLAST);
        registermethod(type+"&    opAddAssign(const "+type+"&in)",        asMETHODPR(olc::v2d_generic<T>, operator+=,         (const olc::v2d_generic<T>&),                  olc::v2d_generic<T>&), asCALL_THISCALL);
        registermethod(type+"&    opSubAssign(const "+type+"&in)",        asMETHODPR(olc::v2d_generic<T>, operator-=,         (const olc::v2d_generic<T>&),                  olc::v2d_generic<T>&), asCALL_THISCALL);
        registermethod(type+"&    opMulAssign(const "+ast+"&in)",         asMETHODPR(olc::v2d_generic<T>, operator*=,         (const T&),                                    olc::v2d_generic<T>&), asCALL_THISCALL);
        registermethod(type+"&    opDivAssign(const "+ast+"&in)",         asMETHODPR(olc::v2d_generic<T>, operator/=,         (const T&),                                    olc::v2d_generic<T>&), asCALL_THISCALL);
        registermethod(type+"&    opMulAssign(const "+type+"&in)",        asMETHODPR(olc::v2d_generic<T>, operator*=,         (const olc::v2d_generic<T>&),                  olc::v2d_generic<T>&), asCALL_THISCALL);
        registermethod(type+"&    opDivAssign(const "+type+"&in)",        asMETHODPR(olc::v2d_generic<T>, operator/=,         (const olc::v2d_generic<T>&),                  olc::v2d_generic<T>&), asCALL_THISCALL);
        registermethod(type+"&    opAssign   (const "+type+"&in)",        asMETHODPR(olc::v2d_generic<T>, operator =,         (const olc::v2d_generic<T>&),                  olc::v2d_generic<T>&), asCALL_THISCALL);
    
        // Registering the methods
        registermethod("string str   ()                                                                       const", asMETHODPR(olc::v2d_generic<T>, str,    ()                                                   const, std::string),          asCALL_THISCALL);
        registermethod(ast+"   mag   ()                                                                       const", asMETHODPR(olc::v2d_generic<T>, mag,    ()                                                   const, double),               asCALL_THISCALL);
        registermethod(ast+"   mag2  ()                                                                       const", asMETHODPR(olc::v2d_generic<T>, mag2,   ()                                                   const, T),                    asCALL_THISCALL);
        registermethod(type+"  norm  ()                                                                       const", asMETHODPR(olc::v2d_generic<T>, norm,   ()                                                   const, olc::v2d_generic<T>),  asCALL_THISCALL);
    if constexpr (!std::is_unsigned<T>::value)
        registermethod(type+"  perp  ()                                                                       const", asMETHODPR(olc::v2d_generic<T>, perp,   ()                                                   const, olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(type+"  floor ()                                                                       const", asMETHODPR(olc::v2d_generic<T>, floor,  ()                                                   const, olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(type+"  ceil  ()                                                                       const", asMETHODPR(olc::v2d_generic<T>, ceil,   ()                                                   const, olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(type+"  max   (const "+type+"&in)                                                      const", asMETHODPR(olc::v2d_generic<T>, max,    (const olc::v2d_generic<T>&)                         const, olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(type+"  min   (const "+type+"&in)                                                      const", asMETHODPR(olc::v2d_generic<T>, min,    (const olc::v2d_generic<T>&)                         const, olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(ast+"   dot   (const "+type+"&in)                                                      const", asMETHODPR(olc::v2d_generic<T>, dot,    (const olc::v2d_generic<T>&)                         const, T),                    asCALL_THISCALL);
        registermethod(ast+"   cross (const "+type+"&in)                                                      const", asMETHODPR(olc::v2d_generic<T>, cross,  (const olc::v2d_generic<T>&)                         const, T),                    asCALL_THISCALL);
        registermethod(type+"  clamp (const "+ast+"&in, const "+ast+"&in, const "+ast+"&in, const "+ast+"&in) const", asMETHODPR(olc::v2d_generic<T>, clamp,  (const T&, const T&, const T&, const T&)             const, olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(type+"  xclamp(const "+ast+"&in, const "+ast+"&in)                                     const", asMETHODPR(olc::v2d_generic<T>, xclamp, (const T&, const T&)                                 const, olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(type+"  yclamp(const "+ast+"&in, const "+ast+"&in)                                     const", asMETHODPR(olc::v2d_generic<T>, yclamp, (const T&, const T&)                                 const, olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(type+"& clamp (const "+ast+"&in, const "+ast+"&in, const "+ast+"&in, const "+ast+"&in)",       asMETHODPR(olc::v2d_generic<T>, clamp,  (const T&, const T&, const T&, const T&),                   olc::v2d_generic<T>&), asCALL_THISCALL);
        registermethod(type+"& xclamp(const "+ast+"&in, const "+ast+"&in)",                                           asMETHODPR(olc::v2d_generic<T>, xclamp, (const T&, const T&),                                       olc::v2d_generic<T>&), asCALL_THISCALL);
        registermethod(type+"& yclamp(const "+ast+"&in, const "+ast+"&in)",                                           asMETHODPR(olc::v2d_generic<T>, yclamp, (const T&, const T&),                                       olc::v2d_generic<T>&), asCALL_THISCALL);
        registermethod(type+"  cart  ()",                                                                             asMETHODPR(olc::v2d_generic<T>, cart,   (),                                                         olc::v2d_generic<T>),  asCALL_THISCALL);
        registermethod(type+"  polar ()",                                                                             asMETHODPR(olc::v2d_generic<T>, polar,  (),                                                         olc::v2d_generic<T>),  asCALL_THISCALL);
    }

    Engine::Engine()
    {
        int r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL); assert(r >= 0);
        RegisterStdString(engine);
        r = engine->RegisterGlobalFunction("void print(const string&in)", asFUNCTION(print), asCALL_CDECL); assert(r >= 0);

        Registerv2d<int32_t> ("vi2d", "int32", "olc");
        Registerv2d<uint32_t>("vu2d", "uint32", "olc");
        Registerv2d<float>("vf2d", "float", "olc");
        Registerv2d<double>("vd2d", "double", "olc");
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
