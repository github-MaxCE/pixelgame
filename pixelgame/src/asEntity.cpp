#include "Entity.h"

namespace max::angelscript
{
    void Entity::Start()
    {
        if (obj != nullptr)
        {
            max::angelscript::callmethod("void Start()", obj, engine);
        }
    }

    void Entity::FixedUpdate(float fElapsedTime)
    {
        if (obj != nullptr)
        {
            max::angelscript::callmethod("void FixedUpdate(float fElapsedTime)", obj, engine, fElapsedTime);
        }
    }

    void Entity::Update(float fElapsedTime)
    {
        if (obj != nullptr)
        {
            max::angelscript::callmethod("void Update(float fElapsedTime)", obj, engine, fElapsedTime);
        }
    }

    void Entity::End()
    {
        if (obj != nullptr)
        {
            max::angelscript::callmethod("void End()", obj, engine);
            engine->ReleaseScriptObject(obj, type);
        }
    }
}