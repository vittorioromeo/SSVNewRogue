#include "Utils/NRSensor.h"
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
	NRSensor::NRSensor(NRCPhysics& mParent, Vector2i mSize) : parent(mParent),
		position(parent.getBody().getPosition()), body(mParent.getWorld().create(position, mSize, false))
	{
		body.addGroups({"sensor"});
		body.addGroupsToCheck({"solid"});
		body.setResolve(false);

		body.onPreUpdate += [&]{ active = false; body.setPosition(position); };
		body.onDetection += [&](DetectionInfo mDetectionInfo)
		{
			if(mDetectionInfo.userData == nullptr) return;
			Entity* entity(static_cast<Entity*>(mDetectionInfo.userData));
			if(entity != &(parent.getEntity())) active = true;
		};
	}
	NRSensor::~NRSensor() { body.destroy(); }

	// Setters
	void NRSensor::setPosition(Vector2i mPosition) { position = mPosition; }

	// Getters
	Body& NRSensor::getBody() { return body; }
	bool NRSensor::isActive() { return active; }
}
