// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <sstream>
#include <string>
#include "Core/NRDependencies.h"
#include "Core/NRAssets.h"
#include "Core/NRGame.h"
#include "Utils/NRUtils.h"

#include "Components/NRCHumanoid.h"

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvuj;
using namespace sses;
using namespace ssvs;
using namespace ssvs::Utils;
using namespace ssvsc;
using namespace ssvsc::Utils;

namespace nr
{
	NRGame::NRGame(GameWindow& mGameWindow, NRAssets& mAssets) : gameWindow(mGameWindow), assets(mAssets), factory{assets, *this, manager, world},
		world(createResolver<Retro>(), createSpatial<Grid>(1000, 1000, 1000, 500)), grid(world.getSpatial<Grid>()), debugText{assets().getBitmapFont("limeStroked")}
	{
		debugText.setTracking(-3);

		gameState.onUpdate += [&](float mFrameTime){ update(mFrameTime); };
		gameState.onDraw += [&]{ draw(); };

		initInput();
		initLevel();
		initDebugGrid();
	}

	void NRGame::initInput()
	{
		using k = Keyboard::Key;
		using b = Mouse::Button;
		using t = Input::Trigger::Type;

		gameState.addInput({{k::Escape}}, [&](float){ gameWindow.stop(); });

		gameState.addInput({{k::A}}, [=](float mFrameTime){ camera.move(Vector2f{-10, 0} * mFrameTime); });
		gameState.addInput({{k::D}}, [=](float mFrameTime){ camera.move(Vector2f{10, 0} * mFrameTime); });
		gameState.addInput({{k::W}}, [=](float mFrameTime){ camera.move(Vector2f{0, -10} * mFrameTime); });
		gameState.addInput({{k::S}}, [=](float mFrameTime){ camera.move(Vector2f{0, 10} * mFrameTime); });
		gameState.addInput({{k::Q}}, [=](float mFrameTime){ camera.zoom(pow(1.1f, mFrameTime)); });
		gameState.addInput({{k::E}}, [=](float mFrameTime){ camera.zoom(pow(0.9f, mFrameTime)); });

		add2StateInput(gameState, {{k::X}}, inputJump);
		add2StateInput(gameState, {{k::LShift}}, inputWalk);

		add3StateInput(gameState, {{k::Left}}, {{k::Right}}, inputX);
		add3StateInput(gameState, {{k::Up}}, {{k::Down}}, inputY);

		gameState.addInput({{k::Z}, {b::Left}},	[&](float){ inputShoot = 1; }, t::Single);

		gameState.addInput({{k::Num1}}, [&](float){ factory.createWall(getMousePosition()); }, t::Single);
		gameState.addInput({{k::Num2}}, [&](float){ factory.createWanderer(getMousePosition()); });
		gameState.addInput({{k::Num3}}, [&](float){ factory.createPlayer(getMousePosition()); }, t::Single);
		gameState.addInput({{k::Num4}}, [&](float)
		{
			auto index(grid.getIndex(getMousePosition()));
			auto count(grid.getCell(index.x, index.y).getBodies().size());
			log(toStr(index.x) + " " + toStr(index.y) + "  :: " + toStr(count));
			debugGrid[index.x + grid.getOffset()][index.y + grid.getOffset()] = 1;
		}, t::Single);
		gameState.addInput({{k::Num5}}, [&](float){ clearDebugGrid(); }, t::Single);
	}
	void NRGame::initLevel()
	{
		int tilesX{320 / 16}, tilesY{240 / 16};
		tilesX = 75;//world.getSpatial<Grid>().getIndexXMax() / 4;
		tilesY = 75;//world.getSpatial<Grid>().getIndexYMax() / 4;

		for(int iY{0}; iY < tilesY; ++iY)
			for(int iX{0}; iX < tilesX; ++iX)
				if(iX == 0 || iY == 0 || iX == tilesX - 1 || iY == tilesY - 1)
					factory.createWall({1600 * iX + 800, 1600 * iY + 800});
				else
				{
					if(getRnd(0, 100) > 50) { factory.createWanderer({1600 * iX + 800, 1600 * iY + 800}); }
					else
					{
						if(getRnd(0, 100) > 50) factory.createWall({1600 * iX + 800, 1600 * iY + 800});

					}
				}

		factory.createWall({1600 * 7 + 800, 1600 * 7 + 800});
		factory.createWall({1600 * 8 + 800, 1600 * 7 + 800});
		factory.createWall({1600 * 2 + 800, 1600 * 10 + 800});
		factory.createWall({1600 * 3 + 800, 1600 * 10 + 800});

		factory.createWall({1600 * 5, 1600 * 12});
		factory.createWall({1600 * 7, 1600 * 13 + 800 - 1300 + 1});
		factory.createWall({1600 * 7, 1600 * 13 + 800 - 1300 - 1300 - 1 - 1600});
		factory.createWall({1600 * 9, 1600 * 12 + 1200});

		auto& player = factory.createPlayer({1600 * 4, 1600 * 5});
		NRCHumanoid& playerCHumanoid = player.getFirstComponent<NRCHumanoid>("humanoid");
		{
			ssvrpg::Modifier<int>& b = playerCHumanoid.modifierManager.create();
			Timeline& buff = timelineManager.create();
			buff.append<Do>([&]{ b.onCompute += [&](ssvrpg::Value<int>&, int& mCurrent){ mCurrent += 200; }; playerCHumanoid.additionalSpeed.addModifier(b); });
			buff.append<Wait>(100);
			buff.append<Do>([&]{ playerCHumanoid.additionalSpeed.removeModifier(b); playerCHumanoid.modifierManager.del(&b); playerCHumanoid.modifierManager.cleanUp(); });
		}
		{
			ssvrpg::Modifier<int>& b = playerCHumanoid.modifierManager.create();
			Timeline& buff = timelineManager.create();
			buff.append<Do>([&]{ b.onCompute += [&](ssvrpg::Value<int>&, int& mCurrent){ mCurrent += 200; }; playerCHumanoid.additionalSpeed.addModifier(b); });
			buff.append<Wait>(200);
			buff.append<Do>([&]{ playerCHumanoid.additionalSpeed.removeModifier(b); playerCHumanoid.modifierManager.del(&b); playerCHumanoid.modifierManager.cleanUp(); });
		}
		{
			ssvrpg::Modifier<int>& b = playerCHumanoid.modifierManager.create();
			Timeline& buff = timelineManager.create();
			buff.append<Do>([&]{ b.onCompute += [&](ssvrpg::Value<int>&, int& mCurrent){ mCurrent += 200; }; playerCHumanoid.additionalSpeed.addModifier(b); });
			buff.append<Wait>(300);
			buff.append<Do>([&]{ playerCHumanoid.additionalSpeed.removeModifier(b); playerCHumanoid.modifierManager.del(&b); playerCHumanoid.modifierManager.cleanUp(); });
		}
	}

