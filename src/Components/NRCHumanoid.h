#ifndef SSVNR_COMPONENTS_HUMANOID
#define SSVNR_COMPONENTS_HUMANOID

#include "Core/NRDependencies.h"
#include "Utils/NRSensor.h"

namespace nr
{
	class NRGame;
	class NRCPhysics;
	
	class NRCHumanoid : public sses::Component
	{
		public:
			enum class Action{ WALKING, RUNNING, CROUCHING, CROUCHWALKING, STANDING, JUMPING, FALLING };
			
		private:
			NRGame& game;
			NRCPhysics& cPhysics;
			NRSensor cSensor;
			ssvsc::Body& body;			
			int standingHeight;			
			Action action;
			bool facingLeft{false}, jumpReady{false}, crouching{false}, canUncrouch{false};
			float crouchSpeed{40.f}, walkSpeed{50.f}, runSpeed{150.f}, jumpSpeed{420.f};
			int crouchingHeight{900};

		public:
			NRCHumanoid(sses::Entity& mEntity, NRGame& mGame, NRCPhysics& mCPhysics);
			
			void update(float mFrameTime) override;
			
			void unCrouch();
			void crouch(bool mForce = false);
			void move(int mDirection, bool mWalk);
			void jump();
			
			// Getters
			bool isFacingLeft();
			bool isJumpReady();
			bool isInAir();
			Action getAction();
	};
	
	// TODO: consider alternatives to a sensor (NRCSensor component? Ignore resolution for certain Y values when crouched?)
}

#endif
