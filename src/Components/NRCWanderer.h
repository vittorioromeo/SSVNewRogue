// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_COMPONENTS_WANDERER
#define SSVNR_COMPONENTS_WANDERER

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;
	class NRCHumanoid;

	class NRCWanderer : public sses::Component
	{
		private:
			NRGame& game;
			NRCHumanoid& cHumanoid;
			float time{200};
			int dir{0};
			bool tired{false};

		public:
			NRCWanderer(sses::Entity& mEntity, NRGame& mGame, NRCHumanoid& mCHumanoid);

			void update(float mFrameTime) override;
	};
}

#endif