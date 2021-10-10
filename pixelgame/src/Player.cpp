#include "Player.h"

Player::Player(map* map)
{
	player = new map::RectTile(5, 5, olc::vi2d{ 20, 30 }, olc::CYAN);
	map->mapelements[3]->push_front(player);
	player = map->mapelements[3]->front();
	isGrounded = false;
	canWalkr = true;
	canWalkl = true;
}

Player::~Player()
{}

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

void Player::FixedUpdate(double dElapsedTime, map *map)
{
	isGrounded = false;
	canWalkr = true;
	canWalkl = true;

	top0 = {player->pos.x, player->pos.y-1};
	top1 = {player->pos.x + player->size.x, player->pos.y-1};

	bottom0 = {player->pos.x, player->pos.y + player->size.y+1};
	bottom1 = {player->pos.x + player->size.x, player->pos.y + player->size.y+1};

	edgel0 = {player->pos.x-1, player->pos.y + player->size.y};
	edgel1 = {player->pos.x-1, player->pos.y + player->size.y/2};
	edgel2 = {player->pos.x-1, player->pos.y};
	edger0 = {player->pos.x + player->size.x+1, player->pos.y + player->size.y};
	edger1 = {player->pos.x + player->size.x+1, player->pos.y + player->size.y/2};
	edger2 = {player->pos.x + player->size.x+1, player->pos.y};

	vel.y += g;
	vel.clamp(-speed, speed, -jump, g);
	player->pos += vel;
	vel = {0, vel.y};
	for (auto const& i : map->layer1)
	{
		if(inRange(player->pos.x-200, player->pos.x+200, i->pos.x) ||
		   inRange(player->pos.y-400, player->pos.y+400, i->pos.y))
		{
			if((inRange(i->pos.x, i->pos.x+i->size.x, top0.x) ||
				inRange(i->pos.x, i->pos.x+i->size.x, top1.x)) &&

				inRange(i->pos.y + i->size.y , INT32_MAX, top1.y))
			{
				player->pos.yclamp(i->pos.y + i->size.y+1, INT32_MAX);
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
				player->pos.yclamp(INT32_MIN, i->pos.y - player->size.y-1);
			}
			if(inRange(i->pos.y, i->pos.y + i->size.y, edger0.y) ||
			   inRange(i->pos.y, i->pos.y + i->size.y, edger1.y) ||
			   inRange(i->pos.y, i->pos.y + i->size.y, edger2.y))
			{
				if(PointInsideRect(edgel0, i->pos, i->size) || PointInsideRect(edgel1, i->pos, i->size) || PointInsideRect(edgel2, i->pos, i->size))
				{
					canWalkl = false;
					player->pos.xclamp(i->pos.x + i->size.x + 1, INT32_MAX);
				}
				
				if(PointInsideRect(edger0, i->pos, i->size) || PointInsideRect(edger1, i->pos, i->size) || PointInsideRect(edger2, i->pos, i->size))
				{
					canWalkr = false;
					player->pos.xclamp(INT32_MIN, i->pos.x - player->size.x - 1);
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
		player->pos = pge->GetMousePos();
}
