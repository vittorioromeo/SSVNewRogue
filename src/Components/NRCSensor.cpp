#include "Components/NRCSensor.h"
#include "Components/NRCPhysics.h"
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
	NRCSensor::NRCSensor(Entity& mEntity, NRGame& mGame, World& mWorld, NRCPhysics& mParent, Vector2i mSize) : Component(mEntity, "sensor"), 
		game(mGame), world(mWorld), parent(mParent), position(parent.getBody().getPosition()), body(world.create(position, mSize, false))
	{
		body.addGroups({"sensor"});
		body.addGroupsToCheck({"solid"});
		body.addGroupsNoResolve({"solid"});
		
		body.onPreUpdate += [&]
		{ 
			active = false;
			body.setPosition(position); 
			
			VertexArray v(PrimitiveType::Quads, 4);
			v.append(toPixels(Vector2i{body.getShape().getLeft(), body.getShape().getTop()}));
			v.append(toPixels(Vector2i{body.getShape().getRight(), body.getShape().getTop()}));
			v.append(toPixels(Vector2i{body.getShape().getRight(), body.getShape().getBottom()}));
			v.append(toPixels(Vector2i{body.getShape().getLeft(), body.getShape().getBottom()}));
			game.render(v);
		};
		body.onDetection += [&](DetectionInfo mDetectionInfo)
		{
			if(mDetectionInfo.userData == nullptr) return;
			Entity* entity(static_cast<Entity*>(mDetectionInfo.userData));
			if(entity != &getEntity() && entity != &(parent.getEntity())) active = true;
		};
	}
	NRCSensor::~NRCSensor() { body.destroy(); }
	
	// Setters
	void NRCSensor::setPosition(Vector2i mPosition) { position = mPosition; }
	
	// Getters
	Body& NRCSensor::getBody() { return body; }
	bool NRCSensor::isActive() { return active; }	
}
