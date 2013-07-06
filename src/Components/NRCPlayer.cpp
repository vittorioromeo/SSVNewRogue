// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Components/NRCPlayer.h"
#include "Components/NRCHumanoid.h"
#include "Components/NRCPhysics.h"
#include "Core/NRGame.h"
#include "Utils/NRUtils.h"

using namespace ssvs;
using namespace ssvs::Utils;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvsc;
using namespace ssvu;

namespace nr
{
	NRCPlayer::NRCPlayer(NRGame& mGame, NRCHumanoid& mCHumanoid) : game(mGame), cHumanoid(mCHumanoid) { }

	void NRCPlayer::update(float)
	{
		if(game.getInputY() == 0) cHumanoid.unCrouch();
		else if(game.getInputY() == 1) cHumanoid.crouch();

		if(game.getInputX() == 0) cHumanoid.move(0, false);
		else if(game.getInputX() == -1) cHumanoid.move(-1, game.getInputWalk());
		else if(game.getInputX() == 1) cHumanoid.move(1, game.getInputWalk());

		if(game.getInputJump() == 1) cHumanoid.jump();
		if(game.getInputShoot() == 0) return;

		auto& body = getEntity().getFirstComponent<NRCPhysics>().getBody();
		Grid& grid(body.getWorld().getSpatial<Grid>());

		Vec2i out;
		Entity* enemy{seekEntity(game, grid, body, game.getMousePosition(), "solid", {}, out)};

		//game.getFactory().createTrail(body.getPosition(), out, Color::Green);

		if(enemy != nullptr)
		{
			//NRCPhysics& cPhysics(enemy->getFirstComponent<NRCPhysics>("physics"));
			//log("pew pew!");
			enemy->destroy();
			//cPhysics.getBody().setStatic(false);
			//cPhysics.setAffectedByGravity(true);
		}
	}
}
