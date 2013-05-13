#include "Components/NRCPlayer.h"
#include "Components/NRCHumanoid.h"
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
	Entity* seekEntity(NRGame& mGame, Grid& mGrid, Body& mSeeker, Vector2i mTarget, Vector2i& mLastPos)
	{
		Entity* result{nullptr};
		const auto& startPosition(mSeeker.getPosition());
		Vector2f direction(mTarget - startPosition);
		if(direction.x == 0 && direction.y == 0) return result;

		auto gridQuery(mGrid.getQuery<GridQueryTypes::RayCast>(startPosition, direction));

		Body* body;
		while((body = gridQuery.next()) != nullptr)
		{
			if(body == &mSeeker) continue;

			Entity* entity{static_cast<Entity*>(body->getUserData())};
			if(entity == nullptr) continue;

			result = entity;
			break;
		}

		mLastPos = Vector2i(gridQuery.getLastPos());
		return result;
	}
	Entity* seekEntity(NRGame& mGame, Grid& mGrid, Body& mSeeker, Vector2i mTarget, const string& mTargetGroup, const vector<string>& mIgnoreGroups, Vector2i& mLastPos)
	{
		Entity* result{nullptr};
		const auto& startPosition(mSeeker.getPosition());
		Vector2f direction(mTarget - startPosition);
		if(direction.x == 0 && direction.y == 0) return result;

		auto gridQuery(mGrid.getQuery<GridQueryTypes::RayCast>(startPosition, direction));

		Body* body;
		while((body = gridQuery.next()) != nullptr)
		{
			if(body == &mSeeker) continue;

			if(containsAny(body->getGroups(), mIgnoreGroups)) continue;
			if(!contains(body->getGroups(), mTargetGroup)) break;
			Entity* entity{static_cast<Entity*>(body->getUserData())};
			if(entity == nullptr) continue;

			result = entity;
			break;
		}

		mLastPos = Vector2i(gridQuery.getLastPos());
		//for(const auto& ii : gridQuery.getVisitedIndexes()) mGame.setDebugGrid(ii.x, ii.y);
		return result;
	}

	NRCPlayer::NRCPlayer(Entity& mEntity, NRGame& mGame, NRCHumanoid& mCHumanoid) : Component(mEntity, "player"), game(mGame), cHumanoid(mCHumanoid) { }

	void NRCPlayer::update(float)
	{
		if(game.getInputY() == 0) cHumanoid.unCrouch();
		else if(game.getInputY() == 1) cHumanoid.crouch();

		if(game.getInputX() == 0) cHumanoid.move(0, false);
		else if(game.getInputX() == -1) cHumanoid.move(-1, game.getInputWalk());
		else if(game.getInputX() == 1) cHumanoid.move(1, game.getInputWalk());

		if(game.getInputJump() == 1) cHumanoid.jump();
		if(game.getInputShoot() == 0) return;

		auto& body = getEntity().getFirstComponent<NRCPhysics>("physics").getBody();
		Grid& grid(body.getWorld().getSpatial<Grid>());
		Vector2i out;
		Entity* enemy{seekEntity(game, grid, body, game.getMousePosition(), "solid", {"sensor"}, out)};

		game.getFactory().createTrail(body.getPosition(), out, Color::Green);

		if(enemy != nullptr)
		{
			//NRCPhysics& cPhysics(enemy->getFirstComponent<NRCPhysics>("physics"));
			//log("pew pew!");
			//enemy->destroy();
			//cPhysics.getBody().setStatic(false);
			//cPhysics.setAffectedByGravity(true);
		}
	}



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

	NRCTrail::NRCTrail(Entity& mEntity, NRGame& mGame, Vector2i mA, Vector2i mB, Color mColor) : Component(mEntity, "trail"), game(mGame), a{mA}, b{mB},
		color{mColor}, vertices{PrimitiveType::Lines, 2} { }

	void NRCTrail::update(float mFrameTime)
	{
		life -= mFrameTime;
		if(life <= 0) getEntity().destroy();
		color.a = life * (255 / 100);
		vertices[0].color = color;
		vertices[1].color = color;
		vertices[0].position = toPixels(a + Vector2i{getRnd(-20, 20), getRnd(-20, 20)});
		vertices[1].position = toPixels(b + Vector2i{getRnd(-20, 20), getRnd(-20, 20)});
	}
	void NRCTrail::draw() { game.render(vertices); }
}
