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
			bool flippedX{false}, flippedY{false};
			sf::Vector2f offset;

		public:
			NRCRender(sses::Entity& mEntity, NRGame& mGame, ssvsc::Body& mBody);

			void update(float) override;
			void draw() override;
			
			void addSprite(sf::Sprite mSprite);

			// Getters
			bool isFlippedX() const;
			bool isFlippedY() const;
			std::vector<sf::Sprite>& getSprites();
			
			// Setters
			void setRotation(float mDegrees);
			void setFlippedX(bool mFlippedX);
			void setFlippedY(bool mFlippedY);
			void setOffset(sf::Vector2f mOffset);
	};
}

#endif
