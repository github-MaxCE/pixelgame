#include "AngelScriptObject.h"

namespace max::angelscript
{
    object::~object()
    {
        max::angelscript::Engine::ReleaseObject(obj);
    }
}