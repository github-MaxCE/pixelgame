#include "Entity.h"

namespace max::angelscript
{
    void asEntity::Start()
    {
        if (obj != nullptr)
        {
            max::angelscript::callmethod("void Start()", obj, engine);
        }
    }

    void asEntity::FixedUpdate(float fElapsedTime)
    {
        if (obj != nullptr)
        {
            max::angelscript::callmethod("void FixedUpdate(float fElapsedTime)", obj, engine, fElapsedTime);
        }
    }

    void asEntity::Update(float fElapsedTime)
    {
        if (obj != nullptr)
        {
            max::angelscript::callmethod("void Update(float fElapsedTime)", obj, engine, fElapsedTime);
        }
    }

    void asEntity::End()
    {
        if (obj != nullptr)
        {
            max::angelscript::callmethod("void End()", obj, engine);
            engine->ReleaseScriptObject(obj, type);
        }
    }
}