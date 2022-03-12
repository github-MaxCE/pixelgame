#include "olcPixelGameEngine.h"
#define OLC_PGEX_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"
#include "GameObject.h"
#include "map.h"
#include "Player.h"
#include "Entity.h"
#include <cassert>
#include <thread>
#include <atomic>
#include <chrono>
#include "AssetManager.h"
#include "EventSystem.h"
#include "AngelScriptEngine.h"

class pixelgame : public olc::PixelGameEngine
{
 public:
    pixelgame()
    {
        sAppName = "pixelgame";
    }

    virtual ~pixelgame()
    {
        events.End();
        fixed.join();
        max::DeleteAllEntities();
    }

    max::EventSystem events = max::EventSystem(this);
    max::AssetManager assets;
    max::map* world;
    olc::GFX2D gfx2d;
    std::thread fixed;
    float fx_fLastElapsed;
    std::atomic<bool> gamestate = false;

    void FixedUpdate()
    {
        using namespace std::chrono_literals;

        std::chrono::time_point<std::chrono::system_clock> tp1, tp2;

        while(!gamestate) {}
        while (gamestate)
        {
            // Handle Timing
            tp2 = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsedTime = tp2 - tp1;
            tp1 = tp2;

            // Our time per frame coefficient
            float fElapsedTime = elapsedTime.count();
            fx_fLastElapsed = fElapsedTime;

            for (auto entity : max::Entities)
            {
                entity->FixedUpdate(fElapsedTime);
            }

            std::this_thread::sleep_for(10ms);
        }
    }

    // Called once at the start
    bool OnUserCreate() override
    {
        world = new max::map("map", &assets, &gfx2d, this);
        new max::Player(this, world, &events);

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

        events.Start();

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
        return gamestate == false;
    }
};

int main()
{
    {
        pixelgame pge;
        if (pge.Construct(400, 180, 2, 2))
            pge.Start();
    }

    return 0;
}