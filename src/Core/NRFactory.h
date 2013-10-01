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
			World& world;

		public:
			NRFactory(NRAssets& mAssets, NRGame& mGame, sses::Manager& mManager, World& mWorld);

			sses::Entity& createTrail(const ssvs::Vec2i& mA, const ssvs::Vec2i& mB, sf::Color mColor);
			sses::Entity& createWall(const ssvs::Vec2i& mPosition);
			sses::Entity& createHumanoid(const ssvs::Vec2i& mPosition);
			sses::Entity& createPlayer(const ssvs::Vec2i& mPosition);
			sses::Entity& createWanderer(const ssvs::Vec2i& mPosition);
	};
}

#endif
