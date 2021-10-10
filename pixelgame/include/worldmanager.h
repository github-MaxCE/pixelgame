#pragma once
#include <list>
#include <cmath>
#include <iostream>
#define XML
#include "Engine.h"

class map
{
public:
	map(std::string mapname);

	public: class gameobject
	{
	public:
		virtual ~gameobject();
		virtual void Draw(olc::PixelGameEngine* pge);
		int8_t layer;
		olc::vi2d pos;
		olc::vi2d size;
		olc::Pixel col;
	};

	public: class ImageTile : public gameobject
	{
	public:
		ImageTile(int32_t x, int32_t y, bool alpha = false, olc::Sprite* sprite = nullptr);
		virtual void Draw(olc::PixelGameEngine* pge);
		olc::Sprite* sprite;
		bool alpha;
	};

	public: class RectTile : public  gameobject
	{
	public:
		RectTile(int32_t x = 0, int32_t y = 0, olc::vi2d size = { 0, 0 }, olc::Pixel col = olc::BLACK);
		virtual void Draw(olc::PixelGameEngine* pge);
	};

	public: class FilledRectTile : public RectTile
	{
	public:
		FilledRectTile(int32_t x = 0, int32_t y = 0, olc::vi2d size = {0, 0}, olc::Pixel col = olc::BLACK);
		virtual void Draw(olc::PixelGameEngine* pge);
	};

public:
	std::list<gameobject*> layer0;
	std::list<gameobject*> layer1;
	std::list<gameobject*> layer2;
	std::list<gameobject*> layer3;
	std::list<gameobject*>* mapelements[4]
	{
		&layer0,
		&layer1,
		&layer2,
		&layer3
	};

public:
	ImageTile* CreateImageRect(int32_t layer, int32_t x, int32_t y, bool alpha, olc::Sprite* sprite);
	RectTile* CreateRect(int32_t layer, int32_t x, int32_t y, int32_t w, int32_t h, olc::Pixel col);
	FilledRectTile* CreateFilledRect(int32_t layer, int32_t x, int32_t y, int32_t w, int32_t h, olc::Pixel col);
};