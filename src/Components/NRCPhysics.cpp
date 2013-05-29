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
		body.setUserData(&getEntity());

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
	}
	NRCPhysics::~NRCPhysics() { body.destroy(); } // BUG: this has to be called before world is destroyed, or else SEGFAULT - find a way to avoid that!

	void NRCPhysics::update(float)
	{
		if(affectedByGravity && body.getVelocity().y < maxVelocityY) body.applyForce(gravityForce);
	}

	// Setters
	void NRCPhysics::setAffectedByGravity(bool mAffectedByGravity) { affectedByGravity = mAffectedByGravity; }

	// Getters
	World& NRCPhysics::getWorld() const				{ return world; }
	Body& NRCPhysics::getBody() const				{ return body; }
	Vector2i NRCPhysics::getLastResolution() const	{ return lastResolution; }
	bool NRCPhysics::isAffectedByGravity() const	{ return affectedByGravity; }
	bool NRCPhysics::isCrushedLeft() const			{ return crushedLeft > crushedTolerance; }
	bool NRCPhysics::isCrushedRight() const			{ return crushedRight > crushedTolerance; }
	bool NRCPhysics::isCrushedTop() const			{ return crushedTop > crushedTolerance; }
	bool NRCPhysics::isCrushedBottom() const		{ return crushedBottom > crushedTolerance; }
	int NRCPhysics::getCrushedLeft() const			{ return crushedLeft; }
	int NRCPhysics::getCrushedRight() const			{ return crushedRight; }
	int NRCPhysics::getCrushedTop() const			{ return crushedTop; }
	int NRCPhysics::getCrushedBottom() const		{ return crushedBottom; }
}
