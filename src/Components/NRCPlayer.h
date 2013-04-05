#ifndef SSVNR_COMPONENTS_PLAYER
#define SSVNR_COMPONENTS_PLAYER

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;
	class NRCHumanoid;
	
	class NRCPlayer : public sses::Component
	{
		private:
			NRGame& game;
			NRCHumanoid& cHumanoid;

		public:
			NRCPlayer(sses::Entity& mEntity, NRGame& mGame, NRCHumanoid& mCHumanoid);
			
			void update(float) override;
	};
	
	class NRCWanderer : public sses::Component
	{
		private:
			NRGame& game;
			NRCHumanoid& cHumanoid;
			float time{200};
			int dir{0};
			bool tired{false};

		public:
			NRCWanderer(sses::Entity& mEntity, NRGame& mGame, NRCHumanoid& mCHumanoid);
			
			void update(float mFrameTime) override;
	};
	
	class NRCTrail : public sses::Component
	{
		private:
			NRGame& game;
			float life{75};
			sf::Vector2i a, b;
			sf::Color color;
			sf::VertexArray vertices;
			
		public:
			NRCTrail(sses::Entity& mEntity, NRGame& mGame, sf::Vector2i mA, sf::Vector2i mB, sf::Color mColor);
			
			void update(float mFrameTime) override;
			void draw() override;
	};
}

#endif
