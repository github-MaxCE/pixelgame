#include "Player.h"
#include "mathutils.h"
#include "map.h"

namespace max
{
    Player::Player(olc::PixelGameEngine* pge, max::map* world) :
        Entity(true),
        GameObject({5, 5}, {20, 30}, olc::CYAN, "player", false, false, pge, world),
        world(world),
        camera(new max::Camera(this, pge, world))
    {
        world->Add(3, this);
    }

    Player::~Player()
    {
        delete camera;
    }

    //Update() called every frame, good for input
    void Player::Update(float fElapsedTime)
    {
        top[0] = olc::vi2d(pos.x, pos.y - 1);
        top[1] = olc::vi2d(pos.x + size.x / 2, pos.y - 1);
        top[2] = olc::vi2d(pos.x + size.x, pos.y - 1);

        bottom[0] = olc::vi2d(pos.x, pos.y + size.y + 1);
        bottom[2] = olc::vi2d(pos.x + size.x / 2, pos.y + size.y + 1);
        bottom[2] = olc::vi2d(pos.x + size.x, pos.y + size.y + 1);

        edge[0][0] = olc::vi2d(pos.x - 1, pos.y + size.y);
        edge[0][1] = olc::vi2d(pos.x - 1, pos.y + size.y / 2);
        edge[0][2] = olc::vi2d(pos.x - 1, pos.y);
        edge[1][0] = olc::vi2d(pos.x + size.x + 1, pos.y + size.y);
        edge[1][1] = olc::vi2d(pos.x + size.x + 1, pos.y + size.y / 2);
        edge[1][2] = olc::vi2d(pos.x + size.x + 1, pos.y);

        if (pge->GetKey(olc::Key::Q).bHeld && canWalkl) vel.x -= curspeed;
        if (pge->GetKey(olc::Key::D).bHeld && canWalkr) vel.x += curspeed;
        if (pge->GetKey(olc::Key::SPACE).bPressed && isGrounded) vel.y -= jumpspeed * 10;
        if (pge->GetKey(olc::Key::CTRL).bPressed) curspeed = curspeed == speed ? speed / 2.0f : speed;
        if (pge->GetKey(olc::Key::C).bPressed) curspeed = curspeed == speed ? speed : (curspeed == speed / 2 ? speed / 4 : speed / 2);
        if (pge->GetKey(olc::Key::CTRL).bReleased) curspeed = speed;

        if (pge->GetMouse(olc::Mouse::M_LEFT).bPressed)
        {
            pos = pge->GetMousePos();
            endupdate = true;
        }
    }

    void Player::FixedUpdate(float fElapsedTime)
    {
        isGrounded = false;
        canWalkr = true;
        canWalkl = true;

        vel.y += g;
        vel.clamp(-curspeed, curspeed, -jumpspeed, g);
        pos += (vel * 100) * fElapsedTime;
        vel.x = 0;

        for (auto const& i : world->GameObjects[1])
        {
            if (endupdate) goto endcollision;

            if (!RectInsideRect(i->pos + camera->worldoffset, i->size, olc::vi2d{ -1, -1 }, pge->ScreenSize() + olc::vu2d{ 1, 1 })) continue;

            if ((inRange(i->pos.x + camera->worldoffset.x, i->pos.x + camera->worldoffset.x + i->size.x, top[0].x) ||
                 inRange(i->pos.x + camera->worldoffset.x, i->pos.x + camera->worldoffset.x + i->size.x, top[1].x) ||
                 inRange(i->pos.x + camera->worldoffset.x, i->pos.x + camera->worldoffset.x + i->size.x, top[2].x)) &&
                 top[0].y > i->pos.y + camera->worldoffset.y + i->size.y)
            {
                auto clmp = i->pos.y + i->size.y + camera->worldoffset.y + 1;
                pos.y = pos.y > clmp ? pos.y : clmp;
                vel.y = g;
            }

            if ((inRange(i->pos.x + camera->worldoffset.x, i->pos.x + camera->worldoffset.x + i->size.x, bottom[0].x) ||
                 inRange(i->pos.x + camera->worldoffset.x, i->pos.x + camera->worldoffset.x + i->size.x, bottom[1].x) ||
                 inRange(i->pos.x + camera->worldoffset.x, i->pos.x + camera->worldoffset.x + i->size.x, bottom[2].x)) &&
                 bottom[0].y < i->pos.y + camera->worldoffset.y + 4)
            {
                if (max::PointInsideRect(bottom[0], i->pos + camera->worldoffset, i->size) ||
                    max::PointInsideRect(bottom[1], i->pos + camera->worldoffset, i->size) ||
                    max::PointInsideRect(bottom[2], i->pos + camera->worldoffset, i->size)) isGrounded = true;
                
                auto clmp = i->pos.y - size.y + camera->worldoffset.y - 1;
                pos.y = pos.y < clmp ? pos.y : clmp;
            }

            if (inRange(i->pos.y + camera->worldoffset.y, i->pos.y + i->size.y + camera->worldoffset.y, edge[0][0].y) ||
                inRange(i->pos.y + camera->worldoffset.y, i->pos.y + i->size.y + camera->worldoffset.y, edge[0][1].y) ||
                inRange(i->pos.y + camera->worldoffset.y, i->pos.y + i->size.y + camera->worldoffset.y, edge[0][2].y))
            {
                if (PointInsideRect(edge[0][0], i->pos + camera->worldoffset, i->size) ||
                    PointInsideRect(edge[0][1], i->pos + camera->worldoffset, i->size) ||
                    PointInsideRect(edge[0][2], i->pos + camera->worldoffset, i->size))
                {
                    canWalkl = false;
                    auto clmp = i->pos.x + camera->worldoffset.x + i->size.x + 1;
                    pos.x = pos.x > clmp ? pos.x : clmp;
                }

                if (PointInsideRect(edge[1][0], i->pos + camera->worldoffset, i->size) ||
                    PointInsideRect(edge[1][1], i->pos + camera->worldoffset, i->size) ||
                    PointInsideRect(edge[1][2], i->pos + camera->worldoffset, i->size))
                {
                    canWalkr = false;
                    auto clmp = i->pos.x - size.x + camera->worldoffset.x - 1;
                    pos.x = pos.x < clmp ? pos.x : clmp;
                }
            }
        }
    endcollision:
        endupdate = false;
        pos.xclamp(0, pge->ScreenWidth() - size.x - 1);
        pos.xclamp(0, world->size.x - size.x);
    }

    Camera::Camera(Player* player, olc::PixelGameEngine* pge, map* world) :
        Entity(true),
        pge(pge),
        player(player),
        world(world)
    {}

    Camera::~Camera()
    {}

    void Camera::FixedUpdate(float fElapsedTime)
    {
        vel.clamp(-player->curspeed, player->curspeed, -player->jumpspeed, player->g);
        worldoffset += vel;
        vel.x = 0;
        worldoffset.xclamp(-signed(world->size.x - pge->ScreenWidth()) - 1, 0);
        if (-worldoffset.x + pge->ScreenWidth() <= world->size.x)
        {
            player->pos.xclamp(0, pge->ScreenWidth() - player->size.x - 1 - 120);
            if (player->pos.x >= 259)
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