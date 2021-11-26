#pragma once
#include "olcPixelGameEngine.h"
#include "worldmanager.h"

class Player
{
public:
	Rectobj* player;
	Player();
	virtual ~Player();
	bool isGrounded;
	bool canWalkl;
	bool canWalkr;
	float g;
	float speed;
	float jump;
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
	olc::vi2d vel;
	void FixedUpdate(double dElapsedTime);
	void Update(double dElapsedTime, olc::PixelGameEngine *pge);
};