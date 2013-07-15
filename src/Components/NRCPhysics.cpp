// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Core/NRGame.h"
#include "Components/NRCPhysics.h"

using namespace ssvs;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvsc;
using namespace ssvu;

namespace nr
{
	NRCPhysics::NRCPhysics(NRGame& mGame, World& mWorld, bool mIsStatic, Vec2i mPosition, Vec2i mSize, bool mAffectedByGravity)
		: game(mGame), world(mWorld), body(world.create(mPosition, mSize, mIsStatic)), affectedByGravity{mAffectedByGravity} { }
	NRCPhysics::~NRCPhysics() { body.destroy(); } // BUG: this has to be called before world is destroyed, or else SEGFAULT - find a way to avoid that!

	void NRCPhysics::init()
	{
		body.setUserData(&getEntity());

		body.onDetection += [&](const DetectionInfo& mDetectionInfo)
		{
			if(mDetectionInfo.userData == nullptr) return;
			Entity* entity(static_cast<Entity*>(mDetectionInfo.userData));
			onDetection(*entity);
		};
		body.onResolution += [&](const ResolutionInfo& mResolutionInfo)
		{
			onResolution(mResolutionInfo.resolution);

			lastResolution = mResolutionInfo.resolution;
			if(mResolutionInfo.resolution.x > 0) crushedLeft = crushedMax;
			else if(mResolutionInfo.resolution.x < 0) crushedRight = crushedMax;
			if(mResolutionInfo.resolution.y > 0) crushedTop = crushedMax;
			else if(mResolutionInfo.resolution.y < 0) crushedBottom = crushedMax;
		};
		body.onPreUpdate += [&]
		{
			lastResolution = {0, 0};
			if(crushedLeft > 0) --crushedLeft;
			if(crushedRight > 0) --crushedRight;
			if(crushedTop > 0) --crushedTop;
			if(crushedBottom > 0) --crushedBottom;
		};
	}

	void NRCPhysics::update(float)
	{
		if(affectedByGravity && body.getVelocity().y < maxVelocityY) body.applyForce(gravityForce);
	}
}
