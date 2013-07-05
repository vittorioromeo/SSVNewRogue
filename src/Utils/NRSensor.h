// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_COMPONENTS_SENSOR
#define SSVNR_COMPONENTS_SENSOR

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;
	class NRCPhysics;

	class NRSensor
	{
		private:
			NRCPhysics& parent;
			ssvs::Vec2i position;
			ssvsc::Sensor& sensor;
			bool active{false};

		public:
			ssvu::Delegate<void, sses::Entity&> onDetection;

			NRSensor(NRCPhysics& mParent, ssvs::Vec2i mSize);
			~NRSensor();

			// Setters
			void setPosition(ssvs::Vec2i mOffset);

			// Getters
			ssvsc::Sensor& getSensor();
			bool isActive() const;
	};

	// TODO: maybe sensor can be a non-body class, that calculates index and checks the grid for collisions?
}

#endif
