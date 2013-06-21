// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

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

			ssvs::Tileset tileset{ssvs::Utils::getTilesetFromJson(ssvuj::getRootFromFile("Data/Tilesets/tilesetHuman.json"))};
			Json::Value animations{ssvuj::getRootFromFile("Data/Animations/animationsHuman.json")};
			ssvs::Animation animStand{ssvs::Utils::getAnimationFromJson(animations["stand"])};
			ssvs::Animation animJump{ssvs::Utils::getAnimationFromJson(animations["jump"])};
			ssvs::Animation animFall{ssvs::Utils::getAnimationFromJson(animations["fall"])};
			ssvs::Animation animCrouch{ssvs::Utils::getAnimationFromJson(animations["crouch"])};
			ssvs::Animation animRun{ssvs::Utils::getAnimationFromJson(animations["run"])};
			ssvs::Animation animWalk{ssvs::Utils::getAnimationFromJson(animations["walk"])};
			ssvs::Animation animCrouchWalk{ssvs::Utils::getAnimationFromJson(animations["crouchWalk"])};
			ssvs::Animation* currentAnim{nullptr};

		public:
			NRCHumanoidAnimationController(sses::Entity& mEntity, NRCRender& mCRender, NRCHumanoid& mCHumanoid);

			void update(float mFrameTime) override;
	};
}

#endif
