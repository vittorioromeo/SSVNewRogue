// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_UTILS_UTILS
#define SSVNR_UTILS_UTILS

#include "Core/NRDependencies.h"

namespace nr
{
	template<typename T> float toPixels(T mValue)						{ return mValue / 100; }
	template<typename T> int toCoords(T mValue)							{ return mValue * 100; }
	template<typename T> sf::Vector2f toPixels(sf::Vector2<T> mValue)	{ return {toPixels(mValue.x), toPixels(mValue.y)}; }
	template<typename T> sf::Vector2i toCoords(sf::Vector2<T> mValue)	{ return {toCoords(mValue.x), toCoords(mValue.y)}; }
}

#endif
