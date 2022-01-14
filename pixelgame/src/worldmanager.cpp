#include "worldmanager.h"

olc::vi2d worldoffset = olc::vi2d( 0 , 0 );
olc::vi2d worldsize = olc::vi2d( 0 , 0 );

bool loadmap(const char* mapname, olc::GFX2D *gfx2d, olc::PixelGameEngine *pge, asIScriptEngine* engine, CScriptBuilder* builder, asIScriptContext* ctx)
{
    using xml_doc = rapidxml::xml_document<>;
    using xml_node = rapidxml::xml_node<>;
    using xml_file = rapidxml::file<>;

    DeleteAllGameobjects();

    auto tmp_mappath = mappath() + mapname;

    xml_file *xmlFile = new xml_file(tmp_mappath.c_str());
    xml_doc *doc = new xml_doc();
    doc->parse<0>(xmlFile->data());
    xml_node *root_node = doc->first_node("pxg");
    xml_node *script_node = root_node->first_node("script");
    xml_node *map_node = root_node->first_node("map");
    std::string tmp_scriptpath;
    if (script_node != 0)
    {
        if (script_node->first_attribute("file") != 0)
        {
            tmp_scriptpath = (scriptpath() + script_node->first_attribute("file")->value());
        }
    }
    if(!tmp_scriptpath.empty())
    {
        int r = builder->StartNewModule(engine, mapname);
        if (r < 0) printf("Unrecoverable error while starting a new module.\n");

        r = builder->AddSectionFromFile(tmp_scriptpath.c_str());
        if (r < 0) printf("Please correct the errors in the script and try again.\n");

        r = builder->BuildModule();
        if (r < 0) printf("Please correct the errors in the script and try again.\n");

        // Find the function that is to be called. 
        asIScriptFunction* func = engine->GetModule(mapname)->GetFunctionByDecl("void main()");
        if (max::angelscript::callfunc(func, engine, ctx) == 0)
            printf("no void main() function. skipping\n");
    }

    for(xml_node *node = map_node->first_node(); node; node = node->next_sibling())
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
                new FilledRect(l, pos, size, col, name, offset, alpha, true, pge);
            else
                new GameObject(l, pos, size, col, name, offset, alpha, true, pge);
        }
    }

    for (auto entity : Entities)
    {
        entity->Start();
    }

    delete xmlFile, doc, root_node, script_node, map_node;
}
