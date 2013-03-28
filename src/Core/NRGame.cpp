#include "Core/NRDependencies.h"
#include "Core/NRAssets.h"
#include "Core/NRGame.h"
#include "Utils/NRUtils.h"

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvuj;
using namespace sses;
using namespace ssvs;
using namespace ssvsc;
using namespace ssvsc::Utils;

namespace nr
{
	NRGame::NRGame(GameWindow& mGameWindow, NRAssets& mAssets) : gameWindow(mGameWindow), assets(mAssets), world(createResolver<Retro>(), createSpatial<Grid>(600, 600, 1600, 300)),
		factory{assets, *this, manager, world}
	{
		gameState.onUpdate += [&](float mFrameTime){ update(mFrameTime); };
		gameState.onDraw += [&]{ draw(); };

		initInput();
		initLevel();
	}
	
	void NRGame::initInput()
	{
		using k = Keyboard::Key;
		using t = Input::Trigger::Types;
		
		gameState.addInput({{k::Escape}}, [&](float){ gameWindow.stop(); });

		gameState.addInput({{k::A}},	[=](float mFrameTime){ camera.move(Vector2f{-10, 0} * mFrameTime); });
		gameState.addInput({{k::D}},	[=](float mFrameTime){ camera.move(Vector2f{10, 0} * mFrameTime); });
		gameState.addInput({{k::W}},	[=](float mFrameTime){ camera.move(Vector2f{0, -10} * mFrameTime); });
		gameState.addInput({{k::S}},	[=](float mFrameTime){ camera.move(Vector2f{0, 10} * mFrameTime); });
		gameState.addInput({{k::Q}},	[=](float mFrameTime){ camera.zoom(pow(1.1f, mFrameTime)); });
		gameState.addInput({{k::E}},	[=](float mFrameTime){ camera.zoom(pow(0.9f, mFrameTime)); });

		gameState.addInput({{k::Left}},		[&](float){ inputX = -1; });
		gameState.addInput({{k::Right}},	[&](float){ inputX = 1; });
		gameState.addInput({{k::Up}},		[&](float){ inputY = -1; });
		gameState.addInput({{k::Down}},		[&](float){ inputY = 1; });
		gameState.addInput({{k::Z}},		[&](float){ inputShoot = 1; });
		gameState.addInput({{k::X}},		[&](float){ inputJump = 1; });
		gameState.addInput({{k::LShift}},	[&](float){ inputWalk = 1; });

		gameState.addInput({{k::Num1}}, [&](float)
		{
			auto mousePosition = camera.getMousePosition() * 100.f;
			factory.createWall(Vector2i(mousePosition.x, mousePosition.y));
		}, t::SINGLE);
	}
	void NRGame::initLevel()
	{
		int tilesX{320 / 16}, tilesY{240 / 16};
		
		for(int iY{0}; iY < tilesY; iY++)
			for(int iX{0}; iX < tilesX; iX++)
				if(iX == 0 || iY == 0 || iX == tilesX - 1 || iY == tilesY - 1)
					factory.createWall({1600 * iX + 800, 1600 * iY + 800});

		factory.createWall({1600 * 7 + 800, 1600 * 7 + 800});
		factory.createWall({1600 * 8 + 800, 1600 * 7 + 800});
		factory.createWall({1600 * 2 + 800, 1600 * 10 + 800});
		factory.createWall({1600 * 3 + 800, 1600 * 10 + 800});
		
		factory.createWall({1600 * 5, 1600 * 12});
		factory.createWall({1600 * 7, 1600 * 13 + 800 - 1300 + 1});
		factory.createWall({1600 * 7, 1600 * 13 + 800 - 1300 - 1300 - 1 - 1600});
		
		factory.createPlayer({1600 * 4, 1600 * 5});
		factory.createWanderer({1600 * 2, 1600 * 5});
		factory.createWanderer({1600 * 3, 1600 * 5});
		factory.createWanderer({1600 * 8, 1600 * 5});
		factory.createWanderer({1600 * 5, 1600 * 5});
		factory.createWanderer({1600 * 6, 1600 * 5});
		factory.createWanderer({1600 * 7, 1600 * 5});
		factory.createWanderer({1600 * 2, 1600 * 5});
		factory.createWanderer({1600 * 3, 1600 * 5});
		factory.createWanderer({1600 * 8, 1600 * 5});
		factory.createWanderer({1600 * 5, 1600 * 5});
		factory.createWanderer({1600 * 6, 1600 * 5});
		factory.createWanderer({1600 * 7, 1600 * 5});
	}

	void NRGame::update(float mFrameTime)
	{
		gameWindow.setTitle(toStr(gameWindow.getFPS()));
		
		for(auto& cPhysics : manager.getComponents<NRCPhysics>("physics")) if(cPhysics->isAffectedByGravity()) cPhysics->getBody().applyForce({0, 25});
		world.update(mFrameTime); // TODO: update physics with static frametime (consider all options though)
		manager.update(mFrameTime);
		
		inputX = inputY = inputShoot = inputJump = inputWalk = 0;
	}
	void NRGame::draw()
	{
		camera.apply();
		manager.draw();
		camera.unapply();
	}

	void NRGame::render(const Drawable& mDrawable) { gameWindow.draw(mDrawable); }

	// Getters
	GameWindow& NRGame::getGameWindow()	{ return gameWindow; }
	GameState& NRGame::getGameState()	{ return gameState; }
	Manager& NRGame::getManager()		{ return manager; }
	World& NRGame::getWorld()			{ return world; }
	int NRGame::getInputX() 			{ return inputX; }
	int NRGame::getInputY() 			{ return inputY; }
	int NRGame::getInputShoot() 		{ return inputShoot; }
	int NRGame::getInputJump() 			{ return inputJump; }
	int NRGame::getInputWalk() 			{ return inputWalk; }
}
