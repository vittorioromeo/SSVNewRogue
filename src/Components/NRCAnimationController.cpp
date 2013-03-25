#include "Components/NRCAnimationController.h"
#include "Components/NRCPhysics.h"
#include "Components/NRCRender.h"
#include "Components/NRCPlayer.h"
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
	using Action = NRCPlayer::Action;
	
	NRCAnimationController::NRCAnimationController(Entity& mEntity, NRCPhysics& mCPhysics, NRCRender& mCRender, NRCPlayer& mCPlayer) : Component(mEntity, "animationController"), 
		cPhysics(mCPhysics), cRender(mCRender), cPlayer(mCPlayer), body(cPhysics.getBody()), testTileset(getTilesetFromJSON(getRootFromFile("Data/Tilesets/tilesetHuman.json")))
	{ 
		testStandAnim.addStep({"stand", 100});
		testJumpAnim.addStep({"run0", 100});
		testFallAnim.addStep({"run8", 100});
		testCrouchAnim.addStep({"crouch", 100});
		testRunAnim.addSteps({"run0", "run1", "run2", "run3", "run4", "run5", "run6", "run7", "run8"}, 5);
		testRunAnim.setSpeed(1.4f);
		testWalkAnim.addSteps({"walk0", "walk1", "walk2", "walk3"}, 5);
		testWalkAnim.setSpeed(0.4f);
		testCrouchWalkAnim.addSteps({"cwalk0", "cwalk1", "cwalk2", "cwalk3"}, 5);
		testCrouchWalkAnim.setSpeed(0.5f);
	}
	
	void NRCAnimationController::update(float mFrameTime)
	{		
		cRender.setFlippedX(cPlayer.isFacingLeft());
		cRender.setOffset({0, 0});
		
		Action action(cPlayer.getAction());
		if(action == Action::STANDING) testCurrentAnim = &testStandAnim;
		else if(action == Action::RUNNING) testCurrentAnim = &testRunAnim;
		else if(action == Action::WALKING) testCurrentAnim = &testWalkAnim;
		else if(action == Action::FALLING) testCurrentAnim = &testFallAnim;
		else if(action == Action::JUMPING) testCurrentAnim = &testJumpAnim;
		else if(action == Action::CROUCHING) 
		{
			cRender.setOffset({0, -4.f});
			testCurrentAnim = &testCrouchAnim;
		}
		else if(action == Action::CROUCHWALKING)
		{
			cRender.setOffset({0, -4.f});
			testCurrentAnim = &testCrouchWalkAnim;
		}
		
		if(testCurrentAnim == nullptr) return; 
		testCurrentAnim->update(mFrameTime);
		for(auto& sprite : cRender.getSprites()) sprite.setTextureRect(testTileset.getTextureRect(testCurrentAnim->getCurrentLabel()));	
	}
	
	void NRCAnimationController::draw()
	{
		
	}
}
