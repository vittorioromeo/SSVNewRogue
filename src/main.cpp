// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#define SSVNEWROGUE_BENCHMARK
#ifndef SSVNEWROGUE_BENCHMARK

#include "Core/NRDependencies.h"
#include "Core/NRAssets.h"
#include "Core/NRGame.h"
#include "Components/NRCPlayer.h"

#include <unordered_set>
#include <string>
#include <memory>
#include <cassert>

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvu::PreAlloc;
using namespace ssvs;
using namespace nr;
using namespace sses;
using namespace ssvrpg;

struct BaseObj : MemoryManageable
{
	BaseObj() { }
	virtual ~BaseObj() { }
};

struct SmallObj : BaseObj
{
	char data[100];
	~SmallObj()
	{
		//lo << "smallobj dtor" << endl;
	}
};

struct BigObj : BaseObj
{
	char data[500];
	~BigObj()
	{
		//lo << "bigobj dtor" << endl;
	}
};

int main()
{
	if(true)
	{
		PreAllocDyn p{65000};					// << this preallocator is VERY speed-dependent on the allocated space
		PreAllocChunk pc{sizeof(BigObj), 200};	// << this preallocator can hold different objects of different types, as long as (their size <= chunk size)
		PreAllocStatic<BigObj> ps{100};			// << this preallocator can hold only a specific type
		PreAllocStatic<SmallObj> pss{100};		// << this preallocator can hold only a specific type

		Manager manager;
		startBenchmark();
		{
			for(int k{0}; k < 100000; ++k)
			{
				for(int i{0}; i < 100; ++i) manager.createEntity();
				manager.update(1);
				for(auto& e : manager.getEntities()) e->destroy();
				manager.update(1);
			}
		}
		lo("manager") << endBenchmark() << endl;

		startBenchmark();
		{
			vector<BaseObj*> bases;

			for(int k{0}; k < 10000; ++k)
			{
				for(int i{0}; i < 100; ++i) bases.push_back(new SmallObj);
				for(int i{0}; i < 100; ++i) bases.push_back(new BigObj);
				for(auto& b : bases) delete b;
				bases.clear();
			}
		}
		lo("new/del") << endBenchmark() << endl;

		startBenchmark();
		{
			vector<SmallObj*> sb;
			vector<BigObj*> bb;

			for(int k{0}; k < 10000; ++k)
			{
				for(int i{0}; i < 100; ++i) sb.push_back(p.create<SmallObj>());
				for(int i{0}; i < 100; ++i) bb.push_back(p.create<BigObj>());
				for(auto& b : sb) p.destroy<SmallObj>(b);
				for(auto& b : bb) p.destroy<BigObj>(b);
				sb.clear();
				bb.clear();
			}
		}
		lo("prealloc dynamic") << endBenchmark() << endl;

		startBenchmark();
		{
			vector<SmallObj*> sb;
			vector<BigObj*> bb;

			for(int k{0}; k < 10000; ++k)
			{
				for(int i{0}; i < 100; ++i) sb.push_back(pc.create<SmallObj>());
				for(int i{0}; i < 100; ++i) bb.push_back(pc.create<BigObj>());
				for(auto& b : sb) pc.destroy<SmallObj>(b);
				for(auto& b : bb) pc.destroy<BigObj>(b);
				sb.clear();
				bb.clear();
			}
		}
		lo("prealloc chunk") << endBenchmark() << endl;

		startBenchmark();
		{
			vector<SmallObj*> sb;

			for(int k{0}; k < 10000; ++k)
			{
				for(int i{0}; i < 100; ++i) sb.push_back(new SmallObj);
				for(auto& b : sb) delete b;
				sb.clear();
			}
		}
		lo("new/del static smallobj") << endBenchmark() << endl;

		startBenchmark();
		{
			vector<SmallObj*> sb;

			for(int k{0}; k < 10000; ++k)
			{
				for(int i{0}; i < 100; ++i) sb.push_back(pss.create());
				for(auto& b : sb) pss.destroy(b);
				sb.clear();
			}
		}
		lo("prealloc_static static smallobj") << endBenchmark() << endl;

		startBenchmark();
		{
			vector<BigObj*> bb;

			for(int k{0}; k < 10000; ++k)
			{
				for(int i{0}; i < 100; ++i) bb.push_back(new BigObj);
				for(auto& b : bb) delete b;
				bb.clear();
			}
		}
		lo("new/del static bigobj") << endBenchmark() << endl;

		startBenchmark();
		{
			vector<BigObj*> bb;

			for(int k{0}; k < 10000; ++k)
			{
				for(int i{0}; i < 100; ++i) bb.push_back(ps.create());
				for(auto& b : bb) ps.destroy(b);
				bb.clear();
			}
		}
		lo("prealloc_static static bigobj") << endBenchmark() << endl;

		startBenchmark();
		{
			PAMMDyn<BigObj> mm(p);

			for(int k{0}; k < 10000; ++k)
			{
				for(int i{0}; i < 100; ++i) mm.create<BigObj>();
				mm.refresh();

				for(auto& b : mm) mm.del(*b);
				mm.refresh();


			}
		}
		lo("prealloc_dyn mm") << endBenchmark() << endl;
	}

	//return 0;

	if(false)
	{
		Value<int> strength{10};
		Value<int> endurance{10};
		Value<int> maxHealth{10};

		Modifier<int> enduranceHealthModifier;
		enduranceHealthModifier.onCompute += [&](Value<int>&, int& mCurrent){ mCurrent += 5 * strength.getComputed(); };
		enduranceHealthModifier.onCompute += [&](Value<int>&, int& mCurrent){ mCurrent += 10 * endurance.getComputed(); };

		maxHealth.addDependency(strength);
		maxHealth.addDependency(endurance);
		maxHealth.addModifier(enduranceHealthModifier);

		lo << maxHealth.getComputed() << endl;
		endurance.setBase(15);
		lo << maxHealth.getComputed() << endl;
	}

	unsigned int width{VideoMode::getDesktopMode().width}, height{VideoMode::getDesktopMode().height};
	width = 800; height = 600;

	NRAssets assets;

	GameWindow gameWindow;
	gameWindow.setTitle("SSVNewRogue");
	gameWindow.setTimer<StaticTimer>(0.5f, 0.5f);
	gameWindow.setSize(width, height);
	gameWindow.setFullscreen(false);
	//gameWindow.setFPSLimited(true);
	//gameWindow.setMaxFPS(200);

	NRGame game{gameWindow, assets};

	gameWindow.setGameState(game.getGameState());
	gameWindow.run();

	return 0;
}

