#ifndef SSVNR_COMPONENTS_PHYSICS
#define SSVNR_COMPONENTS_PHYSICS

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;
	
	class NRCPhysics : public sses::Component
	{
		private:
			NRGame& game;
			ssvsc::World& world;
			ssvsc::Body& body;	
			sf::Vector2i lastResolution;
			bool affectedByGravity{true}, crushedLeft{false}, crushedRight{false}, crushedTop{false}, crushedBottom{false};
	
		public:
			ssvu::Delegate<void, sses::Entity&> onDetection;
			ssvu::Delegate<void, sf::Vector2i> onResolution;
	
			NRCPhysics(sses::Entity& mEntity, NRGame& mGame, ssvsc::World& mWorld, bool mIsStatic, sf::Vector2i mPosition, sf::Vector2i mSize, bool mAffectedByGravity = true);
			~NRCPhysics();
	
			// Getters
			ssvsc::Body& getBody();
			sf::Vector2i getLastResolution();
			bool isAffectedByGravity();
			bool isCrushedLeft();
			bool isCrushedRight();
			bool isCrushedTop();
			bool isCrushedBottom();
			
	};
}

#endif
