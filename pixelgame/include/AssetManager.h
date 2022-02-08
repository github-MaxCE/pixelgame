#pragma once
#include "olcPixelGameEngine.h"
#include <unordered_map>
#include <string>

namespace max
{
	class AssetManager
	{
	private:
		std::unordered_map<std::string, olc::Sprite*> sprites;
	public:
		olc::Sprite* Load(std::string name)
		{
			if (sprites.find(name) == sprites.end())
				sprites.insert(std::pair(name, new olc::Sprite(name)));

			return sprites[name];
		}
	};
}