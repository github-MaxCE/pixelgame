#pragma once
#include <olcPixelGameEngine.h>
#include <unordered_map>
#include <string>

namespace max
{
	class AssetManager
	{
		AssetManager() = default;
		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;

		AssetManager& operator=(const AssetManager&) = delete;
		AssetManager& operator=(AssetManager&&) = delete;

		std::unordered_map<std::string, olc::Sprite*> sprites;

		olc::Sprite* I_Load(std::string name);

		static AssetManager& Get();
	public:
		static olc::Sprite* Load(std::string name);
	};
}