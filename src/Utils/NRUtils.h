// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_UTILS_UTILS
#define SSVNR_UTILS_UTILS

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;

	template<typename T> float toPixels(T mValue)						{ return mValue / 100; }
	template<typename T> int toCoords(T mValue)							{ return mValue * 100; }
	template<typename T> sf::Vector2f toPixels(sf::Vector2<T> mValue)	{ return {toPixels(mValue.x), toPixels(mValue.y)}; }
	template<typename T> sf::Vector2i toCoords(sf::Vector2<T> mValue)	{ return {toCoords(mValue.x), toCoords(mValue.y)}; }

	// TODO: refactor or remove these
	sses::Entity* seekEntity(NRGame& mGame, ssvsc::Grid& mGrid, ssvsc::Body& mSeeker, sf::Vector2i mTarget, sf::Vector2i& mLastPos);
	sses::Entity* seekEntity(NRGame& mGame, ssvsc::Grid& mGrid, ssvsc::Body& mSeeker, sf::Vector2i mTarget, const std::string& mTargetGroup, const std::vector<std::string>& mIgnoreGroups, sf::Vector2i& mLastPos);
}

#endif
