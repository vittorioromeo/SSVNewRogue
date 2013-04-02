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
	template<typename TDirection> Entity* seek(Grid& mGrid, Body& mSeeker, const string& , Vector2i& mOut, Vector2i mMP = Vector2i(0,0))
	{
		GridQuery gq{mGrid.getQuery(mSeeker.getPosition())};

		float dir = getRadiansToPoint(Vector2f(mSeeker.getPosition()), Vector2f(mMP));
		Vector2f dirv = getVectorFromRadians(dir, 1.f);

		Body* b;
		while((b = gq.next(dirv, "solid")) != nullptr)
		{
			mOut = Vector2i(gq.getOut());
			if(b == &mSeeker) continue;

			Entity* entity = static_cast<Entity*>(b->getUserData());
			if(entity == nullptr) continue;

			//if(!contains(b->getGroups(), mGroup)) return nullptr;

			return entity;
		}

		mOut = Vector2i(gq.getOut());
		return nullptr;

		// TODO: make this a serious method
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
		Entity* enemy;
		if(cHumanoid.isFacingLeft()) enemy = seek<QueryTraits::Orthogonal::Left>(grid, body, "humanoid", out, game.getMousePosition());
		else enemy = seek<QueryTraits::Orthogonal::Right>(grid, body, "humanoid", out, game.getMousePosition());

		game.getFactory().createTrail(body.getPosition(), out, Color::Green);

		if(enemy != nullptr)
		{
			NRCPhysics& body = enemy->getFirstComponent<NRCPhysics>("physics");
			//log("pew pew!");
			//enemy->destroy();
			body.getBody().setStatic(false);
			body.setAffectedByGravity(true);
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

		auto& body = getEntity().getFirstComponent<NRCPhysics>("physics").getBody();
		Grid& grid(body.getWorld().getSpatial<Grid>());

		Vector2i out{0, 0};
		Entity* enemy;
		if(cHumanoid.isFacingLeft()) enemy = seek<QueryTraits::Orthogonal::Left>(grid, body, "humanoid", out, game.getMousePosition());
		else enemy = seek<QueryTraits::Orthogonal::Right>(grid, body, "humanoid", out, game.getMousePosition());

		game.getFactory().createTrail(body.getPosition(), out, Color::Red);

		if(enemy != nullptr)
		{
			//NRCPhysics& body = enemy->getFirstComponent<NRCPhysics>("physics");
			//log("pew pew!");
			//enemy->destroy();
			//body.getBody().setStatic(true);
			//body.setAffectedByGravity(false);
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
