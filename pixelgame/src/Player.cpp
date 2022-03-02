#include "Player.h"
#include "map.h"

namespace max
{
    Player::Player(olc::PixelGameEngine* pge, max::map* world, max::EventSystem* events) :
        pge(pge),
        world(world),
        max::Entity(true),
        player(3, olc::vi2d(5, 5), olc::vi2d(20, 30), olc::CYAN, "player", false, false, true, pge, world),
        camera(new max::Camera(this, pge, world)),
        max::EventSubscriber(events)
    {
        events->Add                (&Player::Right      , this, olc::Key::D       , 0, 2);
        events->Add                (&Player::Left       , this, olc::Key::Q       , 0, 2);
        events->Add                (&Player::Jump       , this, olc::Key::SPACE   , 1, 2);
        events->Add<max::HW::MOUSE>(&Player::OnMouseLeft, this, olc::Mouse::M_LEFT, 1, 0);
        
    }

    Player::~Player()
    {
        delete camera;
    }

    bool inRange(int low, int high, int x)
    {
        return ((unsigned)(x - low) <= (high - low));
    }

    template<class T1, class T2, class T3>
    bool PointInsideRect(const olc::v2d_generic<T1>& point, const olc::v2d_generic<T2>& rectPos, const olc::v2d_generic<T3>& rectSize)
    {
        return (point.x >= rectPos.x) &&
            (point.y >= rectPos.y) &&
            (point.x <= rectPos.x + rectSize.x) &&
            (point.y <= rectPos.y + rectSize.y);
    }

    template<class T1, class T2, class T3>
    bool RectInsideRect(const olc::v2d_generic<T1>& rect1Pos, const olc::v2d_generic<T1>& rect1Size, const olc::v2d_generic<T1>& rect2Pos, const olc::v2d_generic<T1>& rect2Size)
    {
        // To check if either rectangle is actually a line
        // For example :  l1 ={-1,0}  r1={1,1}  l2={0,-1}
        // r2={0,1}

        auto r1 = rect1Pos + rect1Size;
        auto r2 = rect2Pos + rect2Size;

        // If one rectangle is on left side of other
        if (r1.y <= rect2Pos.y || r2.y <= rect1Pos.y) return false;

        // If one rectangle is above other
        if (r1.y >= rect2Pos.y || r2.y >= rect1Pos.y) return false;

        return true;
    }

    void Player::Left(float fElapsedTime) { if (canWalkl) vel.x -= curspeed * fElapsedTime; }
    void Player::Right(float fElapsedTime) { if (canWalkr) vel.x += curspeed * fElapsedTime; }
    void Player::Jump(float fElapsedTime) { if (isGrounded) vel.y -= jumpspeed * fElapsedTime; }
    void Player::OnMouseLeft(float fElapsedTime) { player.pos = pge->GetMousePos(); }

    //Update() called every frame, good for input
    void Player::Update(float fElapsedTime)
    {
        top[0] = olc::vi2d(player.pos.x, player.pos.y - 1);
        top[1] = olc::vi2d(player.pos.x + player.size.x / 2, player.pos.y - 1);
        top[2] = olc::vi2d(player.pos.x + player.size.x, player.pos.y - 1);

        bottom[0] = olc::vi2d(player.pos.x, player.pos.y + player.size.y + 1);
        bottom[2] = olc::vi2d(player.pos.x + player.size.x / 2, player.pos.y + player.size.y + 1);
        bottom[2] = olc::vi2d(player.pos.x + player.size.x, player.pos.y + player.size.y + 1);

        edge[0][0] = olc::vi2d(player.pos.x - 1, player.pos.y + player.size.y);
        edge[0][1] = olc::vi2d(player.pos.x - 1, player.pos.y + player.size.y / 2);
        edge[0][2] = olc::vi2d(player.pos.x - 1, player.pos.y);
        edge[1][0] = olc::vi2d(player.pos.x + player.size.x + 1, player.pos.y + player.size.y);
        edge[1][1] = olc::vi2d(player.pos.x + player.size.x + 1, player.pos.y + player.size.y / 2);
        edge[1][2] = olc::vi2d(player.pos.x + player.size.x + 1, player.pos.y);

        auto CTRL = pge->GetKey(olc::CTRL);
        auto C = pge->GetKey(olc::C);

        curspeed = CTRL.bHeld || C.bReleased ? speed / 2 : speed;

        if (CTRL.bHeld && C.bHeld)
        {
            curspeed = speed * 2 / 5;
        }

        if ((C.bReleased && CTRL.bHeld && CTRL.bReleased) || CTRL.bReleased)
        {
            curspeed = speed;
        }
    }

