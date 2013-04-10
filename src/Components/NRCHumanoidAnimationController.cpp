#include "Components/NRCHumanoidAnimationController.h"
#include "Components/NRCRender.h"
#include "Components/NRCHumanoid.h"

using namespace ssvs;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvsc;
using namespace ssvu;
using namespace ssvuj;

namespace nr
{
	using Action = NRCHumanoid::Action;

	NRCHumanoidAnimationController::NRCHumanoidAnimationController(Entity& mEntity, NRCRender& mCRender, NRCHumanoid& mCHumanoid) : Component(mEntity, "humanoidAnimationController"),
		cRender(mCRender), cHumanoid(mCHumanoid) { }

	void NRCHumanoidAnimationController::update(float mFrameTime)
	{
		cRender.setFlippedX(cHumanoid.isFacingLeft());
		cRender.setOffset({0, -1});

		Action action(cHumanoid.getAction());
		if(action == Action::STANDING)				currentAnim = &animStand;
		else if(action == Action::RUNNING)			currentAnim = &animRun;
		else if(action == Action::WALKING)			currentAnim = &animWalk;
		else if(action == Action::FALLING)			currentAnim = &animFall;
		else if(action == Action::JUMPING)			currentAnim = &animJump;
		else if(action == Action::CROUCHING)		{ cRender.setOffset({0, -3}); currentAnim = &animCrouch; }
		else if(action == Action::CROUCHWALKING)	{ cRender.setOffset({0, -3}); currentAnim = &animCrouchWalk; }

		if(currentAnim == nullptr) return;
		currentAnim->update(mFrameTime);
		for(auto& sprite : cRender.getSprites()) sprite.setTextureRect(tileset[currentAnim->getCurrentLabel()]);
	}
}
