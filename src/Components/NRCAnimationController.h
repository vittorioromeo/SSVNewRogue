#ifndef SSVNR_COMPONENTS_ANIMATIONCONTROLLER
#define SSVNR_COMPONENTS_ANIMATIONCONTROLLER

#include "Core/NRDependencies.h"
#include "Utils/NRAnimation.h"

namespace nr
{
	class NRCRender;
	class NRCPhysics;
	class NRCHumanoid;
	
	class NRCAnimationController : public sses::Component
	{
		private:
			NRCPhysics& cPhysics;
			NRCRender& cRender;
			NRCHumanoid& cHumanoid;
			ssvsc::Body& body;
			
			ssvs::Tileset testTileset;
			NRAnimation testStandAnim, testRunAnim, testWalkAnim, testJumpAnim, testFallAnim, testCrouchAnim, testCrouchWalkAnim;
			NRAnimation* testCurrentAnim{nullptr};
		
		public:
			NRCAnimationController(sses::Entity& mEntity, NRCPhysics& mCPhysics, NRCRender& mCRender, NRCHumanoid& mCHumanoid);
			
			void update(float mFrameTime) override;
	};
	
	// TODO: make this component NRCHumanoidAnimationController, refactor it and clean it up (or use LUA in the future?)
}

#endif
