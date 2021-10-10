#include "worldmanager.h"
#include "filemanager.h"

void map::gameobject::Draw(olc::PixelGameEngine *pge)
{}
map::gameobject::~gameobject()
{}

map::ImageTile::ImageTile(int32_t x, int32_t y, bool alpha, olc::Sprite* sprite)
{
	this->pos.y  = y;
	this->pos.x  = x;
	this->sprite = sprite;
	this->alpha  = alpha;
}
void map::ImageTile::Draw(olc::PixelGameEngine* pge)
{
	pge->DrawSprite(this->pos, this->sprite);
}


map::RectTile::RectTile(int32_t x, int32_t y, olc::vi2d size, olc::Pixel col)
{
	this->pos.y = y;
	this->pos.x = x;
	this->size  = size;
}
void map::RectTile::Draw(olc::PixelGameEngine* pge)
{
	pge->DrawRect(this->pos, this->size, this->col);
}

map::FilledRectTile::FilledRectTile(int32_t x, int32_t y, olc::vi2d size, olc::Pixel col)
{
	this->pos.y = y;
	this->pos.x = x;
	this->size  = size;
}

void map::FilledRectTile::Draw(olc::PixelGameEngine* pge)
{
	pge->FillRect(this->pos, this->size, this->col);
}



map::ImageTile *map::CreateImageRect(int32_t layer, int32_t x, int32_t y, bool alpha, olc::Sprite* sprite)
{
	ImageTile* val = new ImageTile(x, y, alpha, sprite);
	mapelements[layer]->emplace_back(val);
	return val;

}

map::RectTile *map::CreateRect(int32_t layer, int32_t x, int32_t y, int32_t w, int32_t h, olc::Pixel col)
{
	RectTile* val = new RectTile(x, y, olc::vi2d{ w, h }, col);
	mapelements[layer]->emplace_back(val);
	return val;
}

map::FilledRectTile *map::CreateFilledRect(int32_t layer, int32_t x, int32_t y, int32_t w, int32_t h, olc::Pixel col)
{
	FilledRectTile* val = new FilledRectTile(x, y, olc::vi2d{ w, h }, col);
	mapelements[layer]->emplace_back(val);
	return val;

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
				CreateImageRect(/*l*/atoi(node->first_attribute("l")->value()),
								/*x*/atoi(node->first_attribute("x")->value()),
								/*y*/atoi(node->first_attribute("y")->value()),
								/*alpha*/node->first_attribute("transparency") == 0 ? false : atoi(node->first_attribute("transparency")->value()),
								/*image*/new olc::Sprite(matpath() + node->first_attribute("image")->value())
					);
				break;
			case str2int("object", 1):
				if(node->first_attribute("filled") == 0 ? false : atoi(node->first_attribute("filled")->value()))
					CreateFilledRect(/*l*/atoi(node->first_attribute("l")->value()),
									 /*x*/atoi(node->first_attribute("x")->value()),
									 /*y*/atoi(node->first_attribute("y")->value()),
									 /*w*/atoi(node->first_attribute("w")->value()),
									 /*h*/atoi(node->first_attribute("h")->value()),
									 /*col*/olc::Pixel(/*r*/atoi(node->first_attribute("r")->value()),
									 				   /*g*/atoi(node->first_attribute("g")->value()),
									 				   /*b*/atoi(node->first_attribute("b")->value())
											)
						);
				else
					CreateFilledRect(/*l*/atoi(node->first_attribute("l")->value()),
									 /*x*/atoi(node->first_attribute("x")->value()),
									 /*y*/atoi(node->first_attribute("y")->value()),
									 /*w*/atoi(node->first_attribute("w")->value()),
									 /*h*/atoi(node->first_attribute("h")->value()),
									 /*col*/olc::Pixel(/*r*/atoi(node->first_attribute("r")->value()),
													   /*g*/atoi(node->first_attribute("g")->value()),
									 				   /*b*/atoi(node->first_attribute("b")->value())
											)
					);
				break;
		}
	}
}