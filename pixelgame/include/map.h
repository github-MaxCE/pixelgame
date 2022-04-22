#pragma once
#include <utility>
#include "rapidxml.h"
#include "rapidxml_utils.h"
#include "filemanager.h"
#include "Gameobject.h"
#include "ScriptEntity.h"
#include "map.h"
#include "olcPixelGameEngine.h"
#include "AssetManager.h"
#include "ScriptEngine.h"

namespace max
{
    class map : public script::Entity
    {
    public:
        //const char* file, const char* objdecl, asIScriptEngine* _engine, CScriptBuilder* _builder, asIScriptContext* _ctx, bool emplace = true, Args... args
        template<class... Args>
        inline map(const char* mapname, olc::GFX2D* gfx2d, olc::PixelGameEngine* pge, Args&& ...args) :
            script::Entity(mapname, (std::string() + "maps/" + mapname + ".as").c_str(), true, std::forward<Args>(args)...),
            assets(assets)
        {
            DeleteAllGameObjects();

            auto tmp_mappath = mappath() + mapname + ".xml";

            rapidxml::file<char>* xmlFile = new rapidxml::file<char>(tmp_mappath.c_str());
            rapidxml::xml_document<>* doc = new rapidxml::xml_document<>();
            doc->parse<0>(xmlFile->data());
            rapidxml::xml_node<>* root_node = doc->first_node("pxg");
            rapidxml::xml_node<>* script_node = root_node->first_node("script");
            rapidxml::xml_node<>* map_node = root_node->first_node("map");
            std::string tmp_scriptpath = std::string() + "maps/" + mapname + ".as";
            if (script_node != 0 && script_node->first_attribute("file") != 0)
                tmp_scriptpath = (scriptpath() + script_node->first_attribute("file")->value());
            
            script::Engine::callfunc("void main()", tmp_scriptpath.c_str());

            for (rapidxml::xml_node<>* node = map_node->first_node(); node; node = node->next_sibling())
            {
                bool alpha = (node->first_attribute("transparency") == 0) ? false : atoi(node->first_attribute("transparency")->value());

                bool offset = node->first_attribute("offset") == 0 ? true : atoi(node->first_attribute("offset")->value());

                olc::vi2d pos = olc::vi2d(atoi(node->first_attribute("x")->value()),
                    atoi(node->first_attribute("y")->value()));

                olc::vi2d size = olc::vi2d(atoi(node->first_attribute("w")->value()),
                    atoi(node->first_attribute("h")->value()));

                olc::Pixel col = olc::Pixel((node->first_attribute("r")) == 0 ? 0 : atoi(node->first_attribute("r")->value()),
                    (node->first_attribute("g")) == 0 ? 0 : atoi(node->first_attribute("g")->value()),
                    (node->first_attribute("b")) == 0 ? 0 : atoi(node->first_attribute("b")->value()),
                    (node->first_attribute("a")) == 0 ? 0 : atoi(node->first_attribute("a")->value()));

                int l = atoi(node->first_attribute("l")->value());

                std::string name = node->first_attribute("name")->value();

                if (strcmp(node->name(), "image") == 0)
                {
                    olc::Sprite* sprite = AssetManager::Load(matpath() + node->first_attribute("image")->value());

                    new Sprite(l, pos, size, col, sprite, name, gfx2d, offset, alpha, true, pge, this);
                }
                else if (strcmp(node->name(), "object") == 0)
                {
                    bool filled = (node->first_attribute("filled") == 0) ? false : atoi(node->first_attribute("filled")->value());

                    if (filled)
                        new max::FilledRect(l, pos, size, col, name, offset, alpha, true, pge, this);
                    else
                        new max::GameObject(l, pos, size, col, name, offset, alpha, true, pge, this);
                }
            }
            
            delete xmlFile, doc, root_node, script_node, map_node;
        }

        ~map();

        std::list<max::GameObject*> GameObjects[4];
        olc::vi2d size = olc::vi2d(0, 0);
        max::AssetManager* assets;

        void DeleteAllGameObjects();

        max::GameObject* FindGameObject(std::string& name);
    };
}