#include "Components/NRCPlayer.h"
#include "Components/NRCHumanoid.h"
#include "Core/NRGame.h"
#include "Utils/NRUtils.h"

using namespace ssvs;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvsc;
using namespace ssvu;

namespace nr
{
	NRCPlayer::NRCPlayer(Entity& mEntity,NRGame& mGame, NRCHumanoid& mCHumanoid) : Component(mEntity, "player"), game(mGame), cHumanoid(mCHumanoid) { }
	
	void NRCPlayer::update(float) 
	{		
		if(game.getInputY() == 0) cHumanoid.unCrouch();
		else if(game.getInputY() == 1) cHumanoid.crouch();
		
		if(game.getInputX() == 0) cHumanoid.move(0, false);
		else if(game.getInputX() == -1) cHumanoid.move(-1, game.getInputWalk());
		else if(game.getInputX() == 1) cHumanoid.move(1, game.getInputWalk());
		
		if(game.getInputJump() == 1) cHumanoid.jump();
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
	}
}
