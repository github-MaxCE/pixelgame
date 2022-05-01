#pragma once
#include <utility>
#include "rapidxml.h"
#include "rapidxml_utils.h"
#include "filemanager.h"
#include "Gameobject.h"
#include "ScriptEntity.h"
#include <olcPixelGameEngine.h>
#include <olcPGEX_Graphics2D.h>
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
            script::Entity(mapname, (std::string() + "maps/" + mapname + ".as").c_str(), false, std::forward<Args>(args)...)
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
                max::GameObject* obj;

                bool alpha = (node->first_attribute("transparency") == 0) ? false : atoi(node->first_attribute("transparency")->value());

                bool offset = node->first_attribute("offset") == 0 ? true : atoi(node->first_attribute("offset")->value());

                olc::vi2d pos = olc::vi2d(atoi(node->first_attribute("x")->value()),
                                          atoi(node->first_attribute("y")->value()));
                
                olc::vu2d size = olc::vu2d(atoi(node->first_attribute("w")->value()),
                                           atoi(node->first_attribute("h")->value()));

                olc::Pixel col = olc::Pixel((node->first_attribute("r")) == 0 ? 0 : atoi(node->first_attribute("r")->value()),
                                            (node->first_attribute("g")) == 0 ? 0 : atoi(node->first_attribute("g")->value()),
                                            (node->first_attribute("b")) == 0 ? 0 : atoi(node->first_attribute("b")->value()),
                                            (node->first_attribute("a")) == 0 ? 0 : atoi(node->first_attribute("a")->value()));

                int l = atoi(node->first_attribute("l")->value());

                const char* name = node->first_attribute("name")->value();

                if (strcmp(node->name(), "image") == 0)
                {
                    olc::Sprite* sprite = AssetManager::Load(node->first_attribute("image")->value());

                    obj = new Sprite(pos, size, col, sprite, name, gfx2d, offset, alpha, pge, this);
                }
                else if (strcmp(node->name(), "object") == 0)
                {
                    bool filled = (node->first_attribute("filled") == 0) ? false : atoi(node->first_attribute("filled")->value());

                    if (filled)
                        obj = new max::FilledRect(pos, size, col, name, offset, alpha, pge, this);
                    else
                        obj = new max::GameObject(pos, size, col, name, offset, alpha, pge, this);
                }

                GameObjects[l].emplace_back(obj);
            }
            
            delete xmlFile;
            delete doc;
            delete root_node;
            delete script_node;
            delete map_node;
        }

        ~map();

        std::vector<max::GameObject*> GameObjects[4];
        olc::vi2d size{0, 0};

        void DeleteAllGameObjects();

        GameObject* Add(int, GameObject*);

        max::GameObject* FindGameObject(std::string);
    };
}