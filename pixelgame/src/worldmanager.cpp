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
        if (node->name() == "image")
        {
            new Spriteobj(atoi(node->first_attribute("l")->value()),

                olc::vi2d(atoi(node->first_attribute("x")->value()),
                    atoi(node->first_attribute("y")->value())),

                olc::vi2d(atoi(node->first_attribute("w")->value()),
                    atoi(node->first_attribute("h")->value())),

                node->first_attribute("transparency") == 0 ? false : atoi(node->first_attribute("transparency")->value()) == 1,

                new olc::Sprite(matpath() + node->first_attribute("image")->value()),

                node->first_attribute("name")->value());
        }
        else if (node->name() == "object")
        {
            if ((node->first_attribute("filled") == 0)? false : atoi(node->first_attribute("filled")->value()))
            {
                new FilledRectobj(atoi(node->first_attribute("l")->value()),

                    olc::vi2d(atoi(node->first_attribute("x")->value()),
                        atoi(node->first_attribute("y")->value())),

                    olc::vi2d(atoi(node->first_attribute("w")->value()),
                        atoi(node->first_attribute("h")->value())),

                    olc::Pixel(atoi(node->first_attribute("r")->value()),
                        atoi(node->first_attribute("g")->value()),
                        atoi(node->first_attribute("b")->value())),

                    node->first_attribute("name")->value());
            }
            else
            {
                new Rectobj(atoi(node->first_attribute("l")->value()),

                    olc::vi2d(atoi(node->first_attribute("x")->value()),
                        atoi(node->first_attribute("y")->value())),

                    olc::vi2d(atoi(node->first_attribute("w")->value()),
                        atoi(node->first_attribute("h")->value())),

                    olc::Pixel(atoi(node->first_attribute("r")->value()),
                        atoi(node->first_attribute("g")->value()),
                        atoi(node->first_attribute("b")->value())),

                    node->first_attribute("name")->value());
            }
        }
    }
    delete doc, xmlFile, root;
}