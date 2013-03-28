#ifndef SSVNR_COMPONENTS_ANIMATIONCONTROLLER
#define SSVNR_COMPONENTS_ANIMATIONCONTROLLER

#include "Core/NRDependencies.h"
#include "Utils/NRAnimation.h"

namespace nr
{
	class NRCRender;
	class NRCHumanoid;
	
	class NRCHumanoidAnimationController : public sses::Component
	{
		private:
			NRCRender& cRender;
			NRCHumanoid& cHumanoid;
			
			ssvs::Tileset tileset;
			NRAnimation animStand, animRun, animWalk, animJump, animFall, animCrouch, animCrouchWalk;
			NRAnimation* currentAnim{nullptr};
		
		public:
			NRCHumanoidAnimationController(sses::Entity& mEntity, NRCRender& mCRender, NRCHumanoid& mCHumanoid);
			
			void update(float mFrameTime) override;
	};
}

#endif
