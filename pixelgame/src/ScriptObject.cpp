#include "ScriptObject.h"

namespace max::script
{
    object::~object()
    {
        Engine::ReleaseObject(obj);
    }
}