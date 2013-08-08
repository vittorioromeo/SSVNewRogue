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
		loadAssetsFromJson(assetManager, "Data/", readFromFile("Data/assets.json"));
		assetManager.load<BitmapFont>("limeStroked", assetManager.get<Texture>("limeStroked.png"), as<BitmapFontData>(readFromFile("Data/lime.json"))); // TODO: automatically load bitmapfonts in ssvs
	}
}
