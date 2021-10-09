#pragma once
#include "Engine.h"

class Player
{
public:
	Player();
	virtual ~Player();
	bool isGrounded;
	bool jumping;
	bool canWalkl;
	bool canWalkr;
	float g = 8.0f;
	float speed = 5.0f;
	float jump = 50.0f;
	void FixedUpdate(double dElapsedTime, olc::PixelGameEngine *pge, map *mmap);
	void Update(double dElapsedTime, olc::PixelGameEngine *pge);
	olc::vi2d top0;
	olc::vi2d top1;
	olc::vi2d bottom0;
	olc::vi2d bottom1;
	olc::vi2d edgel0;
	olc::vi2d edgel1;
	olc::vi2d edgel2;
	olc::vi2d edger0;
	olc::vi2d edger1;
	olc::vi2d edger2;
	olc::vi2d pos;
	olc::vi2d vel;
	olc::vi2d size{20, 30};
};