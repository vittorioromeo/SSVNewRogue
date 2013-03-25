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
	NRCPhysics::NRCPhysics(Entity& mEntity, NRGame& mGame, World& mWorld, bool mIsStatic, Vector2i mPosition, Vector2i mSize, bool mAffectedByGravity)
		: Component(mEntity, "physics"), game(mGame), world(mWorld), body(world.create(mPosition, mSize, mIsStatic)), affectedByGravity{mAffectedByGravity}
	{
		body.onDetection += [&](DetectionInfo mDetectionInfo)
		{
			if(mDetectionInfo.userData == nullptr) return;
			Entity* entity(static_cast<Entity*>(mDetectionInfo.userData));
			onDetection(*entity);
		};
		body.onResolution += [&](ResolutionInfo mResolutionInfo)
		{
			onResolution(mResolutionInfo.resolution);

			lastResolution = mResolutionInfo.resolution;
			if(mResolutionInfo.resolution.x > 0) crushedLeft = true;
			else if(mResolutionInfo.resolution.x < 0) crushedRight = true;
			if(mResolutionInfo.resolution.y > 0) crushedTop = true;
			else if(mResolutionInfo.resolution.y < 0) crushedBottom = true;
		};
		body.onPreUpdate += [&]
		{
			lastResolution = {0, 0};
			crushedLeft = crushedRight = crushedTop = crushedBottom = false;
		};
		
		body.setUserData(&getEntity());
	}
	NRCPhysics::~NRCPhysics() { body.destroy(); }

	// Getters
	Body& NRCPhysics::getBody()					{ return body; }
	Vector2i NRCPhysics::getLastResolution()	{ return lastResolution; }
	bool NRCPhysics::isAffectedByGravity()		{ return affectedByGravity; }
	bool NRCPhysics::isCrushedLeft()			{ return crushedLeft; }
	bool NRCPhysics::isCrushedRight()			{ return crushedRight; }
	bool NRCPhysics::isCrushedTop()				{ return crushedTop; }
	bool NRCPhysics::isCrushedBottom()			{ return crushedBottom; }
}
