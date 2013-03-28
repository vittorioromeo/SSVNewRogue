#include "Components/NRCPlayer.h"
#include "Components/NRCPhysics.h"
#include "Components/NRCSensor.h"
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
	
	NRCHumanoid::NRCHumanoid(Entity& mEntity, NRGame& mGame, NRCPhysics& mCPhysics, NRCSensor& mCSensor)
		: Component(mEntity, "humanoid"), game(mGame), cPhysics(mCPhysics), cSensor(mCSensor), body(cPhysics.getBody()),
		  standingHeight{body.getHeight()}
	{
		body.onPreUpdate += [&]{ jumpReady = false; };
		body.onPostUpdate += [&]{ cSensor.setPosition({body.getShape().getX(), body.getShape().getBottom() - cSensor.getBody().getShape().getHalfHeight()}); };
		cPhysics.onResolution += [&](Vector2i mMinIntersection) { if(mMinIntersection.y < 0) jumpReady = true; };
	}

	void NRCHumanoid::update(float)
	{							
		Vector2f velocity{body.getVelocity()};
		if(cPhysics.isCrushedTop() && cPhysics.isCrushedBottom()) { canUncrouch = false; crouch(true); }
		else canUncrouch = true;
				
		body.setHeight(crouching ? crouchingHeight : standingHeight);	
		
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
		if(cSensor.isActive() || !canUncrouch) return;
		if(crouching) body.setPosition(body.getPosition() - Vector2i{0, (standingHeight - crouchingHeight) / 2});
		crouching = false; 
	}
	void NRCHumanoid::crouch(bool mForce)
	{
		if(!mForce && isInAir()) return;
		if(!crouching) body.setPosition(body.getPosition() + Vector2i{0, (standingHeight - crouchingHeight) / 2});
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
