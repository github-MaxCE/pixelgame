#include "worldmanager.h"

void loadmap(std::string mapname, olc::GFX2D gfx2d)
{
    using xml_doc = rapidxml::xml_document<>;
    using xml_node = rapidxml::xml_node<>;
    using xml_file = rapidxml::file<>;

    DeleteAllGameobjects();

    auto tmp = mappath() + mapname;


    xml_file *xmlFile = new xml_file(tmp.c_str());
    xml_doc *doc = new xml_doc;
    doc->parse<0>(xmlFile->data());
    xml_node *root = doc->first_node("xml");
    xml_node *map = root->first_node("map");
    for(xml_node *node = map->first_node(); node; node = node->next_sibling())
    {
        bool alpha = (node->first_attribute("transparency") == 0) ? false : atoi(node->first_attribute("transparency")->value());

        bool offset = (node->first_attribute("offset") == 0) ? true : atoi(node->first_attribute("offset")->value());

        olc::vi2d pos = olc::vi2d(atoi(node->first_attribute("x")->value()),
            atoi(node->first_attribute("y")->value()));

        olc::vi2d size = olc::vi2d(atoi(node->first_attribute("w")->value()),
            atoi(node->first_attribute("h")->value()));

        olc::Pixel col = olc::Pixel(atoi(node->first_attribute("r")->value()),
            atoi(node->first_attribute("g")->value()),
            atoi(node->first_attribute("b")->value()),
            atoi(node->first_attribute("a")->value()));

        int l = atoi(node->first_attribute("l")->value());

        std::string name = node->first_attribute("name")->value();

        if (strcmp(node->name(), "image") == 0)
        {
            olc::Sprite* sprite = new olc::Sprite(matpath() + node->first_attribute("image")->value());

            new Spriteobj(l, pos, size, col, sprite, name, &gfx2d, offset, alpha);
        }
        else if (strcmp(node->name(), "object") == 0)
        {
            bool filled = (node->first_attribute("filled") == 0) ? false : atoi(node->first_attribute("filled")->value());

            if (filled)
                new FilledRectobj(l, pos, size, col, name, offset, alpha);
            else
                new Rectobj(l, pos, size, col, name, offset, alpha, true);
        }
    }
    delete doc, xmlFile, root;
}