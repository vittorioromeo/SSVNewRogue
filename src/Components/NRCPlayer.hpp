// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_COMPONENTS_PLAYER
#define SSVNR_COMPONENTS_PLAYER

#include "Core/NRDependencies.hpp"

namespace nr
{
class NRGame;
class NRCHumanoid;

class NRCPlayer : public sses::Component
{
private:
    NRGame& game;
    NRCHumanoid& cHumanoid;

public:
    NRCPlayer(sses::Entity& mE, NRGame& mGame, NRCHumanoid& mCHumanoid);

    void update(FT) override;
};
}

#endif
