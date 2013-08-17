// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_COMPONENTS_RENDER
#define SSVNR_COMPONENTS_RENDER

#include "Core/NRDependencies.h"
#include "Core/NRGame.h"
#include "Utils/NRUtils.h"

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
			NRCRender(NRGame& mGame, ssvsc::Body& mBody) : game(mGame), body(mBody) { }

			void update(float) override
			{
				const auto& position(toPixels(body.getPosition()));
				for(auto& s : sprites)
				{
					const auto& rect(s.getTextureRect());
					s.setOrigin({rect.width / 2.f, rect.height / 2.f});
					s.setPosition(position + offset);
					s.setScale(flippedX ? -1 : 1, flippedY ? -1 : 1);

					if(!scaleWithBody) continue;
					const auto& size(toPixels(body.getSize()));
					s.setScale(size.x / 16.f, size.y / 16.f); // TODO: what is 16?
				}
			}
			inline void draw() override { for(const auto& s : sprites) game.render(s); }

			inline void addSprite(const sf::Sprite& mSprite) { sprites.push_back(mSprite); }

			inline bool isFlippedX() const					{ return flippedY; }
			inline bool isFlippedY() const					{ return flippedX; }
			inline std::vector<sf::Sprite>& getSprites()	{ return sprites; }

			inline void setRotation(float mDegrees)				{ for(auto& s : sprites) s.setRotation(mDegrees); }
			inline void setFlippedX(bool mFlippedX)				{ flippedX = mFlippedX; }
			inline void setFlippedY(bool mFlippedY)				{ flippedY = mFlippedY; }
			inline void setScaleWithBody(bool mScaleWithBody)	{ scaleWithBody = mScaleWithBody; }
			inline void setOffset(ssvs::Vec2f mOffset)			{ offset = mOffset; }
	};

	// TODO: find a way to add sprites with individual offsets, and still have a global offset (RenderData struct?)
}

#endif
