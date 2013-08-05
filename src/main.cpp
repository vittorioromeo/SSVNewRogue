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
using namespace ssvs;
using namespace nr;
using namespace ssvrpg;

int main()
{
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
	GameWindow gameWindow{"SSVNewRogue", createStaticTimer(gameWindow, 0.5f, 0.5f), width, height, 1, false};
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
using namespace ssvs::Utils;
using namespace ssvsc;
using namespace ssvsc::Utils;
using namespace ssvu;

AssetManager assets;
void initAssets() { AssetFolder("Data/").loadToManager(assets); }

struct CTest : Component
{
	vector<Vertex*>& vertexPtrs;
	World& world; Body& body;
	vector<Vertex> myVertices{4};

	CTest(Vec2i mPosition, vector<Vertex*>& mVertices, World& mWorld) : vertexPtrs(mVertices), world(mWorld), body(world.create(mPosition, {1500, 1500}, false)) { }
	~CTest() { body.destroy(); for(const auto& v : myVertices) eraseRemove(vertexPtrs, &v); }

	void setColor(const Color& mColor) { for(auto& v : myVertices) v.color = mColor; }
	void move(const Vec2f& mOffset)
	{
		if(mOffset.x != 0) body.setVelocityX(mOffset.x);
		if(mOffset.y != 0) body.setVelocityY(mOffset.y);
	}
	void init() override
	{
		body.setUserData(this);

		body.addGroup(0);
		body.addGroupToCheck(0);
		//body.addGroupNoResolve(0);

		for(int i{0}; i < 4; ++i) vertexPtrs.push_back(&myVertices[i]);

		body.onDetection += [&](const DetectionInfo&){ };
		body.onOutOfBounds += [&]{ getEntity().destroy(); };
	}
	void update(float) override
	{
		if(getRnd(0, 190) > 180) body.setVelocity(Vec2f(getRnd(-550, 550), getRnd(-550, 550)));
		//body.applyForce({0.f, 100.f});

		const AABB& s(body.getShape());

		const float left{toPixels(s.getLeft())};
		const float right{toPixels(s.getRight())};
		const float top{toPixels(s.getTop())};
		const float bottom{toPixels(s.getBottom())};

		myVertices[0].position = {left, top};
		myVertices[1].position = {right, top};
		myVertices[2].position = {right, bottom};
		myVertices[3].position = {left, bottom};
	}
};

struct TestGame
{
	GameWindow window{"", createStaticTimer(window, 0.5f, 0.5f), 1280, 720, 1};
	GameState game;
	Camera camera{window, {{0, 0}, {1280, 720}}};

	World world{createResolver<Retro>(), createSpatial<Grid>(1000, 1000, 1500, 400)};
	Manager manager; vector<Vertex*> vertices; TimelineManager tm;


	Entity& create(Vec2i mPosition, bool mStatic = false)
	{
		auto& e = manager.createEntity();
		auto& c = e.createComponent<CTest>(mPosition, vertices, world);

		if(mStatic) { c.body.setStatic(true); c.setColor(Color::White); }
		return e;
	}

	Entity& createPlayer(Vec2i mPosition)
	{
		auto& e = manager.createEntity();
		auto& c = e.createComponent<CTest>(mPosition, vertices, world);
		c.setColor(Color::Green);
		return e;
	}

	TestGame()
	{
		if(true)
		{
			startBenchmark();
			for(int iY{0}; iY < 100; ++iY) for(int iX{0}; iX < 100; ++iX) create({iX * 1500, iY * 1500}, false);
			lo << lt("creation b") << endBenchmark() << endl;
		}

		if(false)
		{
			{
				auto& e = manager.createEntity(); auto& c = e.createComponent<CTest>(Vec2i{10000, 170000}, vertices, world);
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
				auto& e = manager.createEntity(); auto& c = e.createComponent<CTest>(Vec2i{15000, 170000}, vertices, world);
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
		}

		{
			auto& e = manager.createEntity();
			auto& f = e.createComponent<CTest>(Vec2i{200000, 170000}, vertices, world);
			f.body.setStatic(true);
			f.setColor(Color::Magenta);
			f.body.setHalfSize({400000, 2500});
		}

		auto& player = createPlayer({-5000, 0});
		auto& c = player.getComponent<CTest>();
		auto move = [&](Vec2f mOffset){ c.move(mOffset); };
		float spd = 610.f;

		using k = Keyboard::Key;
		game.addInput({{k::Left}}, 	[=](float){ move({-spd, 0}); });
		game.addInput({{k::Right}}, [=](float){ move({spd, 0}); });
		game.addInput({{k::Up}}, 	[=](float){ move({0, -spd}); });
		game.addInput({{k::Down}}, 	[=](float){ move({0, spd}); });
		game.addInput({{k::Z}}, 	[=](float mFrameTime){ camera.zoom(pow(1.1f, mFrameTime)); });
		game.addInput({{k::X}}, 	[=](float mFrameTime){ camera.zoom(pow(0.9f, mFrameTime)); });

		game.onUpdate += [&](float mFrameTime)
		{
			window.setTitle(toStr(window.getFPS()));
			//camera.centerOn(Vec2f(c.body.getPosition()) / 100.f);

			//for(const auto& e : manager.getComponents<CTest>()) { e->body.applyForce({0, 20});  }

			tm.update(mFrameTime);
			world.update(mFrameTime);
			manager.update(mFrameTime);

			return;
			if(manager.getEntities().size() <= 0) return;

			startBenchmark();
			for(const auto& e : manager.getEntities()) e->destroy();
			lo << lt("desrtoy b") << endBenchmark() << endl;

			//for(Body* b : player.getComponent<CTest>("test").body.test) { static_cast<CTest*>(b->getUserData())->setColor(Color::Blue); }
			//c->body.setVelocity({0, 0});
		};

		game.onDraw += [&]
		{
			camera.apply();
			manager.draw();
			VertexArray v(PrimitiveType::Quads, vertices.size()); for(unsigned int i{0}; i < vertices.size(); i++) v[i] = *(vertices[i]); window.draw(v);
			camera.unapply();
		};

		camera.zoom(2.7f);
		window.setVsync(false);
		window.setGameState(game);
		window.run();
	}
};

int main() { initAssets(); TestGame{}; return 0; }

#endif
