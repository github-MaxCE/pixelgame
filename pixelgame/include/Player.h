#pragma once
#include "olcPixelGameEngine.h"
#include "worldmanager.h"

class Player;

class Camera
{
public:
	Camera(Player* player);
	olc::vi2d pos;
	olc::vi2d vel;
	Player* player;
	void Update(double dElapsedTime, olc::PixelGameEngine* pge);
};

class Player
{
public:
	Player();
	virtual ~Player();
	Camera* camera;
	Rectobj* player;
	bool isGrounded;
	bool canWalkl;
	bool canWalkr;
	float g;
	float speed;
	float jump;
	olc::vi2d top0;
	olc::vi2d top1;
	olc::vi2d top2;
	olc::vi2d bottom0;
	olc::vi2d bottom1;
	olc::vi2d bottom2;
	olc::vi2d edgel0;
	olc::vi2d edgel1;
	olc::vi2d edgel2;
	olc::vi2d edger0;
	olc::vi2d edger1;
	olc::vi2d edger2;
	olc::vi2d vel;
	void FixedUpdate(double dElapsedTime, olc::PixelGameEngine* pge);
	void Update(double dElapsedTime, olc::PixelGameEngine *pge);
};


extern bool inRange(int low, int high, int x);
extern bool PointInsideRect(const olc::vi2d& point, const olc::vi2d& rectPos, const olc::vi2d& rectSize);
