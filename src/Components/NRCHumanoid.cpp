// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Components/NRCPlayer.hpp"
#include "Components/NRCHumanoid.hpp"
#include "Components/NRCPhysics.hpp"
#include "Core/NRGame.hpp"
#include "Utils/NRUtils.hpp"

using namespace ssvs;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvu;

namespace nr
{
	using Action = NRCHumanoid::Action;

	NRCHumanoid::NRCHumanoid(sses::Entity& mE, NRCPhysics& mCPhysics) : sses::Component{mE}, cPhysics(mCPhysics), unCrouchSensor{cPhysics, Vec2i{700, 1300}},
		autoCrouchTopSensor{cPhysics, Vec2i(100, 100)}, autoCrouchBottomSensor{cPhysics, Vec2i(100, 100)}, body(cPhysics.getBody()), standingHeight{body.getHeight()}
	{
		const auto& s(body.getShape());
		const auto& ucsShape(unCrouchSensor.getSensor().getShape());
		const auto& actsShape(autoCrouchTopSensor.getSensor().getShape());
		const auto& acbsShape(autoCrouchBottomSensor.getSensor().getShape());

		body.onPreUpdate += [this]{ jumpReady = false; };
		body.onPostUpdate += [&]
		{
			unCrouchSensor.setPosition({s.getX(), s.getBottom() - ucsShape.getHalfHeight()});

			int atcsX{s.getLeft() - actsShape.getHalfWidth()};
			int abcsX{s.getLeft() - acbsShape.getHalfWidth()};
			int atcsY{s.getBottom() - standingHeight + actsShape.getHalfHeight()};
			int abcsY{s.getBottom() - crouchingHeight + acbsShape.getHalfHeight()};
			if(!facingLeft)
			{
				atcsX = s.getRight() + actsShape.getHalfWidth();
				abcsX = s.getRight() + acbsShape.getHalfWidth();
			}

			autoCrouchTopSensor.setPosition({atcsX, atcsY});
			autoCrouchBottomSensor.setPosition({abcsX, abcsY});
		};
		cPhysics.onResolution += [&](const Vec2i& mMinIntersection) { if(mMinIntersection.y < 0) jumpReady = true; };
	}

	void NRCHumanoid::update(FT)
	{
		const auto& velocity(body.getVelocity());
		if(!isInAir() && autoCrouchTopSensor.isActive() && !autoCrouchBottomSensor.isActive())
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
				if(velocity.x == 0) action = Action::Standing;
				else if(abs(velocity.x) >= runSpeed) action = Action::Running;
				else if(abs(velocity.x) >= walkSpeed) action = Action::Walking;
			}
			else action = velocity.x == 0 ? Action::Crouching : Action::CrouchWalking;
		}
		else
		{
			if(velocity.y > 0) action = Action::Falling;
			else if(velocity.y < 0) action = Action::Jumping;
		}
	}

	void NRCHumanoid::unCrouch()
	{
		if(unCrouchSensor.isActive() || autoCrouching) return;
		if(crouching)
		{
			body.setPosition(body.getPosition() - Vec2i{0, (standingHeight - crouchingHeight) / 2});
			body.setHeight(standingHeight);
		}
		crouching = false;
	}
	void NRCHumanoid::crouch(bool mForce)
	{
		if(!mForce && isInAir()) return;
		if(!crouching)
		{
			body.setPosition(body.getPosition() + Vec2i{0, (standingHeight - crouchingHeight) / 2});
			body.setHeight(crouchingHeight);
		}
		crouching = true;
	}
	void NRCHumanoid::move(int mDirection, bool mWalk)
	{
		float speed{mWalk ? walkSpeed : runSpeed};
		//speed += additionalSpeed.getComputed();
		if(crouching) speed = crouchSpeed;
		body.setVelocityX(speed * mDirection);
	}
	void NRCHumanoid::jump()
	{
		if(crouching || isInAir() || !jumpReady) return;
		body.setVelocityY(-jumpSpeed);
	}
}
