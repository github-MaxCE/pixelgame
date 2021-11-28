#include "worldmanager.h"

void loadmap(std::string mapname)
{
    DeleteAllGameobjects();
    auto tmp = mappath() + mapname;
    rapidxml::file<> *xmlFile = new rapidxml::file(tmp.c_str());
    rapidxml::xml_document<>* doc = new rapidxml::xml_document;
    doc->parse<0>(xmlFile->data());
    rapidxml::xml_node<> *root = doc->first_node("map");
    for(rapidxml::xml_node<> * node = root->first_node(); node; node = node->next_sibling())
    {
        if (strcmp(node->name(), "image") == 0)
        {
            int l = atoi(node->first_attribute("l")->value());

            olc::vi2d pos = olc::vi2d(atoi(node->first_attribute("x")->value()),
                                      atoi(node->first_attribute("y")->value()));

            olc::vi2d size = olc::vi2d(atoi(node->first_attribute("w")->value()),
                                      atoi(node->first_attribute("h")->value()));

            bool alpha = (node->first_attribute("transparency") == 0) ? false : atoi(node->first_attribute("transparency")->value());

            olc::Sprite* sprite = new olc::Sprite(matpath() + node->first_attribute("image")->value());

            std::string name = node->first_attribute("name")->value();

            new Spriteobj(l, pos, size, alpha, sprite, name);
        }
        else if (strcmp(node->name(), "object") == 0)
        {
            bool filled = (node->first_attribute("filled") == 0) ? false : atoi(node->first_attribute("filled")->value());

            int l = atoi(node->first_attribute("l")->value());

            olc::vi2d pos = olc::vi2d(atoi(node->first_attribute("x")->value()),
                                      atoi(node->first_attribute("y")->value()));

            olc::vi2d size = olc::vi2d(atoi(node->first_attribute("w")->value()),
                                       atoi(node->first_attribute("h")->value()));

            olc::Pixel col = olc::Pixel(atoi(node->first_attribute("r")->value()),
                                        atoi(node->first_attribute("g")->value()),
                                        atoi(node->first_attribute("b")->value()));

            std::string name = node->first_attribute("name")->value();

            if (filled)
                new FilledRectobj(l, pos, size, col, name);
            else
                new Rectobj(l, pos, size, col, name);
        }
    }
    delete doc, xmlFile, root;
}