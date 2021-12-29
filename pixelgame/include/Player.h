#pragma once
#include "olcPixelGameEngine.h"
#include "worldmanager.h"
#include "Gameobject.h"

class Player;

class Camera
{
public:
	Camera(Player* player, olc::PixelGameEngine* pge);
	olc::PixelGameEngine* pge;
	olc::vi2d pos;
	olc::vi2d vel;
	Player* player;
	void Update(double dElapsedTime);
};

class Player
{
public:
	Player(olc::PixelGameEngine* pge);
	virtual ~Player();
	olc::PixelGameEngine* pge;
	Camera* camera;
	GameObject* player;
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
	void FixedUpdate(double dElapsedTime);
	void Update(double dElapsedTime);
};


extern bool inRange(int low, int high, int x);

template<class T = int32_t>
extern bool RectInsideRect(const olc::v2d_generic<T>& rect1Pos, const olc::v2d_generic<T>& rect1Size, const olc::v2d_generic<T>& rect2Pos, const olc::v2d_generic<T>& rect2Size);

template<class T = int32_t>
extern bool PointInsideRect(const olc::v2d_generic<T>& point, const olc::v2d_generic<T>& rectPos, const olc::v2d_generic<T>& rectSize);
