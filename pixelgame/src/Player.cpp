#include "Player.h"

Player::Player()
{
	Player::pos.x = 5;
	Player::pos.y = 5;
}

Player::~Player()
{}

enum playerstate
{
	WALKING,
	JUMPING,
	FALLING
};

bool inRange(int low, int high, int x)
{
	return ((unsigned)(x-low) <= (high-low));
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

bool PointInsideRect(const olc::vi2d &point, const olc::vi2d &rectPos, const olc::vi2d &rectSize)
{
	return (point.x >= rectPos.x) &&
		   (point.y >= rectPos.y) &&
		   (point.x <= rectPos.x + rectSize.x) &&
		   (point.y <= rectPos.y + rectSize.y);
}

void Player::FixedUpdate(double dElapsedTime, olc::PixelGameEngine *pge, map *mmap)
{
	isGrounded = false;
	canWalkr = true;
	canWalkl = true;

	top0 = {pos.x, pos.y-1};
	top1 = {pos.x + size.x, pos.y-1};

	bottom0 = {pos.x, pos.y + size.y+1};
	bottom1 = {pos.x + size.x, pos.y + size.y+1};

	edgel0 = {pos.x-1, pos.y + size.y};
	edgel1 = {pos.x-1, pos.y + size.y/2};
	edgel2 = {pos.x-1, pos.y};
	edger0 = {pos.x + size.x+1, pos.y + size.y};
	edger1 = {pos.x + size.x+1, pos.y + size.y/2};
	edger2 = {pos.x + size.x+1, pos.y};

	vel.y += g;
	vel.clamp(-speed, speed, -jump, g);
	pos += vel;
	vel = {0, vel.y};
	for(auto &i : mmap->layer1)
	{
		if(inRange(pos.x-200, pos.x+200, i->pos.x) ||
		   inRange(pos.y-400, pos.y+400, i->pos.y))
		{
			if((inRange(i->pos.x, i->pos.x+i->size.x, top0.x) ||
				inRange(i->pos.x, i->pos.x+i->size.x, top1.x)) &&

				inRange(i->pos.y + i->size.y , INT32_MAX, top1.y))
			{
				pos.yclamp(i->pos.y + i->size.y+1, INT32_MAX);
				vel.y = g;
			}

			if((inRange(i->pos.x, i->pos.x+i->size.x, bottom0.x) ||
				inRange(i->pos.x, i->pos.x+i->size.x, bottom1.x)) &&

				inRange(INT32_MIN ,i->pos.y + 4, bottom1.y))
			{
				if(PointInsideRect(bottom0, i->pos, i->size) ||
			   	   PointInsideRect(bottom1, i->pos, i->size))
				{
					isGrounded = true;
				}
				pos.yclamp(INT32_MIN, i->pos.y-size.y-1);
			}
			if(inRange(i->pos.y, i->pos.y + i->size.y, edger0.y) ||
			   inRange(i->pos.y, i->pos.y + i->size.y, edger1.y) ||
			   inRange(i->pos.y, i->pos.y + i->size.y, edger2.y))
			{
				if(PointInsideRect(edgel0, i->pos, i->size) || PointInsideRect(edgel1, i->pos, i->size) || PointInsideRect(edgel2, i->pos, i->size))
				{
					canWalkl = false;
					pos.xclamp(i->pos.x + i->size.x + 1, INT32_MAX);
				}
				
				if(PointInsideRect(edger0, i->pos, i->size) || PointInsideRect(edger1, i->pos, i->size) || PointInsideRect(edger2, i->pos, i->size))
				{
					canWalkr = false;
					pos.xclamp(INT32_MIN, i->pos.x - size.x - 1);
				}
			}
		}
	}
}

//Update() called every frame, good for input
void Player::Update(double dElapsedTime, olc::PixelGameEngine *pge)
{
	if(pge->GetKey(olc::Q).bHeld && canWalkl)
		vel.x -= speed;
	if(pge->GetKey(olc::D).bHeld && canWalkr)
		vel.x += speed;
	if(pge->GetKey(olc::SPACE).bPressed && isGrounded)
		vel.y -= jump;
	//so i can change the position of the player for collision purposes
	if(pge->GetMouse(olc::Mouse::LEFT).bPressed)
		pos = pge->GetMousePos();
}
