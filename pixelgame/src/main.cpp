#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"
#include "Gameobject.h"
#include "Entity.h"
#include "worldmanager.h"
#include "Player.h"
#include "angelscript/angelscript.h"
#include "angelscript/scriptstdstring/scriptstdstring.h"
#include "angelscript/scriptbuilder/scriptbuilder.h"
#include <cassert>
#include <thread>

namespace max::angelscript
{
    void MessageCallback(const asSMessageInfo* msg, void* param)
    {
        const char* type = "ERR ";
        if (msg->type == asMSGTYPE_WARNING)
            type = "WARN";
        else if (msg->type == asMSGTYPE_INFORMATION)
            type = "INFO";
        printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
    }

    void print(const std::string& str)
    {
        printf("%s", str.c_str());
    }
}

class pixelgame : public olc::PixelGameEngine
{
 public:
    pixelgame()
    {
        sAppName = "pixelgame";
    }

    Player *player = new Player(this);
    olc::GFX2D gfx2d;
    asIScriptEngine* engine = asCreateScriptEngine();
    CScriptBuilder builder;
    asIScriptObject* map;
    asIScriptContext* ctx;
    std::thread fixed = std::thread(&pixelgame::FixedUpdate, this);
    std::chrono::time_point<std::chrono::system_clock> fx_tp1, fx_tp2;
    float fx_fLastElapsed;
    bool gamestate = true;

    void FixedUpdate()
    {
        while (true)
        {
            if (!gamestate) break;
            // Handle Timing
            fx_tp2 = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsedTime = fx_tp2 - fx_tp1;
            fx_tp1 = fx_tp2;

            // Our time per frame coefficient
            float fx_fElapsedTime = elapsedTime.count();
            fx_fLastElapsed = fx_fElapsedTime;

            for (const auto& entity : Entities)
            {
                entity->FixedUpdate(fx_fElapsedTime);
            }

            if (map != 0)
            {
                asIScriptFunction* func = map->GetObjectType()->GetMethodByDecl("void OnMapFixedUpdate()");
                if (func != 0)
                {
                    // Create our context, prepare it, and then execute
                    ctx = engine->CreateContext();

                    ctx->Prepare(func);

                    ctx->SetObject(map);

                    int r = ctx->Execute();
                    if (r != asEXECUTION_FINISHED)
                    {
                        if (r == asEXECUTION_EXCEPTION)
                        {
                            printf("An exception '%s' occurred. Please correct the code and try again.\n", ctx->GetExceptionString());
                        }
                    }

                    // Clean up
                    ctx->Release();
                }
            }
            std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(10));
        }
    }

    // Called once at the start
    bool OnUserCreate() override
    {
        int r = engine->SetMessageCallback(asFUNCTION(max::angelscript::MessageCallback), 0, asCALL_CDECL); assert(r >= 0);
        RegisterStdString(engine);
        r = engine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(max::angelscript::print), asCALL_CDECL); assert(r >= 0);


        r = builder.StartNewModule(engine, "main");
        if (r < 0) printf("Unrecoverable error while starting a new module.\n");
        // Get the object type

        r = builder.AddSectionFromFile((scriptpath() + "maps/map.as").c_str());
        if (r < 0) printf("Please correct the errors in the script and try again.\n");

        r = builder.BuildModule();
        if (r < 0) printf("Please correct the errors in the script and try again.\n");

        // Get the object type
        asIScriptModule* module = engine->GetModule("main");
        asITypeInfo* type = module->GetTypeInfoByDecl("map");
        if (type != 0)
        {
            // Get the factory function from the object type
            asIScriptFunction* factory = type->GetFactoryByDecl("map @map()");


            ctx = engine->CreateContext();

            // Prepare the context to call the factory function
            ctx->Prepare(factory);

            // Execute the call
            ctx->Execute();

            // Get the object that was created
            map = *(asIScriptObject**)ctx->GetAddressOfReturnValue();

            // If you're going to store the object you must increase the reference,
            // otherwise it will be destroyed when the context is reused or destroyed.
            map->AddRef();

            ctx->Release();
        }

        if(!loadmap("map.xml", &gfx2d, this, engine, &builder, ctx, map)) gamestate = false;


        for (auto x : Gameobjects[1])
            if (x->pos + x->size > worldsize)
                worldsize = x->pos + x->size;

        if (!worldsize) gamestate = false;

        return gamestate;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);
        

        for (const auto& entity : Entities)
        {
            entity->Update(fElapsedTime);
        }
        
        if (GetKey( (olc::Key)0 ).bHeld)
            if (GetKey( olc::Key::F4 ).bPressed)
                gamestate = false;

        return gamestate;
    }
    public: bool OnUserDestroy() override
    {
        gamestate = false;
        if (map != nullptr)
        {
            asIScriptFunction* func = map->GetObjectType()->GetMethodByDecl("void OnMapEnd()");
            if (func == 0) goto noend;

            // Create our context, prepare it, and then execute
            ctx = engine->CreateContext();

            ctx->Prepare(func);

            ctx->SetObject(map);

            int r = ctx->Execute();
            if (r != asEXECUTION_FINISHED)
            {
                if (r == asEXECUTION_EXCEPTION)
                {
                    printf("An exception '%s' occurred. Please correct the code and try again.\n", ctx->GetExceptionString());
                }
            }

            // Clean up
            ctx->Release();
        }
noend:

        fixed.join();
        if (map != 0)engine->ReleaseScriptObject(map, map->GetObjectType());
        engine->ShutDownAndRelease();
        return !gamestate;
    }
};


int main()
{
    pixelgame pge;
    if (pge.Construct(400, 180, 2, 2, false, false))
        pge.Start();

    return 0;
}