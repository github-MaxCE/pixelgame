#pragma once
#include "angelscript/angelscript.h"
#include "angelscript/scriptstdstring/scriptstdstring.h"
#include "angelscript/scriptbuilder/scriptbuilder.h"
#include "AngelScriptutil.h"
#include "rapidxml.h"
#include "rapidxml_utils.h"
#include "filemanager.h"
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Gameobject.h"
#include "Entity.h"

namespace max
{
    class map : public max::angelscript::Entity
    {
    public:
        //const char* file, const char* objdecl, asIScriptEngine* _engine, CScriptBuilder* _builder, asIScriptContext* _ctx, bool emplace = true, Args... args
        template<class... Args>
        inline map(const char* mapname, olc::GFX2D* gfx2d, olc::PixelGameEngine* pge, asIScriptEngine* engine, CScriptBuilder* builder, asIScriptContext* ctx, Args... args) :
            max::angelscript::Entity((std::string() + "maps/" + mapname + ".as").c_str(), mapname, engine, builder, ctx, true, ...args)
        {
            DeleteAllGameObjects();

            auto tmp_mappath = mappath() + mapname + ".xml";

            rapidxml::file<>* xmlFile = new rapidxml::file<>(tmp_mappath.c_str());
            rapidxml::xml_document<>* doc = new rapidxml::xml_document<>();
            doc->parse<0>(xmlFile->data());
            rapidxml::xml_node<>* root_node = doc->first_node("pxg");
            rapidxml::xml_node<>* script_node = root_node->first_node("script");
            rapidxml::xml_node<>* map_node = root_node->first_node("map");
            std::string tmp_scriptpath;
            if (script_node != 0)
            {
                if (script_node->first_attribute("file") != 0)
                {
                    tmp_scriptpath = (scriptpath() + script_node->first_attribute("file")->value());
                }
            }
            if (!tmp_scriptpath.empty())
            {
                int r = builder->StartNewModule(engine, mapname);
                if (r < 0) printf("Unrecoverable error while starting a new module.\n");

                r = builder->AddSectionFromFile(tmp_scriptpath.c_str());
                if (r < 0) printf("Please correct the errors in the script and try again.\n");

                r = builder->BuildModule();
                if (r < 0) printf("Please correct the errors in the script and try again.\n");

                // Find the function that is to be called. 
                asIScriptFunction* func = engine->GetModule(mapname)->GetFunctionByDecl("void main()");
                if (max::angelscript::callfunc("void main()", engine->GetModule(mapname), engine) == 0)
                    printf("no void main() function. skipping\n");
            }

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
                    olc::Sprite* sprite = new olc::Sprite(matpath() + node->first_attribute("image")->value());

                    new Sprite(l, pos, size, col, sprite, name, gfx2d, offset, alpha, true, pge);
                }
                else if (strcmp(node->name(), "object") == 0)
                {
                    bool filled = (node->first_attribute("filled") == 0) ? false : atoi(node->first_attribute("filled")->value());

                    if (filled)
                        new max::FilledRect(l, pos, size, col, name, offset, alpha, true, pge);
                    else
                        new max::GameObject(l, pos, size, col, name, offset, alpha, true, pge);
                }
            }

            for (auto entity : Entities)
            {
                entity->Start();
            }

            delete xmlFile, doc, root_node, script_node, map_node;
        }

        ~map();

        std::list<max::GameObject*> GameObjects[4];
        olc::vi2d size = olc::vi2d(0, 0);

        void DeleteAllGameObjects();

        max::GameObject* FindGameObject(std::string& name);
    };
}