// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_COMPONENTS_RENDER
#define SSVNR_COMPONENTS_RENDER

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;

	class NRCRender : public sses::Component
	{
		private:
			NRGame& game;
			ssvsc::Body& body;
			std::vector<sf::Sprite> sprites;
			bool flippedX{false}, flippedY{false}, scaleWithBody{false};
			ssvs::Vec2f offset;

		public:
			NRCRender(NRGame& mGame, ssvsc::Body& mBody);

			void update(float) override;
			void draw() override;

			void addSprite(const sf::Sprite& mSprite);

			// Getters
			bool isFlippedX() const;
			bool isFlippedY() const;
			std::vector<sf::Sprite>& getSprites();

			// Setters
			void setRotation(float mDegrees);
			void setFlippedX(bool mFlippedX);
			void setFlippedY(bool mFlippedY);
			void setScaleWithBody(bool mScaleWithBody);
			void setOffset(ssvs::Vec2f mOffset);
	};

	// TODO: find a way to add sprites with individual offsets, and still have a global offset (RenderData struct?)
}

#endif
