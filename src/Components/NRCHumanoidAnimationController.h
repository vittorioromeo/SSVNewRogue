#ifndef SSVNR_COMPONENTS_ANIMATIONCONTROLLER
#define SSVNR_COMPONENTS_ANIMATIONCONTROLLER

#include "Core/NRDependencies.h"
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
			ssvs::Animation animStand{getAnimationFromJSON(animations["stand"])};
			ssvs::Animation animJump{getAnimationFromJSON(animations["jump"])};
			ssvs::Animation animFall{getAnimationFromJSON(animations["fall"])};
			ssvs::Animation animCrouch{getAnimationFromJSON(animations["crouch"])};
			ssvs::Animation animRun{getAnimationFromJSON(animations["run"])};
			ssvs::Animation animWalk{getAnimationFromJSON(animations["walk"])};
			ssvs::Animation animCrouchWalk{getAnimationFromJSON(animations["crouchWalk"])};
			ssvs::Animation* currentAnim{nullptr};

		public:
			NRCHumanoidAnimationController(sses::Entity& mEntity, NRCRender& mCRender, NRCHumanoid& mCHumanoid);

			void update(float mFrameTime) override;
	};
}

#endif
