// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_CORE_FACTORY
#define SSVNR_CORE_FACTORY

#include "Core/NRDependencies.h"

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

			sses::Entity& createTrail(ssvs::Vec2i mA, ssvs::Vec2i mB, sf::Color mColor);
			sses::Entity& createWall(ssvs::Vec2i mPosition);
			sses::Entity& createHumanoid(ssvs::Vec2i mPosition);
			sses::Entity& createPlayer(ssvs::Vec2i mPosition);
			sses::Entity& createWanderer(ssvs::Vec2i mPosition);
	};
}

#endif
