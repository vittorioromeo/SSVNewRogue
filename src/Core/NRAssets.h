#ifndef SSVNR_CORE_ASSETS
#define SSVNR_CORE_ASSETS

#include "Core/NRDependencies.h"

namespace nr
{
	class NRAssets 
	{ 
		private:
			ssvs::AssetManager assetManager;
			
		public:
			NRAssets();
			
			// Getters
			ssvs::AssetManager& getAssetManager();
			sf::Texture& getTexture(std::string mId);
	};
}

#endif
