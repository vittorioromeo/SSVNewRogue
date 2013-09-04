// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_UTILS_UTILS
#define SSVNR_UTILS_UTILS

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;

	template<typename T> inline constexpr float toPixels(T mValue)					{ return mValue / 100.f; }
	template<typename T> inline constexpr int toCoords(T mValue)					{ return mValue * 100; }
	template<typename T> inline ssvs::Vec2f toPixels(const ssvs::Vec2<T>& mValue)	{ return {toPixels(mValue.x), toPixels(mValue.y)}; }
	template<typename T> inline ssvs::Vec2i toCoords(const ssvs::Vec2<T>& mValue)	{ return {toCoords(mValue.x), toCoords(mValue.y)}; }

	// TODO: refactor or remove these
	sses::Entity* seekEntity(NRGame& mGame, ssvsc::Body& mSeeker, const ssvs::Vec2i& mTarget, ssvs::Vec2i& mLastPos);
	sses::Entity* seekEntity(NRGame& mGame, ssvsc::Body& mSeeker, const ssvs::Vec2i& mTarget, ssvsc::Group mTargetGroup, const std::vector<ssvsc::Group>& mIgnoreGroups, ssvs::Vec2i& mLastPos);
}

#endif
