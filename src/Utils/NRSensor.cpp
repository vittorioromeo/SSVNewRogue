// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Utils/NRSensor.hpp"
#include "Components/NRCPhysics.hpp"
#include "Core/NRGame.hpp"
#include "Utils/NRUtils.hpp"
#include "Core/NRGroups.hpp"

using namespace ssvs;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvu;

namespace nr
{
NRSensor::NRSensor(NRCPhysics& mParent, const Vec2i& mSize)
    : parent(mParent), position(parent.getBody().getPosition()),
      sensor(parent.getWorld().createSensor(position, mSize))
{
    sensor.addGroups(NRGroup::GSensor);
    sensor.addGroupsToCheck(NRGroup::Solid);

    sensor.onPreUpdate += [this]
    {
        active = false;
        sensor.setPosition(position);
    };
    sensor.onDetection += [this](const DetectionInfo& mDI)
    {
        if(mDI.userData == nullptr) return;
        Entity* entity(static_cast<Entity*>(mDI.userData));
        if(entity != &(parent.getEntity())) active = true;
    };
}
NRSensor::~NRSensor() { sensor.destroy(); }
}
