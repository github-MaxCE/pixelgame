#define OLC_PGE_APPLICATION
#define OLC_PGEX_FUI
#include "Engine.h"
#include "Player.h"

class pixelgame : public olc::PixelGameEngine
{
	public: pixelgame()
	{
		sAppName = "pixelgame";
	}
	
	map *mmap = new map("map.xml");
	Player *player = new Player(mmap);

	// Called once at the start
	public: bool OnUserCreate() override
	{
		return true;
	}

	double fixedupdate = 0;
	// called once per frame
	public: bool OnUserUpdate(double dElapsedTime) override
	{
		fixedupdate += dElapsedTime;
		if (fixedupdate >= 0.0166)
		{
			player->FixedUpdate(fixedupdate, mmap);
			fixedupdate = 0;
		}
		player->Update(dElapsedTime, this);
		Clear(olc::BLACK);
		for (auto const &i : mmap->mapelements)
		{
			for (auto const &x : *i)
			{
				x->Draw(this);
			}
		}
		return true;
	}

	public: bool OnUserDestroy() override
	{
		for (auto const &i : mmap->mapelements)
		{
			for (auto const &x : *i)
			{
				delete x;
			}
			i->clear();
		}
		return true;
	}
};

int main()
{
	pixelgame Game;
	if (Game.Construct(400, 180, 2, 2, false, true))
		Game.Start();

	return 0;
}
