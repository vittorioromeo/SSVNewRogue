#ifndef SSVNR_COMPONENTS_PLAYER
#define SSVNR_COMPONENTS_PLAYER

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;
	class NRCPhysics;
	
	class NRCPlayer : public sses::Component
	{
		public:
			enum class Action{ WALKING, RUNNING, CROUCHING, CROUCHWALKING, STANDING, JUMPING, FALLING };
			
		private:
			NRGame& game;
			NRCPhysics& cPhysics;
			ssvsc::Body& body;
			ssvsc::Body& crouchSensor;
			Action action;
			bool facingLeft{false}, jumpReady{false}, crouching{false}, canUncrouch{false};
			float crouchSpeed{40.f}, walkSpeed{50.f}, runSpeed{150.f}, jumpSpeed{420.f};

		public:
			NRCPlayer(sses::Entity& mEntity, ssvsc::World& mWorld, NRGame& mGame, NRCPhysics& mCPhysics);
			~NRCPlayer();
			
			void update(float) override;
			
			// Getters
			bool isFacingLeft();
			bool isJumpReady();
			Action getAction();
	};
	
	// TODO: split this class in two - one for all humanoid characters and one for player controls
	// TODO: consider alternatives to a sensor (NRCSensor component? Ignore resolution for certain Y values when crouched?)
}

#endif
