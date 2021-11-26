#define OLC_PGE_APPLICATION
#define OLC_PGEX_FUI
#include "worldmanager.h"
#include "olcPixelGameEngine.h"
#include "Player.h"
#include "Gameobject.h"

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
}* pge = new pixelgame;

int main()
{
    if (pge->Construct(400, 180, 2, 2, false, true))
        pge->Start();

    return 0;
}
