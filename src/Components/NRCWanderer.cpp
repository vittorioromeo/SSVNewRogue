// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Components/NRCWanderer.h"
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
	NRCWanderer::NRCWanderer(Entity& mEntity, NRGame& mGame, NRCHumanoid& mCHumanoid) : Component(mEntity, "wanderer"), game(mGame), cHumanoid(mCHumanoid) { }

	void NRCWanderer::update(float mFrameTime)
	{
		time -= mFrameTime * 2.2f;
		if(time <= 0)
		{
			time = 100 + getRnd(0, 100);
			dir = getRnd(-1, 2);
			tired = getRnd(0, 15) > 10;
		}

		cHumanoid.move(dir, false);
		if(!tired) cHumanoid.unCrouch(); else cHumanoid.crouch();

		if((int)time % 90 == 0) cHumanoid.jump();
		return;

		auto& body = getEntity().getFirstComponent<NRCPhysics>("physics").getBody();
		Grid& grid(body.getWorld().getSpatial<Grid>());
		Vector2i out;
		Entity* enemy{seekEntity(game, grid, body, body.getPosition() + Vector2i(body.getVelocity()), "humanoid", {"sensor"}, out)};

		game.getFactory().createTrail(body.getPosition(), out, Color::Red);

		if(enemy != nullptr)
		{
			//enemy->destroy();
		}
	}
}