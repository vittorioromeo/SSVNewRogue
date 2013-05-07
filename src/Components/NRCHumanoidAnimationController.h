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

			ssvs::Tileset tileset{ssvs::Utils::getTilesetFromJSON(ssvuj::getRootFromFile("Data/Tilesets/tilesetHuman.json"))};
			Json::Value animations{ssvuj::getRootFromFile("Data/Animations/animationsHuman.json")};
			ssvs::Animation animStand{ssvs::Utils::getAnimationFromJSON(animations["stand"])};
			ssvs::Animation animJump{ssvs::Utils::getAnimationFromJSON(animations["jump"])};
			ssvs::Animation animFall{ssvs::Utils::getAnimationFromJSON(animations["fall"])};
			ssvs::Animation animCrouch{ssvs::Utils::getAnimationFromJSON(animations["crouch"])};
			ssvs::Animation animRun{ssvs::Utils::getAnimationFromJSON(animations["run"])};
			ssvs::Animation animWalk{ssvs::Utils::getAnimationFromJSON(animations["walk"])};
			ssvs::Animation animCrouchWalk{ssvs::Utils::getAnimationFromJSON(animations["crouchWalk"])};
			ssvs::Animation* currentAnim{nullptr};

		public:
			NRCHumanoidAnimationController(sses::Entity& mEntity, NRCRender& mCRender, NRCHumanoid& mCHumanoid);

			void update(float mFrameTime) override;
	};
}

#endif
