// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_CORE_GAME
#define SSVNR_CORE_GAME

#include "Core/NRDependencies.h"
#include "Core/NRFactory.h"

namespace nr
{
	class NRAssets;

	class NRGame
	{
		private:
			ssvs::GameWindow& gameWindow;
			ssvs::Camera camera{gameWindow, {{800 / 2 - 200, 600 / 2 - 150}, {400, 300}}};
			NRAssets& assets;
			NRFactory factory;
			ssvs::GameState gameState;
			ssvsc::World world; // BUG: world must be destroyed after manager, find a way to make this not required!
			ssvsc::Grid& grid;
			sses::Manager manager;

			ssvs::BitmapText debugText;

			ssvu::TimelineManager timelineManager;

			int inputX{0}, inputY{0}, inputShoot{0};
			bool inputJump{false}, inputWalk{false};
			std::vector<std::vector<int>> debugGrid;
			sf::VertexArray debugGridVertices{sf::PrimitiveType::Quads};

			void initInput();
			void initLevel();
			void initDebugGrid();

		public:
			NRGame(ssvs::GameWindow& mGameWindow, NRAssets& mAssets);

			void update(float mFrameTime);
			void updateDebugText(float mFrameTime);
			void drawDebugGrid();
			void draw();

			void render(const sf::Drawable& mDrawable);
			void setDebugGrid(int mX, int mY);
			void clearDebugGrid();

			// Getters
			ssvs::GameWindow& getGameWindow();
			ssvs::GameState& getGameState();
			sses::Manager& getManager();
			ssvsc::World& getWorld();
			NRFactory& getFactory();
			ssvs::Vec2i getMousePosition();
			int getInputX();
			int getInputY();
			int getInputShoot();
			bool getInputJump();
			bool getInputWalk();
	};
}

#endif
