#define OLC_PGE_APPLICATION
#define OLC_PGEX_GRAPHICS2D
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Gameobject.h"
#include "Entity.h"
#include "worldmanager.h"
#include "Player.h"
#include "angelscript/angelscript.h"
#include "angelscript/scriptstdstring/scriptstdstring.h"
#include "angelscript/scriptbuilder/scriptbuilder.h"
#include "AngelScriptutil.h"
#include <cassert>
#include <thread>
#include <chrono>

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
    asIScriptContext* ctx;
    std::thread fixed;
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

            for (auto entity : Entities)
            {
                entity->FixedUpdate(fx_fElapsedTime);
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

        new max::angelscript::asEntity("maps/map.as", "map", engine, &builder, ctx, true);

        if(!loadmap("map.xml", &gfx2d, this, engine, &builder, ctx)) gamestate = false;

        for (const auto& x : Gameobjects[1])
        {
            if (x->pos + x->size > worldsize)
            {
                worldsize = x->pos + x->size;
            }
        }

        if (!worldsize) return false;


        for (auto entity : Entities)
        {
            entity->Start();
        }

        fixed = std::thread(&pixelgame::FixedUpdate, this);

        return gamestate;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        for (auto entity : Entities)
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

        for (auto entity : Entities)
        {
            entity->End();
        }

        fixed.join();
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