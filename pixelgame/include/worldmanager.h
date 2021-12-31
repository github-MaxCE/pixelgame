#pragma once
#define XML

#include "olcPixelGameEngine.h"
#include "filemanager.h"
#include "rapidxml.h"
#include "rapidxml_utils.h"
#include "olcPGEX_Graphics2D.h"


#include "angelscript/angelscript.h"
#include "angelscript/scriptstdstring/scriptstdstring.h"
#include "angelscript/scriptbuilder/scriptbuilder.h"

#include "Gameobject.h"
#include "Entity.h"

extern bool loadmap(const char* mapname, olc::GFX2D *gfx2d, olc::PixelGameEngine* pge, asIScriptEngine* engine, CScriptBuilder* builder, asIScriptContext* ctx, asIScriptObject* map);

extern olc::vi2d worldoffset;
extern olc::vi2d worldsize;