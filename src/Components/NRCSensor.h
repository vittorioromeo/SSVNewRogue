#ifndef SSVNR_COMPONENTS_SENSOR
#define SSVNR_COMPONENTS_SENSOR

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;
	class NRCPhysics;
	
	class NRCSensor : public sses::Component
	{
		private:
			NRGame& game;
			ssvsc::World& world;
			NRCPhysics& parent;
			sf::Vector2i position;
			ssvsc::Body& body;
			bool active{false};
		
		public:
			ssvu::Delegate<void, sses::Entity&> onDetection;
	
			NRCSensor(sses::Entity& mEntity, NRGame& mGame, ssvsc::World& mWorld, NRCPhysics& mParent, sf::Vector2i mSize);
			~NRCSensor();
			
			// Setters
			void setPosition(sf::Vector2i mOffset);
			
			// Getters
			ssvsc::Body& getBody();
			bool isActive();
	};
}

#endif
