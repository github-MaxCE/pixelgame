#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"
#include "Gameobject.h"
#include "worldmanager.h"
#include "Player.h"

class pixelgame : public olc::PixelGameEngine
{
    public: pixelgame()
    {
        sAppName = "pixelgame";
    }
    Player *player = new Player();
    olc::GFX2D gfx2d;

    // Called once at the start
    public: bool OnUserCreate() override
    {
        loadmap("map.xml", gfx2d);
        worldsize = olc::vi2d(0, 0);
        for (auto x : Gameobjects[1])
        {
            if (x->pos.x + x->size.x > worldsize.x) {
                worldsize.x = x->pos.x + x->size.x;
            }
            if (x->pos.y + x->size.y > worldsize.y) {
               worldsize.y = x->pos.y + x->size.y;
            }
        }
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
            player->FixedUpdate(fixedupdate, this);
            fixedupdate = 0;
        }
        player->Update(dElapsedTime, this);
        player->camera->Update(dElapsedTime, this);

        DrawRect(worldsize + worldoffset, olc::vi2d(5, 5), olc::DARK_MAGENTA);
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

