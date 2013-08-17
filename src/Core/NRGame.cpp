// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <sstream>
#include <string>
#include "Core/NRDependencies.h"
#include "Core/NRAssets.h"
#include "Core/NRGame.h"


#include "Components/NRCHumanoid.h"
#include "Components/NRCTrail.h"

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
		world(createResolver<Retro>(), createSpatial<Grid>(1000, 1000, 1000, 500)), grid(world.getSpatial<Grid>()), debugText{assets.get<BitmapFont>("limeStroked")}
	{
		debugText.setTracking(-3);

		gameState.onUpdate += [&](float mFrameTime){ update(mFrameTime); };
		gameState.onDraw += [&]{ draw(); };

		initInput(); initLevel(); initDebugGrid();
	}

	void NRGame::initInput()
	{
		using k = Keyboard::Key;
		using b = Mouse::Button;
		using t = Input::Trigger::Type;

		gameState.addInput({{k::Escape}}, [&](float){ gameWindow.stop(); });

		gameState.addInput({{k::A}}, [=](float mFrameTime){ camera.move(Vec2f{-10, 0} * mFrameTime); });
		gameState.addInput({{k::D}}, [=](float mFrameTime){ camera.move(Vec2f{10, 0} * mFrameTime); });
		gameState.addInput({{k::W}}, [=](float mFrameTime){ camera.move(Vec2f{0, -10} * mFrameTime); });
		gameState.addInput({{k::S}}, [=](float mFrameTime){ camera.move(Vec2f{0, 10} * mFrameTime); });
		gameState.addInput({{k::Q}}, [=](float mFrameTime){ camera.zoom(pow(1.1f, mFrameTime)); });
		gameState.addInput({{k::E}}, [=](float mFrameTime){ camera.zoom(pow(0.9f, mFrameTime)); });

		add2StateInput(gameState, {{k::X}}, inputJump);
		add2StateInput(gameState, {{k::LShift}}, inputWalk);

		add3StateInput(gameState, {{k::Left}}, {{k::Right}}, inputX);
		add3StateInput(gameState, {{k::Up}}, {{k::Down}}, inputY);

		gameState.addInput({{k::Z}, {b::Left}},	[&](float){ inputShoot = 1; }, t::Once);

		gameState.addInput({{k::Num1}}, [&](float){ factory.createWall(getMousePosition()); }, t::Once);
		gameState.addInput({{k::Num2}}, [&](float){ factory.createWanderer(getMousePosition()); });
		gameState.addInput({{k::Num3}}, [&](float){ factory.createPlayer(getMousePosition()); }, t::Once);
		gameState.addInput({{k::Num4}}, [&](float)
		{
			auto index(grid.getIndex(getMousePosition()));
			auto count(grid.getCell(index.x, index.y).getBodies().size());
			lo << index.x << " " << index.y << "  :: " << count << endl;
			debugGrid[index.x + grid.getOffset()][index.y + grid.getOffset()] = 1;
		}, t::Once);
		gameState.addInput({{k::Num5}}, [&](float){ clearDebugGrid(); }, t::Once);
		gameState.addInput({{k::Num6}}, [&](float)
		{
			auto body(grid.getQuery<GridQueryTypes::Point>(getMousePosition()).next());
			if(body == nullptr) return;
			auto entity(static_cast<Entity*>(body->getUserData()));
			if(entity != nullptr) entity->destroy();
		});
	}
	void NRGame::initLevel()
	{
		int tilesX{320 / 16}, tilesY{240 / 16};
		// tilesX = 50; // world.getSpatial<Grid>().getIndexXMax() / 4;
		// tilesY = 50; // world.getSpatial<Grid>().getIndexYMax() / 4;

		for(int iY{0}; iY < tilesY; ++iY)
			for(int iX{0}; iX < tilesX; ++iX)
				if(iX == 0 || iY == 0 || iX == tilesX - 1 || iY == tilesY - 1) factory.createWall({1600 * iX + 800, 1600 * iY + 800});
				else if(false)
				{
					if(getRnd(0, 100) > 90) factory.createWanderer({1600 * iX + 800, 1600 * iY + 800});
					else if(getRnd(0, 100) > 90) factory.createWall({1600 * iX + 800, 1600 * iY + 800});
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

	void NRGame::update(float mFrameTime)
	{
		manager.update(mFrameTime);
		world.update(mFrameTime);
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

		s << "FPS: "				<< gameWindow.getFPS() << endl;
		s << "FrameTime: "			<< mFrameTime << endl;
		s << "Bodies(all): "		<< bodies.size() << endl;
		s << "Bodies(static): "		<< bodies.size() - dynamicBodiesCount << endl;
		s << "Bodies(dynamic): "	<< dynamicBodiesCount << endl;
		s << "Sensors: "			<< world.getSensors().size() << endl;
		s << "Entities: "			<< entities.size() << endl;
		s << "Components: "			<< componentCount << endl;

		debugText.setString(s.str());
	}

	void NRGame::draw()
	{
		camera.apply();
		//if(getGameWindow().isKeyPressed(Keyboard::Key::F))
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
				debugGridVertices.append({toPixels(a), color});
				debugGridVertices.append({toPixels(b), color});
				debugGridVertices.append({toPixels(c), color});
				debugGridVertices.append({toPixels(d), color});
			}
		render(debugGridVertices);
	}
	void NRGame::initDebugGrid()
	{
		for(int iX{0}; iX < grid.getColumns(); ++iX)
		{
			debugGrid.push_back(vector<int>(grid.getRows()));
			for(int iY{0}; iY < grid.getRows(); ++iY) debugGrid[iX][iY] = 0;
		}
	}
}
