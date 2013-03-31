#include "Components/NRCHumanoidAnimationController.h"
#include "Components/NRCRender.h"
#include "Components/NRCHumanoid.h"
#include "Utils/NRUtils.h"

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
		cRender(mCRender), cHumanoid(mCHumanoid), tileset(getTilesetFromJSON(getRootFromFile("Data/Tilesets/tilesetHuman.json")))
	{ 
		animStand.addStep({"stand", 100});
		animJump.addStep({"run0", 100});
		animFall.addStep({"run8", 100});
		animCrouch.addStep({"crouch", 100});
		animRun.addSteps({"run0", "run1", "run2", "run3", "run4", "run5", "run6", "run7", "run8"}, 3.2f);
		animWalk.addSteps({"walk0", "walk1", "walk2", "walk3", "walk4", "walk5", "walk6", "walk7"}, 6.5f);
		animCrouchWalk.addSteps({"cwalk0", "cwalk1", "cwalk2", "cwalk3"}, 8.7f);
	}
	
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
		for(auto& sprite : cRender.getSprites()) sprite.setTextureRect(tileset.getTextureRect(currentAnim->getCurrentLabel()));	
	}
}
