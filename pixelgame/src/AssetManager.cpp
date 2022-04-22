#include "AssetManager.h"

namespace max
{
	AssetManager& AssetManager::Get()
	{
		static AssetManager inst;
		return inst;
	}

	olc::Sprite* AssetManager::I_Load(std::string& name)
	{
		if (sprites.find(name) == sprites.end())
			sprites.insert(std::pair(name, new olc::Sprite(name)));

		return sprites[name];
	}

	olc::Sprite* AssetManager::Load(std::string name)
	{
		return Get().I_Load(name);
	}
}