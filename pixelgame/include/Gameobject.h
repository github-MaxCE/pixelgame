#pragma once
#include <list>
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class Gameobject
{
public:
	int layer;
	olc::vi2d size;
	std::string name;
	olc::vi2d pos;
	Gameobject* Parent;
	std::list<Gameobject*> Childs;
	virtual ~Gameobject();
	void AddChild(Gameobject* child);
	Gameobject* FindChild(std::string name);
	virtual void Render(olc::PixelGameEngine* pge) {}
	static void INIT(Gameobject* obj, int layer, olc::vi2d pos, olc::vi2d size, std::string name);
};

class Rectobj : public Gameobject
{
public:
	olc::Pixel color;
	Rectobj(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name);
	~Rectobj();
	virtual void Render(olc::PixelGameEngine* pge);
};

class FilledRectobj : public Gameobject
{
public:
	olc::Pixel color;
	FilledRectobj(int layer, olc::vi2d pos, olc::vi2d size, olc::Pixel col, std::string name);
	~FilledRectobj();
	virtual void Render(olc::PixelGameEngine* pge);
};

class Spriteobj : public Gameobject
{
public:
	olc::Sprite* sprite;
	bool alpha;
	Spriteobj(int layer, olc::vi2d pos, olc::vi2d size, bool alpha, olc::Sprite* sprite, std::string name);
	~Spriteobj();
	virtual void Render(olc::PixelGameEngine* pge);
};

extern Gameobject* FindGameObject(std::string name);
extern void DeleteAllGameobjects();
extern std::list<Gameobject*> Gameobjects[4];