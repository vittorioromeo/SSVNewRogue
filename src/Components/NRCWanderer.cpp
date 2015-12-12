// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Components/NRCWanderer.hpp"
#include "Components/NRCHumanoid.hpp"
#include "Components/NRCPhysics.hpp"
#include "Core/NRGame.hpp"
#include "Core/NRGroups.hpp"
#include "Utils/NRUtils.hpp"

using namespace ssvs;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvu;

namespace nr
{
    NRCWanderer::NRCWanderer(
        sses::Entity& mE, NRGame&, NRCHumanoid& mCHumanoid)
        : sses::Component{mE}, /*game(mGame),*/ cHumanoid(mCHumanoid)
    {
    }

    void NRCWanderer::update(FT mFT)
    {
        time -= mFT * 2.2f;
        if(time <= 0)
        {
            time = 100 + getRndI(0, 100);
            dir = getRndI(-1, 2);
            tired = getRndI(0, 15) > 10;
        }

        cHumanoid.move(dir, false);
        if(!tired)
            cHumanoid.unCrouch();
        else
            cHumanoid.crouch();

        if(ssvu::toInt(time) % 90 == 0) cHumanoid.jump();
        return;
        /*
                auto& body = getEntity().getComponent<NRCPhysics>().getBody();
                Vec2i out;
                Entity* enemy{seekEntity(game, body, body.getPosition() +
           Vec2i(body.getVelocity()), NRGroup::Humanoid, {NRGroup::GSensor},
           out)};

                game.getFactory().createTrail(body.getPosition(), out,
           Color::Red);

                if(enemy != nullptr)
                {
                    //enemy->destroy();
                }
                */
    }
}
