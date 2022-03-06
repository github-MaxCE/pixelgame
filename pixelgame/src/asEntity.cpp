#include "asEntity.h"

namespace max::angelscript
{
    void Entity::Start()
    {
        call("void Start()");
    }

    void Entity::FixedUpdate(float fElapsedTime)
    {
        call("void FixedUpdate(float fElapsedTime)", fElapsedTime);
    }

    void Entity::Update(float fElapsedTime)
    {
        call("void Update(float fElapsedTime)", fElapsedTime);
    }

    void Entity::End()
    {
        call("void End()");
    }
}