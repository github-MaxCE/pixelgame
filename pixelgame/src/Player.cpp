#include "Player.h"

Player::Player(olc::PixelGameEngine* pge) :
    player(new GameObject(3, olc::vi2d(15, 10), olc::vi2d(20, 30), olc::CYAN, "player", false, false, pge)),
    camera(new Camera(this, pge)),
    isGrounded(false),
    canWalkr(true),
    canWalkl(true),
    g(8.0f),
    speed(5.0f),
    jump(50.0f),
    pge(pge)
{}

Player::~Player()
{}

bool inRange(int low, int high, int x)
{
    return ((unsigned)(x-low) <= (high-low));
}

template<typename T>
bool PointInsideRect(const olc::v2d_generic<T>& point, const olc::v2d_generic<T>& rectPos, const olc::v2d_generic<T>& rectSize)
{
    return (point.x >= rectPos.x) &&
           (point.y >= rectPos.y) &&
           (point.x <= rectPos.x + rectSize.x) &&
           (point.y <= rectPos.y + rectSize.y);
}

template<typename T>
bool RectInsideRect(const olc::v2d_generic<T>& rect1Pos, const olc::v2d_generic<T>& rect1Size, const olc::v2d_generic<T>& rect2Pos, const olc::v2d_generic<T>& rect2Size)
{
    // To check if either rectangle is actually a line
    // For example :  l1 ={-1,0}  r1={1,1}  l2={0,-1}
    // r2={0,1}

    olc::v2d_generic<T> r1 = rect1Pos + rect1Size;
    olc::v2d_generic<T> r2 = rect2Pos + rect2Size;

    // If one rectangle is on left side of other
    if (r1.y <= rect2Pos.y || r2.y <= rect1Pos.y) return false;

    // If one rectangle is above other
    if (r1.y >= rect2Pos.y || r2.y >= rect1Pos.y) return false;

    return true;
}

/// <summary>
/// Player::FixedUpdate() : called 60 times a second, good for physics
/// </summary>
/// <param name="dElapsedTime">
/// : the time in seconds between the last and current frame
/// </param>
/// <param name="pge">
/// : pointer to the main game class to access pge functions
/// </param>
void Player::FixedUpdate(double dElapsedTime)
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
void Player::Update(double dElapsedTime)
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

Camera::Camera(Player* player, olc::PixelGameEngine* pge) :
    player(player),
    pge(pge),
    pos(0, 0)
{}

void Camera::Update(double dElapsedTime)
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
            x->Render();
        }
    }
}