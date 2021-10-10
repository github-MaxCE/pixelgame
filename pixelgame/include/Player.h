#pragma once
#include "Engine.h"

class Player
{
public:
	Player(map* map);
	virtual ~Player();
	map::gameobject *player;
	bool isGrounded;
	bool canWalkl;
	bool canWalkr;
	float g = 8.0f;
	float speed = 5.0f;
	float jump = 50.0f;
	void FixedUpdate(double dElapsedTime, map *mmap);
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
	olc::vi2d vel;
};