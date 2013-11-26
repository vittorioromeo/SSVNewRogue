// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Components/NRCHumanoidAnimationController.hpp"
#include "Components/NRCRender.hpp"
#include "Components/NRCHumanoid.hpp"

using namespace ssvs;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvuj;

namespace nr
{
	using Action = NRCHumanoid::Action;

	NRCHumanoidAnimationController::NRCHumanoidAnimationController(NRCRender& mCRender, NRCHumanoid& mCHumanoid) : cRender(mCRender), cHumanoid(mCHumanoid) { }

	void NRCHumanoidAnimationController::update(FT mFT)
	{
		cRender.setFlippedX(cHumanoid.isFacingLeft());
		cRender.setOffset({0, -1});

		switch(cHumanoid.getAction())
		{
			case Action::Walking: currentAnim = &animWalk; break;
			case Action::Running: currentAnim = &animRun; break;
			case Action::Standing: currentAnim = &animStand; break;
			case Action::Jumping: currentAnim = &animJump; break;
			case Action::Falling: currentAnim = &animFall; break;
			case Action::Crouching: cRender.setOffset({0, -3}); currentAnim = &animCrouch; break;
			case Action::CrouchWalking: cRender.setOffset({0, -3}); currentAnim = &animCrouchWalk; break;
		}

		if(currentAnim == nullptr) return;
		currentAnim->update(mFT);
		const auto& rect(tileset(currentAnim->getTileIndex()));
		for(auto& s : cRender.getSprites()) s.setTextureRect(rect);
	}
}
