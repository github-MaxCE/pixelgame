#pragma once
#include "olcPixelGameEngine.h"
#include "worldmanager.h"
#include "Entity.h"
#include "Gameobject.h"

extern bool inRange(int low, int high, int x);

template<class T = int32_t>
extern bool RectInsideRect(const olc::v2d_generic<T>& rect1Pos, const olc::v2d_generic<T>& rect1Size, const olc::v2d_generic<T>& rect2Pos, const olc::v2d_generic<T>& rect2Size);

template<class T = int32_t>
extern bool PointInsideRect(const olc::v2d_generic<T>& point, const olc::v2d_generic<T>& rectPos, const olc::v2d_generic<T>& rectSize);

class Player;

class Camera : public Entity
{
public:
	Camera(Player* player, olc::PixelGameEngine* pge);
	virtual void FixedUpdate(float fElapsedTime) override;
	virtual void Update(float fElapsedTime) override;

private:
	olc::PixelGameEngine* pge;
	olc::vi2d pos, vel;
	Player* player;
};

class Player : public Entity
{
public:
	Player(olc::PixelGameEngine* pge);
	virtual ~Player();
	virtual void FixedUpdate(float fElapsedTime) override;
	virtual void Update(float fElapsedTime) override;
	GameObject player;
	bool isGrounded,
		 canWalkl,
		 canWalkr;
	float g,
		  speed,
		  jumpspeed;

private:
	olc::PixelGameEngine* pge;
	Camera* camera;



	olc::vi2d top[3],
			  edge[2][3],
			  bottom[3],
			  vel;
};