    void Player::FixedUpdate(float fElapsedTime)
    {
        isGrounded = false;
        canWalkr = true;
        canWalkl = true;

        vel.y += g;
        vel.clamp(-curspeed, curspeed, -jumpspeed, g);
        player.pos += vel;
        vel = { 0, vel.y };
        for (auto const& i : world->GameObjects[1])
        {
            if (inRange(top[0].x - 200, bottom[0].x + 200, i->pos.x + camera->worldoffset.x) ||
                inRange(edge[0][1].y - 200, edge[0][1].y + 200, i->pos.y + camera->worldoffset.y))
            {
                if ((inRange(i->pos.x + camera->worldoffset.x, i->pos.x + camera->worldoffset.x + i->size.x, top[0].x) ||
                    inRange(i->pos.x + camera->worldoffset.x, i->pos.x + camera->worldoffset.x + i->size.x, top[2].x)) &&

                    inRange(i->pos.y + camera->worldoffset.y + i->size.y, INT32_MAX, top[0].y))
                {
                    player.pos.yclamp(i->pos.y + camera->worldoffset.y + i->size.y + 1, INT32_MAX);
                    vel.y = g;
                }

                if ((inRange(i->pos.x + camera->worldoffset.x, i->pos.x + camera->worldoffset.x + i->size.x, bottom[0].x) ||
                    inRange(i->pos.x + camera->worldoffset.x, i->pos.x + camera->worldoffset.x + i->size.x, bottom[2].x)) &&

                    inRange(INT32_MIN, i->pos.y + camera->worldoffset.y + 4, bottom[0].y))
                {
                    if (max::PointInsideRect(bottom[0], i->pos + camera->worldoffset, i->size) ||
                        max::PointInsideRect(bottom[2], i->pos + camera->worldoffset, i->size))
                    {
                        isGrounded = true;
                    }
                    player.pos.yclamp(INT32_MIN, i->pos.y + camera->worldoffset.y - player.size.y - 1);
                }
                if (inRange(i->pos.y + camera->worldoffset.y, i->pos.y + camera->worldoffset.y + i->size.y, edge[1][0].y) ||
                    inRange(i->pos.y + camera->worldoffset.y, i->pos.y + camera->worldoffset.y + i->size.y, edge[1][2].y))
                {
                    if (max::PointInsideRect(edge[0][0], i->pos + camera->worldoffset, i->size) || max::PointInsideRect(edge[0][1], i->pos + camera->worldoffset, i->size) || max::PointInsideRect(edge[0][2], i->pos + camera->worldoffset, i->size))
                    {
                        canWalkl = false;
                        player.pos.xclamp(i->pos.x + camera->worldoffset.x + i->size.x + 1, INT32_MAX);
                    }

                    if (max::PointInsideRect(edge[1][0], i->pos + camera->worldoffset, i->size) || max::PointInsideRect(edge[1][1], i->pos + camera->worldoffset, i->size) || max::PointInsideRect(edge[1][2], i->pos + camera->worldoffset, i->size))
                    {
                        canWalkr = false;
                        player.pos.xclamp(INT32_MIN, i->pos.x + camera->worldoffset.x - player.size.x - 1);
                    }
                }
            }
        }
        player.pos.xclamp(0, world->size.x - player.size.x);
        player.pos.xclamp(0, pge->ScreenWidth() - player.size.x - 1);
    }

    Camera::Camera(Player* player, olc::PixelGameEngine* pge, max::map* world) :
        pge(pge),
        max::Entity(true),
        player(player),
        world(world),
        pos(0, 0)
    {}

    void Camera::FixedUpdate(float fElapsedTime)
    {
        vel.clamp(-player->curspeed, player->curspeed, -player->jumpspeed, player->g);
        worldoffset += vel;
        vel.x = 0;
        worldoffset.xclamp(-(world->size.x - pge->ScreenWidth()) - 1, 0);
        if (-worldoffset.x + pge->ScreenWidth() <= world->size.x)
        {
            player->player.pos.xclamp(0, pge->ScreenWidth() - player->player.size.x - 1 - 120);
            if (player->player.pos.x >= 259)
            {
                if (pge->GetKey(olc::D).bHeld && player->canWalkr) vel.x -= player->curspeed;
            }
        }
    }

    void Camera::Update(float fElapsedTime)
    {
        pge->Clear(olc::BLACK);

        for (const auto& i : world->GameObjects)
        {
            for (const auto& x : i)
            {
                x->Render(this);
            }
        }
    }
}