#else

#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <bitset>
#include <fstream>
#include "Core/NRDependencies.h"
#include "Utils/NRUtils.h"

using namespace nr;
using namespace sf;
using namespace std;
using namespace sses;
using namespace ssvs;
using namespace ssvu::FileSystem;
using namespace ssvsc::Utils;
using namespace ssvu;

AssetManager assets;
void initAssets() { AssetFolder("Data/").loadToManager(assets); }

struct CTest : Component
{
	World& world; Body& body; GameWindow& window;
	ssvs::VertexVector<sf::PrimitiveType::Quads> myVertices{4};

	CTest(const Vec2i& mPosition, World& mWorld, GameWindow& mWindow) : world(mWorld), body(world.create(mPosition, {1500, 1500}, false)), window(mWindow) { }
	~CTest() { body.destroy(); }

	void setColor(const Color& mColor) { for(int i{0}; i < 4; ++i) myVertices[i].color = mColor; }
	void move(const Vec2f& mOffset)
	{
		if(mOffset.x != 0) body.setVelocityX(mOffset.x);
		if(mOffset.y != 0) body.setVelocityY(mOffset.y);
	}
	void init() override
	{
		body.setUserData(this);

		body.setRestitutionX(0.8f);
		body.setRestitutionY(0.8f);

		body.addGroup(0);
		body.addGroupToCheck(0);
		//body.setVelTransferMultX(0.0001f);
		body.addGroupNoResolve(0);

		body.onDetection += [&](const DetectionInfo&){ /*getEntity().destroy();*/ };
		body.onOutOfBounds += [&]{ getEntity().destroy(); };
	}
	void update(float) override
	{
		if(getRnd(0, 190) > 180) body.setVelocity(Vec2f(getRnd(-550, 550), getRnd(-550, 550)));
		//body.applyForce({0.f, 20.f});

		const auto& s(body.getShape());
		const float left{toPixels(s.getLeft())};
		const float right{toPixels(s.getRight())};
		const float top{toPixels(s.getTop())};
		const float bottom{toPixels(s.getBottom())};

		myVertices[0].position = {left, top};
		myVertices[1].position = {right, top};
		myVertices[2].position = {right, bottom};
		myVertices[3].position = {left, bottom};

		//body.applyForce({0, 20});
	}
	inline void draw() override { window.draw(myVertices); }
};

