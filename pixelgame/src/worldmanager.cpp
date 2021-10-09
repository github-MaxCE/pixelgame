#include "worldmanager.h"
#include "filemanager.h"

map::gameobject::~gameobject()
{}

map::ImageTile::ImageTile(int32_t x, int32_t y, bool alpha, olc::Sprite* sprite)
{
	this->pos.y  = y;
	this->pos.x  = x;
	this->sprite = sprite;
	this->alpha  = alpha;
}


map::RectTile::RectTile(int32_t x, int32_t y, olc::vi2d size, olc::Pixel col)
{
	this->pos.y = y;
	this->pos.x = x;
	this->size  = size;
}

map::FilledRectTile::FilledRectTile(int32_t x, int32_t y, olc::vi2d size, olc::Pixel col)
{
	this->pos.y = y;
	this->pos.x = x;
	this->size  = size;
}

void map::Tile(int32_t layer, int32_t x, int32_t y, bool alpha, olc::Sprite* sprite)
{
	mapelements[layer]->emplace_back(std::make_unique<ImageTile>(x, y, alpha, sprite));
}

void map::Tile(int32_t layer, olc::vi2d pos, bool alpha, olc::Sprite* sprite)
{
	Tile(layer, pos.x, pos.y, alpha, sprite);
}

void map::Tile(int32_t layer, int32_t x, int32_t y, int32_t w, int32_t h, olc::Pixel col, bool filled)
{
	std::cout << filled << std::endl;
	if (filled)
	{
		mapelements[layer]->emplace_back(std::make_unique<FilledRectTile>(x, y, olc::vi2d{ w, h }, col));
	}
	else
	{
		mapelements[layer]->emplace_back(std::make_unique<RectTile>(x, y, olc::vi2d{ w, h }, col));
	}
}

void map::Tile(int32_t layer, olc::vi2d pos, int32_t w, int32_t h, olc::Pixel col, bool filled)
{
	Tile(layer, pos.x, pos. y, w, h, col, filled);
}

map::map(std::string mapname)
{
	for (auto const& i : mapelements)
	{
		i->clear();
	}
	auto tmp = mappath() + mapname;
	rapidxml::xml_document<> doc;
	rapidxml::file<> xmlFile(tmp.c_str());
	doc.parse<0>(xmlFile.data());
	rapidxml::xml_node<> *root;
	root = doc.first_node("map");
	for(rapidxml::xml_node<> * node = root->first_node(); node; node = node->next_sibling())
	{
		switch(str2int(node->name(), 1))
		{
			case str2int("image", 1):
				Tile(/*l*/atoi(node->first_attribute("l")->value()),
					/*x*/atoi(node->first_attribute("x")->value()),
					/*y*/atoi(node->first_attribute("y")->value()),
					/*alpha*/node->first_attribute("transparency") == nullptr ? false : atoi(node->first_attribute("transparency")->value()) == 1,
					/*image*/new olc::Sprite(matpath() + node->first_attribute("image")->value())
					);
				break;
			case str2int("object", 1):
				Tile(/*l*/atoi(node->first_attribute("l")->value()),
					/*x*/atoi(node->first_attribute("x")->value()),
					/*y*/atoi(node->first_attribute("y")->value()),
					/*w*/atoi(node->first_attribute("w")->value()),
					/*h*/atoi(node->first_attribute("h")->value()),
					/*col*/olc::Pixel(/*r*/atoi(node->first_attribute("r")->value()),
									/*g*/atoi(node->first_attribute("g")->value()),
									/*b*/atoi(node->first_attribute("b")->value())
									),
					/*filled*/atoi(node->first_attribute("filled")->value())
					);
				break;
		}
	}
}