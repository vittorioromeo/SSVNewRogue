#ifndef SSVNR_CORE_GAME
#define SSVNR_CORE_GAME

#include "Core/NRDependencies.h"
#include "Core/NRFactory.h"
#include "Utils/NRAnimation.h"

namespace nr
{
	class NRAssets;
	
	class NRGame
	{
		private:
			ssvs::GameWindow& gameWindow;
			ssvs::Camera camera{gameWindow, {{320 / 2, 240 / 2}, {320, 240}}};
			NRAssets& assets;
			ssvs::GameState gameState;
			ssvsc::World world; // BUG: world must be destroyed after manager, find a way to make this not required!
			sses::Manager manager;
			NRFactory factory;
			
			int inputX{0}, inputY{0}, inputShoot{0}, inputJump{0}, inputWalk{0};
			float lastFT{0};
			
			void initInput();
			void initLevel();
	
		public:
			NRGame(ssvs::GameWindow& mGameWindow, NRAssets& mAssets);
			
			void update(float mFrameTime);
			void draw();
			
			void render(const sf::Drawable& mDrawable);
			
			// Getters
			ssvs::GameWindow& getGameWindow();
			ssvs::GameState& getGameState();
			sses::Manager& getManager();
			ssvsc::World& getWorld();
			NRFactory& getFactory();
			sf::Vector2i getMousePosition();
			int getInputX();
			int getInputY();
			int getInputShoot();
			int getInputJump();
			int getInputWalk();
	};
}

#endif
