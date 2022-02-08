#include "AngelScriptUtils.h"

namespace max::angelscript
{
    asIScriptModule* BuildScript(const char* filename, CScriptBuilder* builder, asIScriptEngine* engine)
    {
        int r = builder->StartNewModule(engine, filename);
        if (r < 0)
        {
            printf("Unrecoverable error while starting a new module.\n");
            return nullptr;
        }
        r = builder->AddSectionFromFile((max::scriptpath() + filename).c_str());
        if (r < 0)
        {
            printf("Please correct the errors in the script and try again.\n");
            return nullptr;
        }
        r = builder->BuildModule();
        if (r < 0)
        {
            printf("Please correct the errors in the script and try again.\n");
            return nullptr;
        }

        return engine->GetModule(filename);
    }

    asITypeInfo* GetType(asIScriptModule* mod, const char* objdecl)
    {
        // Get the object type
        return mod->GetTypeInfoByDecl(objdecl);
    }
}