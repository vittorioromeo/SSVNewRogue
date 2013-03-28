#ifndef SSVNR_CORE_FACTORY
#define SSVNR_CORE_FACTORY

#include "Core/NRDependencies.h"
#include "Components/NRCPhysics.h"
#include "Components/NRCRender.h"
#include "Components/NRCPlayer.h"
#include "Components/NRCAnimationController.h"
#include "Components/NRCHumanoid.h"
#include "Components/NRCSensor.h"

namespace nr
{
	class NRAssets;
	class NRGame;
	
	class NRFactory
	{
		private:
			NRAssets& assets;
			NRGame& game;
			sses::Manager& manager;
			ssvsc::World& world;
			
		public:
			NRFactory(NRAssets& mAssets, NRGame& mGame, sses::Manager& mManager, ssvsc::World& mWorld);
			
			sses::Entity& createWall(sf::Vector2i mPosition);
			sses::Entity& createPlayer(sf::Vector2i mPosition);
	};
}

#endif