struct TestGame
{
	GameWindow window;
	GameState game;
	Camera camera{window, {ssvs::zeroVec2f, {1280, 720}}};

	World world{1000, 1000, 1500, 400};
	Manager manager; TimelineManager tm;

	Entity& create(const Vec2i& mPosition, bool mStatic = false)
	{
		auto& e = manager.createEntity();
		auto& c = e.createComponent<CTest>(mPosition, world, window);

		if(mStatic) { c.body.setStatic(true); c.setColor(Color::White); }
		return e;
	}

	Entity& createPlayer(const Vec2i& mPosition)
	{
		auto& e = manager.createEntity();
		auto& c = e.createComponent<CTest>(mPosition, world, window);
		c.setColor(Color::Green);
		e.getComponent<CTest>().body.setMass(1000.f);
		return e;
	}

	TestGame()
	{
		window.setTitle("SSVNewRogue benchmark");
		window.setTimer<StaticTimer>(0.5f, 0.5f);
		window.setSize(1280, 720);
		window.setFullscreen(false);

		if(true)
		{
			startBenchmark();
			for(int iY{0}; iY < 100; ++iY) for(int iX{0}; iX < 100; ++iX) create({iX * 1500, iY * 1500}, false);
			lo("creation b") << endBenchmark() << endl;
		}

		/*if(false)
		{
			{
				auto& e = manager.createEntity(); auto& c = e.createComponent<CTest>(Vec2i{10000, 170000}, world);
				c.body.setStatic(true); c.setColor(Color::Blue);
				c.body.setHalfSize({5000, 5000});

				auto& t = tm.create();
				t.append<Do>([&]
				{
					Body& b = c.body;
					b.setPosition({b.getPosition().x, b.getPosition().y - 100});
				});
				t.append<Wait>(1);
				t.append<Go>(0, -1);
			}

			{
				auto& e = manager.createEntity(); auto& c = e.createComponent<CTest>(Vec2i{15000, 170000}, world);
				c.body.setStatic(true); c.setColor(Color::Yellow);
				c.body.setHalfSize({5000, 5000});

				auto& t = tm.create();
				t.append<Do>([&]
				{
					Body& b = c.body;
					b.setPosition({b.getPosition().x, b.getPosition().y - 100});
				});
				t.append<Wait>(1);
				t.append<Go>(0, -1);
			}
		}*/

		if(false){
			auto& e = manager.createEntity();
			auto& f = e.createComponent<CTest>(Vec2i{200000, 170000}, world, window);
			f.body.setStatic(true);
			f.setColor(Color::Magenta);
			f.body.setHalfSize({400000, 2500});
			f.body.setVelTransferMultX(2.f);
		}

		auto& player = createPlayer({-5000, 0});
		auto& c = player.getComponent<CTest>();
		auto move = [&](const Vec2f& mOffset){ c.move(mOffset); };
		float spd = 610.f;

		using k = Keyboard::Key;
		game.addInput({{k::Left}}, 	[=](float){ move({-spd, 0}); });
		game.addInput({{k::Right}}, [=](float){ move({spd, 0}); });
		game.addInput({{k::Up}}, 	[=](float){ move({0, -spd}); });
		game.addInput({{k::Down}}, 	[=](float){ move({0, spd}); });
		game.addInput({{k::Q}}, 	[=](float){ camera.zoomOut(1.1f); });
		game.addInput({{k::E}}, 	[=](float){ camera.zoomIn(1.1f); });

		game.onUpdate += [&](float mFT)
		{
			window.setTitle(toStr(window.getFPS()));
			camera.setCenter(Vec2f(c.body.getPosition()) / 100.f);

			tm.update(mFT);
			manager.update(mFT);
			world.update(mFT);
			camera.update(mFT);

			return;
			if(manager.getEntities().size() <= 0) return;

			startBenchmark();
			for(const auto& e : manager.getEntities()) e->destroy();
			lo("desrtoy b") << endBenchmark() << endl;
		};

		game.onDraw += [&]
		{
			camera.apply();
			manager.draw();
			camera.unapply();
		};

		camera.setNextZoomFactor(2.7f);
		window.setVsync(false);
		window.setGameState(game);
		window.run();
	}
};

int main() { initAssets(); TestGame{}; return 0; }

#endif
