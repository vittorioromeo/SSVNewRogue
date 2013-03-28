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
		
		body.setUserData(&getEntity());
	}
	NRCPhysics::~NRCPhysics() { body.destroy(); } // BUG: this has to be called before world is destroyed, or else SEGFAULT - find a way to avoid that!

	// Getters
	World& NRCPhysics::getWorld()				{ return world; }
	Body& NRCPhysics::getBody()					{ return body; }
	Vector2i NRCPhysics::getLastResolution()	{ return lastResolution; }
	bool NRCPhysics::isAffectedByGravity()		{ return affectedByGravity; }
	bool NRCPhysics::isCrushedLeft()			{ return crushedLeft > crushedTolerance; }
	bool NRCPhysics::isCrushedRight()			{ return crushedRight > crushedTolerance; }
	bool NRCPhysics::isCrushedTop()				{ return crushedTop > crushedTolerance; }
	bool NRCPhysics::isCrushedBottom()			{ return crushedBottom > crushedTolerance; }
	int NRCPhysics::getCrushedLeft()			{ return crushedLeft; }
	int NRCPhysics::getCrushedRight()			{ return crushedRight; }
	int NRCPhysics::getCrushedTop()				{ return crushedTop; }
	int NRCPhysics::getCrushedBottom()			{ return crushedBottom; }
}
