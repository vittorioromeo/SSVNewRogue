#ifndef SSVNR_COMPONENTS_ANIMATIONCONTROLLER
#define SSVNR_COMPONENTS_ANIMATIONCONTROLLER

#include "Core/NRDependencies.h"
#include "Utils/NRAnimation.h"
#include "Utils/NRUtils.h"

namespace nr
{
	class NRCRender;
	class NRCHumanoid;

	class NRCHumanoidAnimationController : public sses::Component
	{
		private:
			NRCRender& cRender;
			NRCHumanoid& cHumanoid;

			ssvs::Tileset tileset{getTilesetFromJSON(ssvuj::getRootFromFile("Data/Tilesets/tilesetHuman.json"))};
			Json::Value animations{ssvuj::getRootFromFile("Data/Animations/animationsHuman.json")};
			NRAnimation animStand{getAnimationFromJSON(animations["stand"])};
			NRAnimation animJump{getAnimationFromJSON(animations["jump"])};
			NRAnimation animFall{getAnimationFromJSON(animations["fall"])};
			NRAnimation animCrouch{getAnimationFromJSON(animations["crouch"])};
			NRAnimation animRun{getAnimationFromJSON(animations["run"])};
			NRAnimation animWalk{getAnimationFromJSON(animations["walk"])};
			NRAnimation animCrouchWalk{getAnimationFromJSON(animations["crouchWalk"])};
			NRAnimation* currentAnim{nullptr};

		public:
			NRCHumanoidAnimationController(sses::Entity& mEntity, NRCRender& mCRender, NRCHumanoid& mCHumanoid);

			void update(float mFrameTime) override;
	};
}

#endif
