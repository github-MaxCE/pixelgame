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
public:
	class gameobject
	{
	public:
		virtual ~gameobject();
		const char* type = "gameobject";
		int8_t layer;
		olc::vi2d pos;
		olc::vi2d size;
	};
public:
	class ImageTile : public gameobject
	{
	public:
		ImageTile(int32_t x = 0, int32_t y = 0, bool alpha = false, olc::Sprite* sprite = nullptr);
		const char* type = "ImageTile";
		olc::Sprite* sprite;
		bool alpha;
	};

	public: class RectTile : public  gameobject
	{
	public:
		RectTile(int32_t x = 0, int32_t y = 0, olc::vi2d size = { 0, 0 }, olc::Pixel col = olc::BLACK);
		const char* type = "RectTile";
		olc::Pixel col;
	};

	public: class FilledRectTile : public gameobject
	{
	public:
		FilledRectTile(int32_t x = 0, int32_t y = 0, olc::vi2d size = { 0, 0 }, olc::Pixel col = olc::BLACK);
		const char* type = "FilledRectTile";
		olc::Pixel col;
	};

public:
	std::list<std::unique_ptr<gameobject>> layer0;
	std::list<std::unique_ptr<gameobject>> layer1;
	std::list<std::unique_ptr<gameobject>> layer2;
	std::list<std::unique_ptr<gameobject>> layer3;
	std::list<std::unique_ptr<gameobject>>* mapelements[4]
	{
		&layer0,
		&layer1,
		&layer2,
		&layer3
	};

public:
	void Tile(int32_t layer, int32_t x, int32_t y, bool alpha, olc::Sprite* sprite);
	void Tile(int32_t layer, olc::vi2d pos, bool alpha, olc::Sprite* sprite);
	void Tile(int32_t layer, int32_t x, int32_t y, int32_t w, int32_t h, olc::Pixel col, bool filled);
	void Tile(int32_t layer, olc::vi2d pos, int32_t w, int32_t h, olc::Pixel col, bool filled);
};