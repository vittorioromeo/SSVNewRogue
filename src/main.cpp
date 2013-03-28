#include "Core/NRDependencies.h"
#include "Core/NRAssets.h"
#include "Core/NRGame.h"

using namespace std;
using namespace ssvu;
using namespace ssvs;
using namespace nr;

int main()
{
	NRAssets assets;	
	GameWindow gameWindow{"SSVNewRogue", 320, 240, 3, false};
	NRGame game{gameWindow, assets};
	
	//gameWindow.setFPSLimit(60);
	gameWindow.setGameState(game.getGameState());
	gameWindow.run();
	
	return 0;
}
/*


#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SSVUtils/SSVUtils.h>
#include <SSVEntitySystem/SSVEntitySystem.h>
#include <SSVStart/SSVStart.h>
#include <SSVSCollision/SSVSCollision.h>

using namespace sf;
using namespace std;
using namespace sses;
using namespace ssvs;
using namespace ssvu::FileSystem;
using namespace ssvs::Utils;
using namespace ssvsc;
using namespace ssvsc::Utils;
using namespace ssvu;


AssetManager* assets;

void initAssets()
{
	ssvu::FileSystem::exists("");
	assets = new AssetManager;
	assets->loadFolder("Data/");
}

float toPixels(int mCoords) { return mCoords / 100.f; }
float toPixels(float mCoords) { return mCoords / 100.f; }

struct CTest : Component
{
	vector<Vertex*>& vertexPtrs;
	World& world;
	Body& body;
	vector<Vertex> myVertices{4};

	CTest(Entity& mEntity, Vector2i mPosition, vector<Vertex*>& mVertices, World& mWorld) : Component(mEntity, "test"), vertexPtrs(mVertices),
		world(mWorld), body(world.create(mPosition, {1500, 1500}, false))
	{
		body.addGroups({"test"});
		body.addGroupsToCheck({"test"});
		//body.addGroupsNoResolve({"test"});

		setColor(Color::Red);
		for(int i{0}; i < 4; i++) vertexPtrs.push_back(&myVertices[i]);

		// body.onDetection += [&](DetectionInfo) { getEntity().destroy(); };
		body.onOutOfBounds += [&]{ body.setPosition({0,0}); };
	}
	~CTest(){ body.destroy(); for(auto& v : myVertices) ssvu::eraseRemove(vertexPtrs, &v); }

	void setColor(Color mColor) { for(auto& v : myVertices) v.color = mColor; }
	void move(const Vector2f& mOffset)
	{		
		Vector2f v = body.getVelocity();
		if(mOffset.x != 0) v.x = mOffset.x;
		if(mOffset.y != 0) v.y = mOffset.y;

		body.setVelocity(v);
	}
	void update(float) override
	{
		float left{toPixels(body.getShape().getLeft())};
		float right{toPixels(body.getShape().getRight())};
		float top{toPixels(body.getShape().getTop())};
		float bottom{toPixels(body.getShape().getBottom())};

		myVertices[0].position = {left, top};
		myVertices[1].position = {right, top};
		myVertices[2].position = {right, bottom};
		myVertices[3].position = {left, bottom};
	}
};

struct TestGame
{
	GameWindow window{"", 1280, 720, 1};
	GameState game;
	Camera camera{window, {{0, 0}, {1280, 720}}};
	
	World world{createResolver<Retro>(), createSpatial<Grid>(1200, 1200, 2500, 300)};
	Manager manager;
	vector<Vertex*> vertices;
	TimelineManager tm;

	Entity& create(Vector2i mPosition, bool mStatic = false)
	{
		auto& e = manager.createEntity("test");
		auto& c = e.createComponent<CTest>(mPosition, vertices, world);

		if(mStatic) { c.body.setStatic(true); c.setColor(Color::White); }
		return e;
	}

	Entity& createPlayer(Vector2i mPosition)
	{
		auto& e = manager.createEntity("test");
		auto& c = e.createComponent<CTest>(mPosition, vertices, world);
		c.setColor(Color::Green);
		return e;
	}

	TestGame()
	{
		if(true)
		{
			startBenchmark();
			for(int iY{0}; iY <100; iY++) for(int iX{0}; iX < 100; iX++) create({iX * 1500, iY * 1500}, false);
			log(endBenchmark(), "creation b");
		}
		
		{
			{
				auto& e = manager.createEntity("test"); auto& c = e.createComponent<CTest>(Vector2i{10000, 170000}, vertices, world);
				c.body.setStatic(true); c.setColor(Color::Blue);
				c.body.getShape().setHalfSize({5000, 5000});

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
				auto& e = manager.createEntity("test"); auto& c = e.createComponent<CTest>(Vector2i{15000, 170000}, vertices, world);
				c.body.setStatic(true); c.setColor(Color::Yellow);
				c.body.getShape().setHalfSize({5000, 5000});

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
			auto& e = manager.createEntity("test"); auto& c = e.createComponent<CTest>(Vector2i{200000, 170000}, vertices, world);
			c.body.setStatic(true); c.setColor(Color::White);
			c.body.getShape().setHalfSize({400000, 2500});
		}

		{
			auto& e = manager.createEntity("test"); auto& c = e.createComponent<CTest>(Vector2i{-170000, 170000}, vertices, world);
			c.body.setStatic(true); c.setColor(Color::White);
			c.body.getShape().setHalfSize({2500, 400000});
		}

		auto& player = createPlayer({-5000, 0});
		CTest* c = player.getComponents<CTest>("test")[0];
		auto move = [=](Vector2f mOffset){ c->move(mOffset); };
		float spd = 610.f;

		// Inputs
		using k = Keyboard::Key;
		game.addInput({{k::Left}}, 	[=](float){ move({-spd, 0}); });
		game.addInput({{k::Right}}, [=](float){ move({spd, 0}); });
		game.addInput({{k::Up}}, 	[=](float){ move({0, -spd}); });
		game.addInput({{k::Down}}, 	[=](float){ move({0, spd}); });
		game.addInput({{k::Z}}, 	[=](float mFrameTime){ camera.zoom(pow(1.1f, mFrameTime)); });
		game.addInput({{k::X}}, 	[=](float mFrameTime){ camera.zoom(pow(0.9f, mFrameTime)); });

		// Window creation		
		window.setStaticFrameTime(false);
		window.setStaticFrameTimeValue(1);
		window.setVsync(false);

		// Update lambdas
		game.onUpdate += [&](float){ window.setTitle(toStr(window.getFPS())); };
		game.onUpdate += [&](float){ camera.centerOn(Vector2f(c->body.getPosition()) / 100.f); };

		game.onUpdate += [=](float) { for(auto& e : manager.getComponents<CTest>("test")) e->body.applyForce({0, 20}); };
		game.onUpdate += [&](float mFrameTime){ tm.update(mFrameTime); };
		game.onUpdate += [&](float mFrameTime){ world.update(mFrameTime); };
		game.onUpdate += [&](float mFrameTime){ manager.update(mFrameTime); };




		//game.onUpdate += [=](float) { for(auto& e : manager.getComponents<CPlayer>("test")) if(e != c && getRnd(0, 250) < 3) e->body.setVelocity({getRnd(-150, 2150), getRnd(-150, 2150)}); };
		//game.onUpdate += [&](float) { c->body->setVelocity({0, 0}); };

		game.onDraw += [&]{ camera.apply(); };
		game.onDraw += [&]{ manager.draw(); };
		game.onDraw += [&]{ VertexArray v(PrimitiveType::Quads, vertices.size()); for(unsigned int i{0}; i < vertices.size(); i++) v[i] = *(vertices[i]); window.draw(v); };
		game.onDraw += [&]{ camera.unapply(); };

		// Other stuff
		camera.zoom(2.7f);

		// Run the game
		window.setGameState(game);
		window.run();
	}
};

int main() { srand(time(0)); initAssets(); TestGame{}; return 0; }
*/
