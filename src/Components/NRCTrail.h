// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_COMPONENTS_TRAIL
#define SSVNR_COMPONENTS_TRAIL

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;
	class NRCHumanoid;

	class NRCTrail : public sses::Component
	{
		private:
			NRGame& game;
			float life{75};
			ssvs::Vec2i a, b;
			sf::Color color;
			ssvs::VertexVector<sf::PrimitiveType::Lines> vertices;

		public:
			NRCTrail(NRGame& mGame, const ssvs::Vec2i& mA, const ssvs::Vec2i& mB, sf::Color mColor);

			void update(float mFT) override;
			void draw() override;
	};
}

#endif
