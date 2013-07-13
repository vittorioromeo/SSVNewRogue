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

			inline void addSprite(const sf::Sprite& mSprite) { sprites.push_back(mSprite); }

			// Getters
			inline bool isFlippedX() const					{ return flippedY; }
			inline bool isFlippedY() const					{ return flippedX; }
			inline std::vector<sf::Sprite>& getSprites()	{ return sprites; }

			// Setters
			inline void setRotation(float mDegrees)				{ for(auto& s : sprites) s.setRotation(mDegrees); }
			inline void setFlippedX(bool mFlippedX)				{ flippedX = mFlippedX; }
			inline void setFlippedY(bool mFlippedY)				{ flippedY = mFlippedY; }
			inline void setScaleWithBody(bool mScaleWithBody)	{ scaleWithBody = mScaleWithBody; }
			inline void setOffset(ssvs::Vec2f mOffset)			{ offset = mOffset; }
	};

	// TODO: find a way to add sprites with individual offsets, and still have a global offset (RenderData struct?)
}

#endif
