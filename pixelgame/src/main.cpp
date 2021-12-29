#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"
#include "Gameobject.h"
#include "worldmanager.h"
#include "Player.h"
#include <charconv>
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

namespace lua
{
    int print(lua_State* L)
    {
        std::cout << luaL_checkstring(L, 1);
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
    lua_State* L = luaL_newstate();

    // Called once at the start
    public: bool OnUserCreate() override
    {
        luaL_openlibs(L);
        lua_register(L, "print", lua::print);

        if(!loadmap("map.xml", &gfx2d, this, L)) return false;
        
        worldsize = olc::vi2d(0, 0);
        for (auto x : Gameobjects[1])
            if (x->pos + x->size > worldsize)
                worldsize = x->pos + x->size;

        if (!worldsize) return false;

        return true;
    }

    double fixedupdate = 0;
    // called once per frame
    public: bool OnUserUpdate(double dElapsedTime) override
    {
        Clear(olc::BLACK);

        fixedupdate += dElapsedTime;
        if (fixedupdate >= 0.0166)
        {
            player->FixedUpdate(fixedupdate);
            fixedupdate = 0;
        }
        player->Update(dElapsedTime);
        player->camera->Update(dElapsedTime);

        return true;
    }
    public: bool OnUserDestroy() override
    {
        return true;
    }
};


int main()
{
    pixelgame pge;
    if (pge.Construct(400, 180, 2, 2, false, true))
        pge.Start();

    return 0;
}