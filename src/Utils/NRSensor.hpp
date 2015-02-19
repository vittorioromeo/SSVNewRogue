// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_COMPONENTS_SENSOR
#define SSVNR_COMPONENTS_SENSOR

#include "Core/NRDependencies.hpp"

namespace nr
{
	class NRGame;
	class NRCPhysics;

	class NRSensor
	{
		private:
			NRCPhysics& parent;
			ssvs::Vec2i position;
			Sensor& sensor;
			bool active{false};

		public:
			ssvu::Delegate<void(sses::Entity&)> onDetection;

			NRSensor(NRCPhysics& mParent, const ssvs::Vec2i& mSize);
			~NRSensor();

			// Setters
			void setPosition(const ssvs::Vec2i& mPosition) { position = mPosition; }

			// Getters
			inline Sensor& getSensor() { return sensor; }
			inline bool isActive() const { return active; }
	};
}

#endif
