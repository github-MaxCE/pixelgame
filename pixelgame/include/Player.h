#pragma once
#include "olcPixelGameEngine.h"
#include "worldmanager.h"
#include "Entity.h"
#include "Gameobject.h"

extern bool inRange(int low, int high, int x);

template<class T1 = int32_t, class T2 = int32_t, class T3 = int32_t>
extern bool PointInsideRect(const olc::v2d_generic<T1>& point, const olc::v2d_generic<T2>& rectPos, const olc::v2d_generic<T3>& rectSize);

template<class T1 = int32_t, class T2 = int32_t, class T3 = int32_t>
extern bool RectInsideRect(const olc::v2d_generic<T1>& rect1Pos, const olc::v2d_generic<T1>& rect1Size, const olc::v2d_generic<T1>& rect2Pos, const olc::v2d_generic<T1>& rect2Size);

class Player;

class Camera : public max::Entity
{
public:
	Camera(Player* player, olc::PixelGameEngine* pge);
	virtual void Start() override {}
	virtual void FixedUpdate(float fElapsedTime) override;
	virtual void Update(float fElapsedTime) override;
	virtual void End() override {}

private:
	olc::PixelGameEngine* pge;
	olc::vi2d pos, vel;
	Player* player;
};

class Player : public max::Entity
{
public:
	Player(olc::PixelGameEngine* pge);
	virtual ~Player();
	virtual void Start() override {}
	virtual void FixedUpdate(float fElapsedTime) override;
	virtual void Update(float fElapsedTime) override;
	virtual void End() override {}
	GameObject player;
	bool isGrounded = false, canWalkl = true, canWalkr = true;
	float g = 8.0f, speed = 4.5f, jumpspeed = 50.0f;
private:
	olc::PixelGameEngine* pge;
	Camera* camera;
	olc::vi2d top[3], edge[2][3], bottom[3], vel = olc::vi2d(0, 0);
};