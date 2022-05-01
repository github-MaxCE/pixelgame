#pragma once
#include "olcPixelGameEngine.h"
#include "GameObject.h"
#include "Entity.h"

namespace max
{
	class map;
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
		Camera(Player*, olc::PixelGameEngine*, max::map*);
		virtual ~Camera();
		virtual void Start() override {}
		virtual void FixedUpdate(float) override;
		virtual void Update(float) override;
		virtual void End() override {}
	};

	class Player : public max::Entity, public max::GameObject
	{
		max::map* world;
		Camera* camera;
		olc::vi2d top[3], edge[2][3], bottom[3];
		olc::vf2d vel;
		std::atomic<bool> endupdate{false};
	public:
		Player(olc::PixelGameEngine*, max::map*);
		virtual ~Player();
		virtual void Start() override {}
		virtual void FixedUpdate(float) override;
		virtual void Update(float) override;
		virtual void End() override {}
		bool isGrounded = false, canWalkl = true, canWalkr = true;
		float g = 8.0f;
		const float speed = 5.0f;
		float curspeed = speed;
		float jumpspeed = 24.0f;
	};
}
