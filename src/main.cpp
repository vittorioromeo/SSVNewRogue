// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

// #define SSVNEWROGUE_BENCH

#ifndef SSVNEWROGUE_BENCH

#include "Core/NRDependencies.hpp"
#include "Core/NRAssets.hpp"
#include "Core/NRGroups.hpp"
#include "Core/NRFactory.hpp"
#include "Core/NRGame.hpp"

using namespace nr;
using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvu::FileSystem;
using namespace ssvs;

int main()
{
    unsigned int width{VideoMode::getDesktopMode().width},
        height{VideoMode::getDesktopMode().height};
    width = 800;
    height = 600;

    NRAssets assets;

    GameWindow gameWindow;
    gameWindow.setTitle("SSVNewRogue test");
    gameWindow.setTimer<TimerStatic>(0.5f, 0.5f);
    gameWindow.setSize(width, height);
    gameWindow.setFullscreen(false);
    gameWindow.setFPSLimited(true);
    gameWindow.setMaxFPS(200);

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
#include "Core/NRDependencies.hpp"
#include "Utils/NRUtils.hpp"

using namespace nr;
using namespace sf;
using namespace std;
using namespace sses;
using namespace ssvs;
using namespace ssvu::FileSystem;
using namespace ssvsc::Utils;
using namespace ssvu;

AssetManager<> assets;
void initAssets() { AssetFolder("Data/").loadToManager(assets); }

struct CTest : Component
{
    World& world;
    Body& body;
    // GameWindow& window;
    // ssvs::VertexVector<sf::PrimitiveType::Quads> myVertices{4};

    CTest(
        Entity& mE, const Vec2i& mPosition, World& mWorld, GameWindow&)
        : Component{mE}, world(mWorld),
          body(world.create(
              mPosition, {1500, 1500}, false)) /*, window(mWindow)*/
    {
    }

    ~CTest() { body.destroy(); }

    void setColor(Color)
    {
        // for(int i{0}; i < 4; ++i) myVertices[i].color = mColor;
    }
    void move(const Vec2f& mOffset)
    {
        if(mOffset.x != 0) body.setVelocityX(mOffset.x);
        if(mOffset.y != 0) body.setVelocityY(mOffset.y);
    }
    void init()
    {
        body.setUserData(this);

        body.setRestitutionX(0.8f);
        body.setRestitutionY(0.8f);

        body.addGroups(0);
        body.addGroupsToCheck(0);
        // body.setVelTransferMultX(0.0001f);
        body.addGroupsNoResolve(0);

        body.onDetection += [&](const DetectionInfo&)
        { /*getEntity().destroy();*/ };
        body.onOutOfBounds += [&]
        {
            getEntity().destroy();
        };
    }
    void update(FT) override
    {
        if(getRndI(0, 190) > 180)
            body.setVelocity(Vec2f(getRndI(-550, 550), getRndI(-550, 550)));
        // body.applyAccel({0.f, 20.f});
        /*
                const auto& s(body.getShape());
                const float left{toPixels(s.getLeft())};
                const float right{toPixels(s.getRight())};
                const float top{toPixels(s.getTop())};
                const float bottom{toPixels(s.getBottom())};

                myVertices[0].position = {left, top};
                myVertices[1].position = {right, top};
                myVertices[2].position = {right, bottom};
                myVertices[3].position = {left, bottom};
        */
        // body.applyAccel({0, 20});
    }
    inline void draw() override
    {
        // window.draw(myVertices);
    }
};

struct TestGame
{
    GameWindow window;
    GameState game;
    Camera camera{window, {ssvs::zeroVec2f, {1280, 720}}};

    World world{1000, 1000, 1500, 400};
    Manager manager;
    TimelineManager tm;

    Entity& create(const Vec2i& mPosition, bool mStatic = false)
    {
        auto& e = manager.createEntity();
        auto& c = e.createComponent<CTest>(mPosition, world, window);

        if(mStatic)
        {
            c.body.setStatic(true);
            c.setColor(Color::White);
        }
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
        window.setTimer<TimerStatic>(0.5f, 0.5f);
        window.setSize(1280, 720);
        window.setFullscreen(false);

        if(true)
        {
            Benchmark::start("Creation");
            for(int iY{0}; iY < 100; ++iY)
                for(int iX{0}; iX < 100; ++iX)
                    create({iX * 1500, iY * 1500}, false);
            Benchmark::endLo();
        }

        /*if(false)
        {
            {
                auto& e = manager.createEntity(); auto& c =
        e.createComponent<CTest>(Vec2i{10000, 170000}, world);
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
                auto& e = manager.createEntity(); auto& c =
        e.createComponent<CTest>(Vec2i{15000, 170000}, world);
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

        /*if(false){
            auto& e = manager.createEntity();
            auto& f = e.createComponent<CTest>(Vec2i{200000, 170000}, world,
        window);
            f.body.setStatic(true);
            f.setColor(Color::Magenta);
            f.body.setHalfSize({400000, 25000});
            //f.body.setVelTransferMultX(2.f);
        }*/

        auto& player = createPlayer({-5000, 0});
        auto& c = player.getComponent<CTest>();
        auto move = [&](const Vec2f& mOffset)
        {
            c.move(mOffset);
        };
        float spd = 610.f;

        using k = KKey;
        game.addInput({{k::Left}}, [=](FT)
            {
                move({-spd, 0});
            });
        game.addInput({{k::Right}}, [=](FT)
            {
                move({spd, 0});
            });
        game.addInput({{k::Up}}, [=](FT)
            {
                move({0, -spd});
            });
        game.addInput({{k::Down}}, [=](FT)
            {
                move({0, spd});
            });
        game.addInput({{k::Q}}, [=](FT)
            {
                camera.zoomOut(1.1f);
            });
        game.addInput({{k::E}}, [=](FT)
            {
                camera.zoomIn(1.1f);
            });

        double acc = 0;
        int itr = 0;

        game.onUpdate += [&](FT mFT)
        {
            ++itr;
            acc += window.getMsUpdate();
            if(itr == 100)
            {
                ssvu::lo("ACC") << acc << std::endl;
                itr = 0;
                acc = 0;
            }

            window.setTitle("up: " + toStr(window.getMsUpdate()) + "\t dw: " +
                            toStr(window.getMsDraw()));
            // camera.setCenter(Vec2f(c.body.getPosition()) / 100.f);

            // tm.update(mFT);
            manager.update(mFT);
            world.update(mFT);
            // camera.update(mFT);

            return;
            /*	if(manager.getEntities().size() <= 0) return;

                Benchmark::start();
                for(const auto& e : manager.getEntities()) e->destroy();
                lo("desrtoy b") << Benchmark::getEndString() << "\n";*/
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

int main()
{
    initAssets();
    TestGame{};
    return 0;
}

#endif
