// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Core/NRAssets.h"

using namespace std;
using namespace sf;
using namespace ssvu::FileSystem;
using namespace ssvs;
using namespace ssvs::Utils;
using namespace ssvuj;

namespace nr
{
	NRAssets::NRAssets()
	{
		loadAssetsFromJson(assetManager, "Data/", getRootFromFile("Data/assets.json"));
	}

	// Getters
	AssetManager& NRAssets::getAssetManager() { return assetManager; }
	Texture& NRAssets::getTexture(const string& mId) { return assetManager.getTexture(mId); }
}
