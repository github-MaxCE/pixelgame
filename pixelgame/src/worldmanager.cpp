#include "worldmanager.h"

bool loadmap(const char* mapname, olc::GFX2D *gfx2d, olc::PixelGameEngine *pge,lua_State* L)
{
    using xml_doc = rapidxml::xml_document<>;
    using xml_node = rapidxml::xml_node<>;
    using xml_file = rapidxml::file<>;

    DeleteAllGameobjects();

    auto tmp = mappath() + mapname;

    xml_file *xmlFile = new xml_file(tmp.c_str());
    xml_doc *doc = new xml_doc();
    doc->parse<0>(xmlFile->data());
    const xml_node *root = doc->first_node("pxg");
    const xml_node *script = root->first_node("script");
    const xml_node *map = root->first_node("map");
    if (script != 0)
    {
        int r = luaL_dostring(L, script->value());
        if (r != LUA_OK) return false;
    }
    for(xml_node *node = map->first_node(); node; node = node->next_sibling())
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

            new Sprite(l, pos, size, col, sprite, name, gfx2d, offset, alpha, pge);
        }
        else if (strcmp(node->name(), "object") == 0)
        {
            bool filled = (node->first_attribute("filled") == 0) ? false : atoi(node->first_attribute("filled")->value());

            if (filled)
                new FilledRect(l, pos, size, col, name, offset, alpha, pge);
            else
                new GameObject(l, pos, size, col, name, offset, alpha, pge);
        }
    }
    delete doc, xmlFile, root;
}