	void NRGame::initDebugGrid()
	{
		for(int iX{0}; iX < grid.getColumns(); ++iX)
		{
			debugGrid.push_back(vector<int>(grid.getRows()));
			for(int iY{0}; iY < grid.getRows(); ++iY) debugGrid[iX][iY] = 0;
		}
	}

	void NRGame::update(float mFrameTime)
	{
		world.update(mFrameTime);
		manager.update(mFrameTime);
		timelineManager.update(mFrameTime);
		updateDebugText(mFrameTime);
		inputShoot = 0;
	}
	void NRGame::updateDebugText(float mFrameTime)
	{
		ostringstream s;
		const auto& entities(manager.getEntities());
		const auto& bodies(world.getBodies());
		int componentCount{0}, dynamicBodiesCount{0};
		for(const auto& e : entities) componentCount += e->getComponents().size();
		for(const auto& b : bodies) if(!b->isStatic()) ++dynamicBodiesCount;

		s << "FPS: "				<< toStr(gameWindow.getFPS()) << endl;
		s << "FrameTime: "			<< toStr(mFrameTime) << endl;
		s << "Bodies(all): "		<< toStr(bodies.size()) << endl;
		s << "Bodies(static): "		<< toStr(bodies.size() - dynamicBodiesCount) << endl;
		s << "Bodies(dynamic): "	<< toStr(dynamicBodiesCount) << endl;
		s << "Sensors: "			<< toStr(world.getSensors().size()) << endl;
		s << "Entities: "			<< toStr(entities.size()) << endl;
		s << "Components: "			<< toStr(componentCount) << endl;

		debugText.setString(s.str());
	}
	void NRGame::drawDebugGrid()
	{
		debugGridVertices.clear();
		for(int iX{0}; iX < grid.getColumns(); ++iX)
			for(int iY{0}; iY < grid.getRows(); ++iY)
			{
				if(debugGrid[iX][iY] == 0) continue;

				Color color{255, 255, 0, 90};

				if((iX % 2 == 0 && iY % 2 != 0) || (iX % 2 != 0 && iY % 2 == 0))
				{
					Color tempColor{color};
					color.r = color.b; color.b = tempColor.r;
				}

				int oIX{iX - grid.getOffset()};
				int oIY{iY - grid.getOffset()};

				Vector2i a{grid.getCellSize() * oIX, grid.getCellSize() * oIY};
				Vector2i b{grid.getCellSize() * (oIX + 1), grid.getCellSize() * oIY};
				Vector2i c{grid.getCellSize() * (oIX + 1), grid.getCellSize() * (oIY + 1)};
				Vector2i d{grid.getCellSize() * oIX, grid.getCellSize() * (oIY + 1)};
				debugGridVertices.append({toPixels(a), color});
				debugGridVertices.append({toPixels(b), color});
				debugGridVertices.append({toPixels(c), color});
				debugGridVertices.append({toPixels(d), color});
			}
		render(debugGridVertices);
	}

	void NRGame::draw()
	{
		camera.apply();
		if(getGameWindow().isKeyPressed(Keyboard::Key::F)) manager.draw();
		//drawDebugGrid();
		camera.unapply();
		render(debugText);
	}

	void NRGame::render(const Drawable& mDrawable) { gameWindow.draw(mDrawable); }

	void NRGame::setDebugGrid(int mX, int mY)
	{
		debugGrid[mX + grid.getOffset()][mY + grid.getOffset()] = 1;
	}
	void NRGame::clearDebugGrid()
	{
		for(int iX{0}; iX < grid.getColumns(); ++iX)
			for(int iY{0}; iY < grid.getRows(); ++iY)
				debugGrid[iX][iY] = 0;
	}

	// Getters
	GameWindow& NRGame::getGameWindow()	{ return gameWindow; }
	GameState& NRGame::getGameState()	{ return gameState; }
	Manager& NRGame::getManager()		{ return manager; }
	World& NRGame::getWorld()			{ return world; }
	NRFactory& NRGame::getFactory()		{ return factory; }
	Vector2i NRGame::getMousePosition() { return toCoords(camera.getMousePosition()); }
	int NRGame::getInputX() 			{ return inputX; }
	int NRGame::getInputY() 			{ return inputY; }
	int NRGame::getInputShoot() 		{ return inputShoot; }
	bool NRGame::getInputJump() 		{ return inputJump; }
	bool NRGame::getInputWalk() 		{ return inputWalk; }
}
