#include "olcPixelGameEngine.h"
#include "GameObject.h"
#include "map.h"
#include "Player.h"
#include "Entity.h"
#include <cassert>
#include <thread>
#include <atomic>
#include <chrono>
#include "AssetManager.h"
#include "ScriptEngine.h"

constexpr static uint64_t _stoi(const char* str, size_t size)
{
    uint64_t res = 0;
    for (size_t i = 0; i < size; i++)
    {
        res += str[i];
    }
    return res;
}

static uint64_t _stoi(const char* str)
{
    return _stoi(str, strlen(str));
}

constexpr static uint64_t operator""_stoi(const char* str, size_t size)
{
    return _stoi(str, size);
}

class pixelgame : public olc::PixelGameEngine
{
 public:
    pixelgame()
    {
        sAppName = "pixelgame";
    }

    virtual ~pixelgame()
    {
        fixed.join();
        max::DeleteAllEntities();
    }

    max::map* world;
    olc::GFX2D gfx2d;
    std::thread fixed;
    std::atomic<bool> gamestate = false;

    void FixedUpdate()
    {
        using namespace std::chrono_literals;

        std::chrono::time_point<std::chrono::system_clock> tp1, tp2;
        float fLastElapsed;

        while(!gamestate) {}
        while(gamestate)
        {
            // Handle Timing
            tp2 = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsedTime = tp2 - tp1;
            tp1 = tp2;

            // Our time per frame coefficient
            float fElapsedTime = elapsedTime.count();
            fLastElapsed = fElapsedTime;

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
        world = new max::map("map", &gfx2d, this);
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

int main(int argc, const char** argv)
{
    uint8_t flags = 0;
    const char* map = "map";

    if (argc > 0)
        for (int curarg = 0; curarg < argc; ++curarg)
            switch (_stoi(argv[curarg]))
            {
            case "-fs"_stoi:case "-fullscreen"_stoi: flags |= olc::FLAGS::FULLSCREEN;
            break;
            case "-vs"_stoi:case "-vsync"_stoi:      flags |= olc::FLAGS::VSYNC;
            break;
            case "-map"_stoi:
                map = argv[++curarg];
                break;
            }

    {
        pixelgame pge;
        if (pge.Construct(400, 180, 2, 2, flags))
            pge.Start();
    }

    return 0;
}