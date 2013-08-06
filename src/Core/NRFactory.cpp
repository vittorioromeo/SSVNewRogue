// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Core/NRFactory.h"
#include "Core/NRAssets.h"
#include "Core/NRGame.h"
#include "Components/NRCPhysics.h"
#include "Components/NRCRender.h"
#include "Components/NRCPlayer.h"
#include "Components/NRCHumanoidAnimationController.h"
#include "Components/NRCHumanoid.h"
#include "Components/NRCTrail.h"
#include "Components/NRCWanderer.h"
#include "Core/NRGroups.h"

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

	Entity& NRFactory::createTrail(Vec2i mA, Vec2i mB, Color mColor)
	{
		auto& result(manager.createEntity());
		//result.addGroups({"trail"});
		result.createComponent<NRCTrail>(game, mA, mB, mColor);
		return result;
	}
	Entity& NRFactory::createWall(Vec2i mPosition)
	{
		auto& result(manager.createEntity());
		//result.addGroups({"wall"});
		auto& cPhysics(result.createComponent<NRCPhysics>(game, world, true, mPosition, Vec2i{1600, 1600}));
		auto& cRender(result.createComponent<NRCRender>(game, cPhysics.getBody()));

		Body& body(cPhysics.getBody());
		body.addGroup(NRGroup::Solid);
		body.addGroupToCheck(NRGroup::Solid);

		cRender.addSprite(Sprite{assets.get<Texture>("wall.png")});
		cRender.setScaleWithBody(true);

		return result;
	}
	Entity& NRFactory::createHumanoid(Vec2i mPosition)
	{
		auto& result(manager.createEntity());
		//result.addGroups({"humanoid"});
		auto& cPhysics(result.createComponent<NRCPhysics>(game, world, false, mPosition, Vec2i{700, 1300}));
		auto& cHumanoid(result.createComponent<NRCHumanoid>(game, cPhysics));
		auto& cRender(result.createComponent<NRCRender>(game, cPhysics.getBody()));
		result.createComponent<NRCHumanoidAnimationController>(cRender, cHumanoid);

		Body& body(cPhysics.getBody());
		body.addGroup(NRGroup::Solid);
		body.addGroup(NRGroup::Humanoid);
		body.addGroupToCheck(NRGroup::Solid);

		return result;
	}
	Entity& NRFactory::createPlayer(Vec2i mPosition)
	{
		auto& result(createHumanoid(mPosition));
		result.createComponent<NRCPlayer>(game, result.getComponent<NRCHumanoid>());

		auto& cRender = result.getComponent<NRCRender>();
		cRender.addSprite(Sprite{assets.get<Texture>("legs.png")});
		cRender.addSprite(Sprite{assets.get<Texture>("body.png")});
		cRender.addSprite(Sprite{assets.get<Texture>("arms.png")});
		cRender.addSprite(Sprite{assets.get<Texture>("head.png")});
		for(auto& s : cRender.getSprites()) s.setTextureRect({0, 0, 16, 16});

		return result;
	}
	Entity& NRFactory::createWanderer(Vec2i mPosition)
	{
		auto& result(createHumanoid(mPosition));
		result.createComponent<NRCWanderer>(game, result.getComponent<NRCHumanoid>());

		auto& cRender = result.getComponent<NRCRender>();
		cRender.addSprite(Sprite{assets.get<Texture>("legs.png")});
		cRender.addSprite(Sprite{assets.get<Texture>("body2.png")});
		cRender.addSprite(Sprite{assets.get<Texture>("arms.png")});
		cRender.addSprite(Sprite{assets.get<Texture>("head.png")});

		for(auto& s : cRender.getSprites()) s.setTextureRect({0, 0, 16, 16});

		return result;
	}
}
