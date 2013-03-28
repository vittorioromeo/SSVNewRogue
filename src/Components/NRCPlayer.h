#ifndef SSVNR_COMPONENTS_PLAYER
#define SSVNR_COMPONENTS_PLAYER

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;
	class NRCHumanoid;
	
	class NRCPlayer : public sses::Component
	{
		private:
			NRGame& game;
			NRCHumanoid& cHumanoid;

		public:
			NRCPlayer(sses::Entity& mEntity, NRGame& mGame, NRCHumanoid& mCHumanoid);
			
			void update(float) override;
	};
}

#endif
