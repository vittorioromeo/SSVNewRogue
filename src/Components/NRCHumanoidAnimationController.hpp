// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_COMPONENTS_ANIMATIONCONTROLLER
#define SSVNR_COMPONENTS_ANIMATIONCONTROLLER

#include "Core/NRDependencies.hpp"
#include "Utils/NRUtils.hpp"

namespace nr
{
	class NRCRender;
	class NRCHumanoid;

	class NRCHumanoidAnimationController : public sses::Component
	{
		private:
			NRCRender& cRender;
			NRCHumanoid& cHumanoid;

			ssvs::Tileset tileset{ssvuj::getExtr<ssvs::Tileset>(ssvuj::getFromFile("Data/Tilesets/tilesetHuman.json"))};
			ssvuj::Obj animations{ssvuj::getFromFile("Data/Animations/animationsHuman.json")};
			ssvs::Animation animStand{ssvs::getAnimationFromJson(tileset, ssvuj::getObj(animations, "stand"))};
			ssvs::Animation animJump{ssvs::getAnimationFromJson(tileset, ssvuj::getObj(animations, "jump"))};
			ssvs::Animation animFall{ssvs::getAnimationFromJson(tileset, ssvuj::getObj(animations, "fall"))};
			ssvs::Animation animCrouch{ssvs::getAnimationFromJson(tileset, ssvuj::getObj(animations, "crouch"))};
			ssvs::Animation animRun{ssvs::getAnimationFromJson(tileset, ssvuj::getObj(animations, "run"))};
			ssvs::Animation animWalk{ssvs::getAnimationFromJson(tileset, ssvuj::getObj(animations, "walk"))};
			ssvs::Animation animCrouchWalk{ssvs::getAnimationFromJson(tileset, ssvuj::getObj(animations, "crouchWalk"))};
			ssvs::Animation* currentAnim{nullptr};

		public:
			NRCHumanoidAnimationController(NRCRender& mCRender, NRCHumanoid& mCHumanoid);

			void update(FT mFT) override;
	};
}

#endif
