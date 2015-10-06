// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_CORE_GAME
#define SSVNR_CORE_GAME

#include "Core/NRDependencies.hpp"
#include "Core/NRFactory.hpp"
#include "Utils/NRUtils.hpp"

namespace nr
{
    class NRAssets;

    class NRGame
    {
    private:
        ssvs::GameWindow& gameWindow;
        ssvs::Camera camera{gameWindow, 2.f};
        NRAssets& assets;
        NRFactory factory;
        ssvs::GameState gameState;
        World
            world; // BUG: world must be destroyed after manager, find a way to
                   // make this not required!
        const typename World::SpatialType& grid;
        sses::Manager manager;

        ssvs::BitmapText debugText;

        ssvu::TimelineManager timelineManager;

        int inputX{0}, inputY{0}, inputShoot{0};
        bool inputJump{false}, inputWalk{false};
        std::vector<std::vector<int>> debugGrid;
        ssvs::VertexVector<sf::PrimitiveType::Quads> debugGridVertices;

        void initInput();
        void initLevel();
        void initDebugGrid();

    public:
        NRGame(ssvs::GameWindow& mGameWindow, NRAssets& mAssets);

        void update(FT mFT);
        void updateDebugText(FT mFT);
        void drawDebugGrid();
        void draw();

        void render(const sf::Drawable& mDrawable);
        void setDebugGrid(int mX, int mY);
        void clearDebugGrid();

        inline ssvs::GameWindow& getGameWindow() { return gameWindow; }
        inline ssvs::GameState& getGameState() { return gameState; }
        inline sses::Manager& getManager() { return manager; }
        inline World& getWorld() { return world; }
        inline NRFactory& getFactory() { return factory; }
        inline ssvs::Vec2i getMousePosition() const
        {
            return toCoords(camera.getMousePosition());
        }
        inline int getInputX() const { return inputX; }
        inline int getInputY() const { return inputY; }
        inline int getInputShoot() const { return inputShoot; }
        inline bool getInputJump() const { return inputJump; }
        inline bool getInputWalk() const { return inputWalk; }
    };
}

#endif
