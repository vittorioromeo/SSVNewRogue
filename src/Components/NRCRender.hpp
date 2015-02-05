// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_COMPONENTS_RENDER
#define SSVNR_COMPONENTS_RENDER

#include "Core/NRDependencies.hpp"
#include "Core/NRGame.hpp"
#include "Utils/NRUtils.hpp"

namespace nr
{
	class NRGame;

	class NRCRender : public sses::Component
	{
		private:
			NRGame& game;
			Body& body;
			std::vector<sf::Sprite> sprites;
			bool flippedX{false}, flippedY{false}, scaleWithBody{false};
			ssvs::Vec2f offset;

		public:
			NRCRender(sses::Entity& mE, NRGame& mGame, Body& mBody) : sses::Component{mE}, game(mGame), body(mBody) { }

			void update(FT) override
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
					s.setScale(size.x / rect.width, size.y / rect.height);
				}
			}
			inline void draw() override { for(const auto& s : sprites) game.render(s); }

			template<typename... TArgs> inline void emplaceSprite(TArgs&&... mArgs) { sprites.emplace_back(FWD(mArgs)...); }

			inline bool isFlippedX() const					{ return flippedY; }
			inline bool isFlippedY() const					{ return flippedY; }
			inline std::vector<sf::Sprite>& getSprites()	{ return sprites; }

			inline void setRotation(float mDegrees)				{ for(auto& s : sprites) s.setRotation(mDegrees); }
			inline void setFlippedX(bool mFlippedX)				{ flippedX = mFlippedX; }
			inline void setFlippedY(bool mFlippedY)				{ flippedY = mFlippedY; }
			inline void setScaleWithBody(bool mScaleWithBody)	{ scaleWithBody = mScaleWithBody; }
			inline void setOffset(const ssvs::Vec2f& mOffset)	{ offset = mOffset; }
	};
}

#endif
