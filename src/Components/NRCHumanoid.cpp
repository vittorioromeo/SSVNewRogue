#include "Components/NRCPlayer.h"
#include "Components/NRCPhysics.h"
#include "Core/NRGame.h"
#include "Utils/NRUtils.h"

using namespace ssvs;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvsc;
using namespace ssvu;

namespace nr
{
	using Action = NRCHumanoid::Action;
	
	NRCHumanoid::NRCHumanoid(Entity& mEntity, NRGame& mGame, NRCPhysics& mCPhysics) : Component(mEntity, "humanoid"), 
		game(mGame), cPhysics(mCPhysics), unCrouchSensor{cPhysics, Vector2i{700, 1300}},
		autoCrouchTopSensor{cPhysics, Vector2i(100, 100)}, autoCrouchBottomSensor{cPhysics, Vector2i(100, 100)},
		body(cPhysics.getBody()), standingHeight{body.getHeight()}
	{
		body.onPreUpdate += [&]{ jumpReady = false; };
		body.onPostUpdate += [&]
		{ 
			auto& s = body.getShape();
			auto& ucsShape = unCrouchSensor.getBody().getShape();
			auto& actsShape = autoCrouchTopSensor.getBody().getShape();
			auto& acbsShape = autoCrouchBottomSensor.getBody().getShape();
			
			unCrouchSensor.setPosition({s.getX(), s.getBottom() - ucsShape.getHalfHeight()}); 
			
			int autoCrouchTopSensorX{s.getLeft() - actsShape.getHalfWidth()};
			int autoCrouchBottomSensorX{s.getLeft() - acbsShape.getHalfWidth()};
			if(!facingLeft) 
			{
				autoCrouchTopSensorX = s.getRight() + actsShape.getHalfWidth();
				autoCrouchBottomSensorX = s.getRight() + acbsShape.getHalfWidth();
			}
			
			autoCrouchTopSensor.setPosition({autoCrouchTopSensorX, s.getBottom() - standingHeight + actsShape.getHalfHeight()});
			autoCrouchBottomSensor.setPosition({autoCrouchBottomSensorX, s.getBottom() - crouchingHeight + acbsShape.getHalfHeight()});
		};
		cPhysics.onResolution += [&](Vector2i mMinIntersection) { if(mMinIntersection.y < 0) jumpReady = true; };

	}

	void NRCHumanoid::update(float)
	{							
		Vector2f velocity{body.getVelocity()};
		if((cPhysics.isCrushedTop() && cPhysics.isCrushedBottom()) || (!isInAir() && autoCrouchTopSensor.isActive() && !autoCrouchBottomSensor.isActive())) 
		{ 
			autoCrouching = true; 
			crouch(true); 
		}
		else autoCrouching = false;
		
		if(velocity.x > 0) facingLeft = false;
		else if(velocity.x < 0) facingLeft = true;
		
		if(!isInAir()) 
		{
			if(!crouching)
			{				
				if(velocity.x == 0) action = Action::STANDING;
				else if(abs(velocity.x) >= runSpeed) action = Action::RUNNING;
				else if(abs(velocity.x) >= walkSpeed) action = Action::WALKING;
			}
			else
			{				
				if(velocity.x == 0) action = Action::CROUCHING;
				else action = Action::CROUCHWALKING;
			}
		}
		else
		{
			if(velocity.y > 0) action = Action::FALLING;
			else if(velocity.y < 0) action = Action::JUMPING;
		}	
	}
		
	void NRCHumanoid::unCrouch()
	{
		if(unCrouchSensor.isActive() || autoCrouching) return;
		if(crouching) body.setPosition(body.getPosition() - Vector2i{0, (standingHeight - crouchingHeight) / 2});
		body.setHeight(standingHeight);
		crouching = false; 
	}
	void NRCHumanoid::crouch(bool mForce)
	{
		if(!mForce && isInAir()) return;
		if(!crouching) body.setPosition(body.getPosition() + Vector2i{0, (standingHeight - crouchingHeight) / 2});
		body.setHeight(crouchingHeight);
		crouching = true;
	}
	void NRCHumanoid::move(int mDirection, bool mWalk)
	{
		float speed{mWalk ? walkSpeed : runSpeed};
		if(crouching) speed = crouchSpeed;
		body.setVelocityX(speed * mDirection);
	}
	void NRCHumanoid::jump()
	{
		if(crouching || isInAir() || !jumpReady) return;
		body.setVelocityY(-jumpSpeed);
	}
		
	// Getters
	bool NRCHumanoid::isFacingLeft()	{ return facingLeft; }
	bool NRCHumanoid::isJumpReady()		{ return jumpReady; }
	bool NRCHumanoid::isInAir()			{ return body.getShape().getY() != body.getOldShape().getY();}
	Action NRCHumanoid::getAction()		{ return action; }
}
