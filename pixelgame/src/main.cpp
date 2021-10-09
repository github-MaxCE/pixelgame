#define OLC_PGE_APPLICATION
#define OLC_PGEX_FUI
#include "Engine.h"
#include "Player.h"

class pixelgame : virtual public olc::PixelGameEngine
{
	public: pixelgame()
	{
		sAppName = "pixelgame";
	}

 	Player player;
	map *mmap = new map("map.xml");

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
		if(fixedupdate >= 0.0166)
		{
			player.FixedUpdate(fixedupdate, this, mmap);
			fixedupdate = 0;
		}
		player.Update(dElapsedTime, this);
		Clear(olc::BLACK);
		for (auto i : mmap->mapelements)
		{
			for (auto &x : *i)
			{
				if(dynamic_cast<map::ImageTile*>(x.get()) != nullptr)
				{
					auto* a = dynamic_cast<map::ImageTile*>(x.get());
					DrawSprite(a->pos, a->sprite);
					break;
				}
				if(dynamic_cast<map::RectTile*>(x.get()) != nullptr)
				{
					auto* b = dynamic_cast<map::RectTile*>(x.get());
					DrawRect(b->pos, b->size, b->col);
					break;
				}
				if(dynamic_cast<map::FilledRectTile*>(x.get()) != nullptr)
				{
					auto* c = dynamic_cast<map::FilledRectTile*>(x.get());
					FillRect(c->pos, c->size, c->col);
					break;
				}
			}
		}
		DrawRect(player.pos, player.size);
		return true;
	}

	public: bool OnUserDestroy() override
	{
		for (auto const& i : mmap->mapelements)
		{
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
