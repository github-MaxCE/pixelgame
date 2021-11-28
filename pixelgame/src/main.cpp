#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Gameobject.h"
#include "worldmanager.h"
#include "Player.h"
#define OLC_PGE_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"

class pixelgame : public olc::PixelGameEngine
{
    public: pixelgame()
    {
        sAppName = "pixelgame";
    }
    Player *player = new Player();


    // Called once at the start
    public: bool OnUserCreate() override
    {
        loadmap("map.xml");
        return true;
    }

    double fixedupdate = 0;
    // called once per frame
    public: bool OnUserUpdate(double dElapsedTime) override
    {
        fixedupdate += dElapsedTime;
        if (fixedupdate >= 0.0166)
        {
            player->FixedUpdate(fixedupdate);
            fixedupdate = 0;
        }
        player->Update(dElapsedTime, this);
        Clear(olc::BLACK);
        for (auto const &i : Gameobjects)
        {
            for (auto const &x : i)
            {
                x->Render(this);
            }
        }

        return true;
    }
    public: bool OnUserDestroy() override
    {
        return true;
    }
} pge = pixelgame();

int main()
{
    if (pge.Construct(400, 180, 2, 2, false, true))
        pge.Start();

    return 0;
}
