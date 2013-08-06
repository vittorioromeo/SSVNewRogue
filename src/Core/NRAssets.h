// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

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

			inline ssvs::AssetManager& operator()() { return assetManager; }
			template<typename T> inline T& get(const std::string& mId) { return assetManager.get<T>(mId); }
	};
}

#endif
