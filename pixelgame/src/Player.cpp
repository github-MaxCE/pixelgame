#include "Player.h"

Player::Player()
{
    player = new Rectobj(3, olc::vi2d(15, 10), olc::vi2d(20, 30), olc::CYAN, "player", false, false);
    camera = new Camera(this);
    this->isGrounded = false;
    this->canWalkr = true;
    this->canWalkl = true;
    this->g = 8.0f;
    this->speed = 5.0f;
    this->jump = 50.0f;
}

Player::~Player()
{}

bool inRange(int low, int high, int x)
{
    return ((unsigned)(x-low) <= (high-low));
}

bool PointInsideRect(const olc::vi2d &point, const olc::vi2d &rectPos, const olc::vi2d &rectSize)
{
    return (point.x >= rectPos.x) &&
           (point.y >= rectPos.y) &&
           (point.x <= rectPos.x + rectSize.x) &&
           (point.y <= rectPos.y + rectSize.y);
}
//FixedUpdate() called 60 times a second, good for physics
//code for calling it regardless of fps
//	double fixedupdate = 0;
//	public: bool OnUserUpdate(double dElapsedTime) override
//	{
//		fixedupdate += dElapsedTime;
//		if(fixedupdate >= 0.0166)
//		{
//			player.Update(fixedupdate, this);
//			fixedupdate = 0;
//		}

void Player::FixedUpdate(double dElapsedTime, olc::PixelGameEngine* pge)
{
    isGrounded = false;
    canWalkr = true;
    canWalkl = true;

    vel.y += g;
    vel.clamp(-speed, speed, -jump, g);
    player->pos += vel;
    vel = {0, vel.y};
    for (auto const& i : Gameobjects[1])
    {
        if (inRange(top0.x   - 200, bottom0.x + 200, i->pos.x + worldoffset.x) ||
            inRange(edgel1.y - 200, edgel1.y  + 200, i->pos.y + worldoffset.y))
        {
            if((inRange(i->pos.x+worldoffset.x, i->pos.x+worldoffset.x+i->size.x, top0.x) ||
                inRange(i->pos.x+worldoffset.x, i->pos.x+worldoffset.x+i->size.x, top2.x)) &&

                inRange(i->pos.y+worldoffset.y + i->size.y , INT32_MAX, top0.y))
            {
                player->pos.yclamp(i->pos.y+worldoffset.y + i->size.y+1, INT32_MAX);
                vel.y = g;
            }

            if((inRange(i->pos.x+worldoffset.x, i->pos.x+worldoffset.x+i->size.x, bottom0.x) ||
                inRange(i->pos.x+worldoffset.x, i->pos.x+worldoffset.x+i->size.x, bottom2.x)) &&

                inRange(INT32_MIN ,i->pos.y+worldoffset.y + 4, bottom0.y))
            {
                if(PointInsideRect(bottom0, i->pos+worldoffset, i->size) ||
                   PointInsideRect(bottom2, i->pos+worldoffset, i->size))
                {
                    isGrounded = true;
                }
                player->pos.yclamp(INT32_MIN, i->pos.y+worldoffset.y - player->size.y-1);
            }
            if(inRange(i->pos.y+worldoffset.y, i->pos.y+worldoffset.y + i->size.y, edger0.y) ||
               inRange(i->pos.y+worldoffset.y, i->pos.y+worldoffset.y + i->size.y, edger2.y))
            {
                if(PointInsideRect(edgel0, i->pos+worldoffset, i->size) || PointInsideRect(edgel1, i->pos+worldoffset, i->size) || PointInsideRect(edgel2, i->pos+worldoffset, i->size))
                {
                    canWalkl = false;
                    player->pos.xclamp(i->pos.x+worldoffset.x + i->size.x + 1, INT32_MAX);
                }
                
                if(PointInsideRect(edger0, i->pos+worldoffset, i->size) || PointInsideRect(edger1, i->pos+worldoffset, i->size) || PointInsideRect(edger2, i->pos+worldoffset, i->size))
                {
                    canWalkr = false;
                    player->pos.xclamp(INT32_MIN, i->pos.x+worldoffset.x - player->size.x - 1);
                }
            }
        }
    }
    player->pos.xclamp(0, worldsize.x);
    player->pos.xclamp(0, pge->ScreenWidth() - player->size.x - 1);
}

//Update() called every frame, good for input
void Player::Update(double dElapsedTime, olc::PixelGameEngine *pge)
{
    top0    = olc::vi2d (  player->pos.x                         , player->pos.y - 1                   );
    top1    = olc::vi2d (  player->pos.x + player->size.x / 2    , player->pos.y - 1                   );
    top2    = olc::vi2d (  player->pos.x + player->size.x        , player->pos.y - 1                   );

    bottom0 = olc::vi2d (  player->pos.x                         , player->pos.y + player->size.y + 1  );
    bottom2 = olc::vi2d (  player->pos.x + player->size.x / 2    , player->pos.y + player->size.y + 1  );
    bottom2 = olc::vi2d (  player->pos.x + player->size.x        , player->pos.y + player->size.y + 1  );

    edgel0  = olc::vi2d (  player->pos.x - 1                     , player->pos.y + player->size.y      );
    edgel1  = olc::vi2d (  player->pos.x - 1                     , player->pos.y + player->size.y / 2  );
    edgel2  = olc::vi2d (  player->pos.x - 1                     , player->pos.y                       );
    edger0  = olc::vi2d (  player->pos.x + player->size.x + 1    , player->pos.y + player->size.y      );
    edger1  = olc::vi2d (  player->pos.x + player->size.x + 1    , player->pos.y + player->size.y / 2  );
    edger2  = olc::vi2d (  player->pos.x + player->size.x + 1    , player->pos.y                       );

    if(pge->GetKey(olc::Q).bHeld && canWalkl)
        vel.x -= speed;
    if(pge->GetKey(olc::D).bHeld && canWalkr)
        vel.x += speed;
    if(pge->GetKey(olc::SPACE).bPressed && isGrounded)
        vel.y -= jump;
    //so i can change the position of the player for collision purposes
    if(pge->GetMouse(olc::Mouse::LEFT).bPressed)
        player->pos = pge->GetMousePos();
}

Camera::Camera(Player* player)
{
    this->pos = olc::vi2d(0, 0);
    this->player = player;
}

void Camera::Update(double dElapsedTime, olc::PixelGameEngine* pge)
{
    player->player->pos.xclamp(0, pge->ScreenWidth()-player->player->size.x-1);
    vel.clamp(-player->speed, player->speed, -player->jump, player->g);
    worldoffset += vel;
    vel = olc::vi2d(0, vel.y);
    worldoffset.xclamp(-(worldsize.x - pge->ScreenWidth())-1, 0);
    if (-worldoffset.x + pge->ScreenWidth() <= worldsize.x)
    {
        player->player->pos.xclamp(0, pge->ScreenWidth() - player->player->size.x - 1 - 120);
        if (player->player->pos.x >= 259)
        {
            if (pge->GetKey(olc::D).bHeld && player->canWalkr) vel.x -= player->speed;
        }
    }

    for (auto const& i : Gameobjects)
    {
        for (auto const& x : i)
        {
            //if (inRange(player->top1.x - 200, player->bottom1.x + 200, x->pos.x + worldoffset.x) ||
            //    inRange(player->edgel1.y - 200, player->edgel1.y + 200, x->pos.y + worldoffset.y))
            //{
                x->Render(pge);
            //}
        }
    }
}