#pragma once
#define XML
#include "olcPixelGameEngine.h"
#include "filemanager.h"
#include "rapidxml.h"
#include "rapidxml_utils.h"
#include "Gameobject.h"
#include "olcPGEX_Graphics2D.h"
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

extern bool loadmap(const char* mapname, olc::GFX2D *gfx2d, olc::PixelGameEngine* pge, lua_State* L);