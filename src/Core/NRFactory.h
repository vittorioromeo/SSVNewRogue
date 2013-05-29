#ifndef SSVNR_CORE_FACTORY
#define SSVNR_CORE_FACTORY

#include "Core/NRDependencies.h"
#include "Components/NRCPhysics.h"
#include "Components/NRCRender.h"
#include "Components/NRCPlayer.h"
#include "Components/NRCHumanoidAnimationController.h"
#include "Components/NRCHumanoid.h"

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

			sses::Entity& createTrail(sf::Vector2i mA, sf::Vector2i mB, sf::Color mColor);
			sses::Entity& createWall(sf::Vector2i mPosition);
			sses::Entity& createHumanoid(sf::Vector2i mPosition);
			sses::Entity& createPlayer(sf::Vector2i mPosition);
			sses::Entity& createWanderer(sf::Vector2i mPosition);
	};
}

#endif
