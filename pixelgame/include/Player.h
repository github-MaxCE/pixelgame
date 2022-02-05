#pragma once
#include "olcPixelGameEngine.h"
#include "GameObject.h"
#include "map.h"
#include "Entity.h"

namespace max
{
	extern bool inRange(int low, int high, int x);

	template<class T1 = int32_t, class T2 = int32_t, class T3 = int32_t>
	extern bool PointInsideRect(const olc::v2d_generic<T1>& point, const olc::v2d_generic<T2>& rectPos, const olc::v2d_generic<T3>& rectSize);

	template<class T1 = int32_t, class T2 = int32_t, class T3 = int32_t>
	extern bool RectInsideRect(const olc::v2d_generic<T1>& rect1Pos, const olc::v2d_generic<T1>& rect1Size, const olc::v2d_generic<T1>& rect2Pos, const olc::v2d_generic<T1>& rect2Size);

	class Player;

	class Camera : public max::Entity
	{
	private:
		olc::PixelGameEngine* pge;
		Player* player;
		max::map* world;
		olc::vi2d pos, vel;
	public:
		olc::vi2d worldoffset;
		Camera(Player* player, olc::PixelGameEngine* pge, max::map* world);
		virtual void Start() override {}
		virtual void FixedUpdate(float fElapsedTime) override;
		virtual void Update(float fElapsedTime) override;
		virtual void End() override {}
	};

	class Player : public max::Entity
	{
	private:
		olc::PixelGameEngine* pge;
		max::map* world;
	public:
		Player(olc::PixelGameEngine* pge, max::map* world);
		virtual ~Player();
		virtual void Start() override {}
		virtual void FixedUpdate(float fElapsedTime) override;
		virtual void Update(float fElapsedTime) override;
		virtual void End() override {}
		max::GameObject player;
		bool isGrounded = false, canWalkl = true, canWalkr = true;
		float g = 8.0f, speed = 4.5f, jumpspeed = 50.0f;
	private:
		Camera* camera;
		olc::vi2d top[3], edge[2][3], bottom[3], vel = olc::vi2d(0, 0);
	};
}
