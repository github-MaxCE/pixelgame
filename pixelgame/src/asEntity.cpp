#include "asEntity.h"

namespace max::angelscript
{
    Entity::~Entity()
    {
    }

    void Entity::Start()
    {
        if (obj != nullptr)
        {
            max::angelscript::callmethod("void Start()", obj, type, engine);
        }
    }

    void Entity::FixedUpdate(float fElapsedTime)
    {
        if (obj != nullptr)
        {
            max::angelscript::callmethod("void FixedUpdate(float fElapsedTime)", obj, type, engine, fElapsedTime);
        }
    }

    void Entity::Update(float fElapsedTime)
    {
        if (obj != nullptr)
        {
            max::angelscript::callmethod("void Update(float fElapsedTime)", obj, type, engine, fElapsedTime);
        }
    }

    void Entity::End()
    {
        if (obj != nullptr)
        {
            max::angelscript::callmethod("void End()", obj, type, engine);
        }
        engine->ReleaseScriptObject(obj, obj->GetObjectType());
    }
}