// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_CORE_DEPENDENCIES
#define SSVNR_CORE_DEPENDENCIES

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SSVUtils/SSVUtils.hpp>
#include <SSVUtilsJson/SSVUtilsJson.hpp>
#include <SSVStart/SSVStart.hpp>
#include <SSVEntitySystem/SSVEntitySystem.hpp>
#include <SSVSCollision/SSVSCollision.hpp>
#include <SSVRPGSystem/SSVRPGSystem.hpp>

namespace nr
{
	using World = ssvsc::World<ssvsc::Grid, ssvsc::Retro>;
	using Body = World::BodyType;
	using Sensor = World::SensorType;
	using DetectionInfo = World::DetectionInfoType;
	using ResolutionInfo = World::ResolutionInfoType;
	using FT = ssvu::FT;
}

#endif
