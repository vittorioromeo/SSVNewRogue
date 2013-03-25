#include "Core/NRAssets.h"

using namespace std;
using namespace sf;
using namespace ssvu::FileSystem;
using namespace ssvs;

namespace nr
{
	NRAssets::NRAssets() 
	{ 
		getFiles("C:/"); // BUG: these useless lines are needed to avoid linker errors (WTF)
		assetManager.loadFolder("Data/");
	}
	
	// Getters
	AssetManager& NRAssets::getAssetManager() { return assetManager; }
	Texture& NRAssets::getTexture(string mId) { return assetManager.getTexture(mId); }	
}
