// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Core/NRFactory.hpp"
#include "Core/NRAssets.hpp"
#include "Core/NRGame.hpp"
#include "Components/NRCPhysics.hpp"
#include "Components/NRCRender.hpp"
#include "Components/NRCPlayer.hpp"
#include "Components/NRCHumanoidAnimationController.hpp"
#include "Components/NRCHumanoid.hpp"
#include "Components/NRCTrail.hpp"
#include "Components/NRCWanderer.hpp"
#include "Core/NRGroups.hpp"

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

	Entity& NRFactory::createTrail(const Vec2i& mA, const Vec2i& mB, Color mColor)
	{
		auto& result(manager.createEntity());
		//result.addGroups({"trail"});
		result.createComponent<NRCTrail>(game, mA, mB, mColor);
		return result;
	}
	Entity& NRFactory::createWall(const Vec2i& mPosition)
	{
		auto& result(manager.createEntity());
		//result.addGroups({"wall"});
		auto& cPhysics(result.createComponent<NRCPhysics>(world, true, mPosition, Vec2i{1600, 1600}));
		auto& cRender(result.createComponent<NRCRender>(game, cPhysics.getBody()));

		Body& body(cPhysics.getBody());
		body.addGroups(NRGroup::Solid);
		body.addGroupsToCheck(NRGroup::Solid);

		cRender.pushSprite(Sprite{assets.get<Texture>("wall.png")});
		cRender.setScaleWithBody(true);

		return result;
	}
	Entity& NRFactory::createHumanoid(const Vec2i& mPosition)
	{
		auto& result(manager.createEntity());
		//result.addGroups({"humanoid"});
		auto& cPhysics(result.createComponent<NRCPhysics>(world, false, mPosition, Vec2i{700, 1300}));
		auto& cHumanoid(result.createComponent<NRCHumanoid>(cPhysics));
		auto& cRender(result.createComponent<NRCRender>(game, cPhysics.getBody()));
		result.createComponent<NRCHumanoidAnimationController>(cRender, cHumanoid);

		Body& body(cPhysics.getBody());
		body.addGroups(NRGroup::Solid, NRGroup::Humanoid);
		body.addGroupsToCheck(NRGroup::Solid);

		return result;
	}
	Entity& NRFactory::createPlayer(const Vec2i& mPosition)
	{
		auto& result(createHumanoid(mPosition));
		result.createComponent<NRCPlayer>(game, result.getComponent<NRCHumanoid>());

		auto& cRender = result.getComponent<NRCRender>();
		cRender.pushSprite(Sprite{assets.get<Texture>("legs.png")});
		cRender.pushSprite(Sprite{assets.get<Texture>("body.png")});
		cRender.pushSprite(Sprite{assets.get<Texture>("arms.png")});
		cRender.pushSprite(Sprite{assets.get<Texture>("head.png")});
		for(auto& s : cRender.getSprites()) s.setTextureRect({0, 0, 16, 16});

		return result;
	}
	Entity& NRFactory::createWanderer(const Vec2i& mPosition)
	{
		auto& result(createHumanoid(mPosition));
		result.createComponent<NRCWanderer>(game, result.getComponent<NRCHumanoid>());

		auto& cRender = result.getComponent<NRCRender>();
		cRender.pushSprite(Sprite{assets.get<Texture>("legs.png")});
		cRender.pushSprite(Sprite{assets.get<Texture>("body2.png")});
		cRender.pushSprite(Sprite{assets.get<Texture>("arms.png")});
		cRender.pushSprite(Sprite{assets.get<Texture>("head.png")});

		for(auto& s : cRender.getSprites()) s.setTextureRect({0, 0, 16, 16});

		return result;
	}
}
