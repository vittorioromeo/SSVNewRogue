// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <sstream>
#include <string>
#include "Core/NRDependencies.hpp"
#include "Core/NRAssets.hpp"
#include "Core/NRGame.hpp"


#include "Components/NRCHumanoid.hpp"
#include "Components/NRCTrail.hpp"

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
	NRGame::NRGame(GameWindow& mGameWindow, NRAssets& mAssets) : gameWindow(mGameWindow), assets(mAssets), factory{assets, *this, manager, world},
		world(1000, 1000, 1000, 500), grid(world.getSpatial()), debugText{assets.get<BitmapFont>("limeStroked")}
	{
		debugText.setTracking(-3);

		gameState.onUpdate += [this](FT mFT){ update(mFT); };
		gameState.onDraw += [this]{ draw(); };

		initInput(); initLevel(); initDebugGrid();
	}

	void NRGame::initInput()
	{
		using k = KKey;
		using b = MBtn;
		using t = Input::Type;

		gameState.addInput({{k::Escape}}, [this](FT){ gameWindow.stop(); });

		gameState.addInput({{k::A}}, [=](FT){ camera.pan(-10, 0); });
		gameState.addInput({{k::D}}, [=](FT){ camera.pan(10, 0); });
		gameState.addInput({{k::W}}, [=](FT){ camera.pan(0, -10); });
		gameState.addInput({{k::S}}, [=](FT){ camera.pan(0, 10); });
		gameState.addInput({{k::Q}}, [=](FT){ camera.zoomOut(1.1f); });
		gameState.addInput({{k::E}}, [=](FT){ camera.zoomIn(1.1f); });

		add2StateInput(gameState, {{k::X}}, inputJump);
		add2StateInput(gameState, {{k::LShift}}, inputWalk);

		add3StateInput(gameState, {{k::Left}}, {{k::Right}}, inputX);
		add3StateInput(gameState, {{k::Up}}, {{k::Down}}, inputY);

		gameState.addInput({{k::Z}, {b::Left}},	[this](FT){ inputShoot = 1; }, t::Once);

		gameState.addInput({{k::Num1}}, [this](FT){ factory.createWall(getMousePosition()); }, t::Once);
		gameState.addInput({{k::Num2}}, [this](FT){ factory.createWanderer(getMousePosition()); });
		gameState.addInput({{k::Num3}}, [this](FT){ factory.createPlayer(getMousePosition()); }, t::Once);
		gameState.addInput({{k::Num4}}, [this](FT)
		{
			auto index(grid.getIdx(getMousePosition()));
			auto count(grid.getCell(index.x, index.y).getBodies().size());
			lo() << index.x << " " << index.y << "  :: " << count << endl;
			//debugGrid[index.x + grid.getOffset()][index.y + grid.getOffset()] = 1;
		}, t::Once);
		gameState.addInput({{k::Num5}}, [this](FT){ clearDebugGrid(); }, t::Once);
		gameState.addInput({{k::Num6}}, [this](FT)
		{
			auto body(world.getQuery<QueryType::Point>(getMousePosition()).next());
			if(body == nullptr) return;
			auto entity(static_cast<Entity*>(body->getUserData()));
			if(entity != nullptr) entity->destroy();
		});
	}
	void NRGame::initLevel()
	{
		#define NRGAME_TEST_BENCH

		#ifdef NRGAME_TEST_BENCH
			int tilesX = 100; // world.getSpatial<Grid>().getIdxXMax() / 4;
			int tilesY = 100; // world.getSpatial<Grid>().getIdxYMax() / 4;
		#else
			int tilesX{320 / 16}, tilesY{240 / 16};
		#endif

		for(int iY{0}; iY < tilesY; ++iY)
			for(int iX{0}; iX < tilesX; ++iX)
				if(iX == 0 || iY == 0 || iX == tilesX - 1 || iY == tilesY - 1) factory.createWall({1600 * iX + 800, 1600 * iY + 800});
				else
				{
					#ifdef NRGAME_TEST_BENCH
						if(getRnd(0, 100) > 90) factory.createWanderer({1600 * iX + 800, 1600 * iY + 800});
						else if(getRnd(0, 100) > 90) factory.createWall({1600 * iX + 800, 1600 * iY + 800});
					#endif
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
		NRCHumanoid& playerCHumanoid = player.getComponent<NRCHumanoid>();
		{
			ssvrpg::Modifier<int>& b = playerCHumanoid.modifierManager.create();
			Timeline& buff = timelineManager.create();
			buff.append<Do>([&]{ b.onCompute += [&](ssvrpg::Value<int>&, int& mCurrent){ mCurrent += 200; }; playerCHumanoid.additionalSpeed.addModifier(b); });
			buff.append<Wait>(100);
			buff.append<Do>([&]{ playerCHumanoid.additionalSpeed.removeModifier(b); playerCHumanoid.modifierManager.del(b); playerCHumanoid.modifierManager.refresh(); });
			playerCHumanoid.modifierManager.refresh();
		}
		{
			ssvrpg::Modifier<int>& b = playerCHumanoid.modifierManager.create();
			Timeline& buff = timelineManager.create();
			buff.append<Do>([&]{ b.onCompute += [&](ssvrpg::Value<int>&, int& mCurrent){ mCurrent += 200; }; playerCHumanoid.additionalSpeed.addModifier(b); });
			buff.append<Wait>(200);
			buff.append<Do>([&]{ playerCHumanoid.additionalSpeed.removeModifier(b); playerCHumanoid.modifierManager.del(b); playerCHumanoid.modifierManager.refresh(); });
			playerCHumanoid.modifierManager.refresh();
		}
		{
			ssvrpg::Modifier<int>& b = playerCHumanoid.modifierManager.create();
			Timeline& buff = timelineManager.create();
			buff.append<Do>([&]{ b.onCompute += [&](ssvrpg::Value<int>&, int& mCurrent){ mCurrent += 200; }; playerCHumanoid.additionalSpeed.addModifier(b); });
			buff.append<Wait>(300);
			buff.append<Do>([&]{ playerCHumanoid.additionalSpeed.removeModifier(b); playerCHumanoid.modifierManager.del(b); playerCHumanoid.modifierManager.refresh(); });
			playerCHumanoid.modifierManager.refresh();
		}
	}

	void NRGame::update(FT mFT)
	{
		camera.update(mFT);
		manager.update(mFT);
		world.update(mFT);
		timelineManager.update(mFT);
		updateDebugText(mFT);
		inputShoot = 0;
	}
	void NRGame::updateDebugText(FT mFT)
	{
		ostringstream s;
		const auto& entities(manager.getEntities());
		const auto& bodies(world.getBodies());
		const auto& sensors(world.getSensors());
		int componentCount{0}, dynamicBodiesCount{0};
		for(const auto& e : entities) componentCount += e->getComponents().size();
		for(const auto& b : bodies) if(!b->isStatic()) ++dynamicBodiesCount;

		s << "FPS: "				<< gameWindow.getFPS() << "\n";
		s << "FrameTime: "			<< mFT << "\n";
		s << "Bodies(all): "		<< bodies.size() << "\n";
		s << "Bodies(static): "		<< bodies.size() - dynamicBodiesCount << "\n";
		s << "Bodies(dynamic): "	<< dynamicBodiesCount << "\n";
		s << "Sensors: "			<< sensors.size() << "\n";
		s << "Entities: "			<< entities.size() << "\n";
		s << "Components: "			<< componentCount << endl;

		debugText.setString(s.str());
	}

	void NRGame::draw()
	{
		camera.apply();
		//if(getGameWindow().isKeyPressed(KKey::F))
		manager.draw();
		//drawDebugGrid();
		camera.unapply();
		render(debugText);
	}

	void NRGame::render(const Drawable& mDrawable) { gameWindow.draw(mDrawable); }

	void NRGame::setDebugGrid(int mX, int mY) { debugGrid[mX + grid.getOffset()][mY + grid.getOffset()] = 1; }
	void NRGame::clearDebugGrid() { for(int iX{0}; iX < grid.getColumns(); ++iX) for(int iY{0}; iY < grid.getRows(); ++iY) debugGrid[iX][iY] = 0; }
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

				Vec2i a{grid.getCellSize() * oIX, grid.getCellSize() * oIY};
				Vec2i b{grid.getCellSize() * (oIX + 1), grid.getCellSize() * oIY};
				Vec2i c{grid.getCellSize() * (oIX + 1), grid.getCellSize() * (oIY + 1)};
				Vec2i d{grid.getCellSize() * oIX, grid.getCellSize() * (oIY + 1)};
				debugGridVertices.emplace_back(toPixels(a), color);
				debugGridVertices.emplace_back(toPixels(b), color);
				debugGridVertices.emplace_back(toPixels(c), color);
				debugGridVertices.emplace_back(toPixels(d), color);
			}
		render(debugGridVertices);
	}
	void NRGame::initDebugGrid()
	{
		for(int iX{0}; iX < grid.getColumns(); ++iX)
		{
			debugGrid.emplace_back(vector<int>(grid.getRows()));
			for(int iY{0}; iY < grid.getRows(); ++iY) debugGrid[iX][iY] = 0;
		}
	}
}
