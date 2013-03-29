#include "Core/NRFactory.h"
#include "Core/NRAssets.h"
#include "Core/NRGame.h"

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvuj;
using namespace sses;
using namespace ssvs;
using namespace ssvsc;
using namespace ssvsc::Utils;

namespace nr
{
	NRFactory::NRFactory(NRAssets& mAssets, NRGame& mGame, Manager& mManager, World& mWorld) : assets(mAssets), game(mGame),
		manager(mManager), world(mWorld) { }
	
	Entity& NRFactory::createWall(Vector2i mPosition)
	{
		auto& result = manager.createEntity("wall");
		auto& cPhysics = result.createComponent<NRCPhysics>(game, world, true, mPosition, Vector2i{1600, 1600});
		auto& cRender = result.createComponent<NRCRender>(game, cPhysics.getBody());

		Body& body(cPhysics.getBody());
		body.addGroups({"solid"});
		
		cRender.addSprite(Sprite{assets.getTexture("wall.png")});

		return result;
	}
	Entity& NRFactory::createHumanoid(Vector2i mPosition)
	{
		auto& result = manager.createEntity("humanoid");
		auto& cPhysics = result.createComponent<NRCPhysics>(game, world, false, mPosition, Vector2i{700, 1300});
		auto& cHumanoid = result.createComponent<NRCHumanoid>(game, cPhysics);
		auto& cRender = result.createComponent<NRCRender>(game, cPhysics.getBody());
		result.createComponent<NRCHumanoidAnimationController>(cRender, cHumanoid);
	
		Body& body(cPhysics.getBody());
		body.addGroups({"solid"});
		body.addGroupsToCheck({"solid"});

		return result;
	}
	Entity& NRFactory::createPlayer(Vector2i mPosition)
	{
		auto& result = createHumanoid(mPosition);
		result.createComponent<NRCPlayer>(game, result.getFirstComponent<NRCHumanoid>("humanoid"));
		
		auto& cRender = result.getFirstComponent<NRCRender>("render");
		cRender.addSprite(Sprite{assets.getTexture("legs.png")});
		cRender.addSprite(Sprite{assets.getTexture("body.png")});
		cRender.addSprite(Sprite{assets.getTexture("arms.png")});
		cRender.addSprite(Sprite{assets.getTexture("head.png")});
		
		return result;
	}
	Entity& NRFactory::createWanderer(Vector2i mPosition)
	{
		auto& result = createHumanoid(mPosition);
		result.createComponent<NRCWanderer>(game, result.getFirstComponent<NRCHumanoid>("humanoid"));
		
		auto& cRender = result.getFirstComponent<NRCRender>("render");
		cRender.addSprite(Sprite{assets.getTexture("legs.png")});
		cRender.addSprite(Sprite{assets.getTexture("body2.png")});
		cRender.addSprite(Sprite{assets.getTexture("arms.png")});
		cRender.addSprite(Sprite{assets.getTexture("head.png")});
		
		return result;
	}
}
