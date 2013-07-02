// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

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
	NRSensor::NRSensor(NRCPhysics& mParent, Vector2i mSize) : parent(mParent), position(parent.getBody().getPosition()), sensor(parent.getWorld().createSensor(position, mSize))
	{
		sensor.addGroupsToCheck({"solid"});

		sensor.onPreUpdate += [&]{ active = false; sensor.setPosition(position); };
		sensor.onDetection += [&](DetectionInfo mDetectionInfo)
		{
			if(mDetectionInfo.userData == nullptr) return;
			Entity* entity(static_cast<Entity*>(mDetectionInfo.userData));
			if(entity != &(parent.getEntity())) active = true;
		};
	}
	NRSensor::~NRSensor() { sensor.destroy(); }

	// Setters
	void NRSensor::setPosition(Vector2i mPosition) { position = mPosition; }

	// Getters
	Sensor& NRSensor::getSensor()	{ return sensor; }
	bool NRSensor::isActive() const	{ return active; }
}
