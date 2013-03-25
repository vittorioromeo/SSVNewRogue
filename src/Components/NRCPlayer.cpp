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
	using Action = NRCPlayer::Action;
	
	NRCPlayer::NRCPlayer(Entity& mEntity, World& mWorld, NRGame& mGame, NRCPhysics& mCPhysics)
		: Component(mEntity, "player"), game(mGame), cPhysics(mCPhysics), body(cPhysics.getBody()),
		  crouchSensor(mWorld.create(body.getPosition(), Vector2i{600, 1500}, false))
	{
		crouchSensor.addGroups({"sensor"});
		crouchSensor.addGroupsToCheck({"solid"});
		crouchSensor.addGroupsNoResolve({"solid"});
		
		crouchSensor.onPreUpdate += [&]
		{ 
			canUncrouch = true;
			crouchSensor.setPosition(Vector2i{body.getShape().getX(), body.getShape().getBottom() - crouchSensor.getHeight() / 2 - 100}); 
			
			/*VertexArray v(PrimitiveType::Quads, 4);
			v.append(toPixels(Vector2i{crouchSensor.getShape().getLeft(), crouchSensor.getShape().getTop()}));
			v.append(toPixels(Vector2i{crouchSensor.getShape().getRight(), crouchSensor.getShape().getTop()}));
			v.append(toPixels(Vector2i{crouchSensor.getShape().getRight(), crouchSensor.getShape().getBottom()}));
			v.append(toPixels(Vector2i{crouchSensor.getShape().getLeft(), crouchSensor.getShape().getBottom()}));
			game.render(v);*/
		};
		
		crouchSensor.onDetection += [&](DetectionInfo mDetectionInfo)
		{
			if(mDetectionInfo.userData == nullptr) return;
			Entity* entity(static_cast<Entity*>(mDetectionInfo.userData));
			if(entity != &getEntity()) canUncrouch = false;
		};
		
		//cPhysics.onDetection += [&](Entity& mEntity)
		//{
			//if(mEntity.getId() == "box")  mEntity.destroy();
		//};
		cPhysics.onResolution += [&](Vector2i mMinIntersection)
		{
			if(mMinIntersection.y < 0) jumpReady = true;
			
		};
	}
	NRCPlayer::~NRCPlayer() { crouchSensor.destroy(); }

	void NRCPlayer::update(float)
	{	
		bool inAir{body.getShape().getY() != body.getOldShape().getY()};		
		
		Vector2f oldVelocity{body.getVelocity()}, newVelocity{oldVelocity};

		if(game.getInputY() == 0)
		{
			if(canUncrouch) crouching = false;
		}
		else if(game.getInputY() == 1 && !inAir)
		{
			if(!crouching) body.setPosition(body.getPosition() + Vector2i{0, 400});
			crouching = true;
		}
		
		float speed{game.getInputWalk() ? walkSpeed : runSpeed};
		if(crouching) 
		{
			speed = crouchSpeed;
			body.setHeight(800);	
		}
		else body.setHeight(1600);	
		
		newVelocity.x = speed * game.getInputX();
		if(newVelocity.x > 0) facingLeft = false;
		else if(newVelocity.x < 0) facingLeft = true;
		
		if(!crouching && jumpReady && game.getInputJump() == 1) newVelocity.y = -jumpSpeed;
		body.setVelocity(newVelocity);
				
		Vector2f velocity{body.getVelocity()};
		if(!inAir) 
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
				
		jumpReady = false;
	}
		
	// Getters
	bool NRCPlayer::isFacingLeft()	{ return facingLeft; }
	bool NRCPlayer::isJumpReady()	{ return jumpReady; }
	Action NRCPlayer::getAction()	{ return action; }
}
