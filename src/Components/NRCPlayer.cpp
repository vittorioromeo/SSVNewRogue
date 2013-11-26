// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Components/NRCPlayer.hpp"
#include "Components/NRCHumanoid.hpp"
#include "Components/NRCPhysics.hpp"
#include "Core/NRGame.hpp"
#include "Utils/NRUtils.hpp"
#include "Core/NRGroups.hpp"

using namespace ssvs;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvu;

namespace nr
{
	NRCPlayer::NRCPlayer(NRGame& mGame, NRCHumanoid& mCHumanoid) : game(mGame), cHumanoid(mCHumanoid) { }

	void NRCPlayer::update(FT)
	{
		if(game.getInputY() == 0) cHumanoid.unCrouch();
		else if(game.getInputY() == 1) cHumanoid.crouch();

		if(game.getInputX() == 0) cHumanoid.move(0, false);
		else if(game.getInputX() == -1) cHumanoid.move(-1, game.getInputWalk());
		else if(game.getInputX() == 1) cHumanoid.move(1, game.getInputWalk());

		if(game.getInputJump() == 1) cHumanoid.jump();
		if(game.getInputShoot() == 0) return;

		auto& body = getEntity().getComponent<NRCPhysics>().getBody();

		Vec2i out;
		Entity* enemy{seekEntity(game, body, game.getMousePosition(), NRGroup::Solid, {}, out)};

		//game.getFactory().createTrail(body.getPosition(), out, Color::Green);

		if(enemy != nullptr)
		{
			//NRCPhysics& cPhysics(enemy->getComponent<NRCPhysics>());
			//log("pew pew!");
			enemy->destroy();
			//cPhysics.getBody().setStatic(false);
			//cPhysics.setAffectedByGravity(true);
		}
	}
}
