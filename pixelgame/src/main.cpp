#include "olcPixelGameEngine.h"
#define OLC_PGEX_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"
#include "GameObject.h"
#include "map.h"
#include "Player.h"
#include "Entity.h"
#include "AngelScriptUtils.h"
#include "angelscript/angelscript.h"
#include "angelscript/scriptstdstring/scriptstdstring.h"
#include "angelscript/scriptbuilder/scriptbuilder.h"
#include <cassert>
#include <thread>
#include <atomic>
#include <chrono>
#include "AssetManager.h"

class pixelgame : public olc::PixelGameEngine
{
 public:
    pixelgame()
    {
        sAppName = "pixelgame";
    }

    max::AssetManager assets;
    max::map* world;
    olc::GFX2D gfx2d;
    asIScriptEngine* engine = asCreateScriptEngine();
    CScriptBuilder builder;
    std::thread fixed;
    std::chrono::time_point<std::chrono::system_clock> fx_tp1, fx_tp2;
    float fx_fLastElapsed;
    std::atomic<bool> gamestate = false;

    void FixedUpdate()
    {
        while(!gamestate) {}
        while (gamestate)
        {
            // Handle Timing
            fx_tp2 = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsedTime = fx_tp2 - fx_tp1;
            fx_tp1 = fx_tp2;

            // Our time per frame coefficient
            float fx_fElapsedTime = elapsedTime.count();
            fx_fLastElapsed = fx_fElapsedTime;

            for (auto entity : max::Entities)
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

        world = new max::map("map", &assets, &gfx2d, this, engine, &builder);
        new max::Player(this, world);

        for (const auto& x : world->GameObjects[1])
        {
            if (x->pos + x->size > world->size)
            {
                world->size = x->pos + x->size;
            }
        }

        if (!world->size) return false;


        for (auto entity : max::Entities)
        {
            entity->Start();
        }

        fixed = std::thread(&pixelgame::FixedUpdate, this);

        return true;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        for (auto& entity : max::Entities)
        {
            entity->Update(fElapsedTime);
        }

        gamestate = true;
        
        if (GetKey( (olc::Key)0 ).bHeld)
            if (GetKey( olc::Key::F4 ).bPressed)
            {
                gamestate = false;
                fixed.join();
            }

        return gamestate;
    }
    
    bool OnUserDestroy() override
    {
        gamestate = false;

        for (auto entity : max::Entities)
        {
            entity->End();
        }
        
        engine->ShutDownAndRelease();
        return gamestate == false;
    }
};

int main()
{
    pixelgame pge;
    if (pge.Construct(400, 180, 2, 2))
        pge.Start();

    return 0;
}