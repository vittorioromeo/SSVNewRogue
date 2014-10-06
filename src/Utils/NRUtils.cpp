// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Utils/NRUtils.hpp"
#include "Core/NRGame.hpp"

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvs;
using namespace ssvsc;
using namespace ssvsc::Utils;
using namespace sses;

namespace nr
{
	Entity* seekEntity(NRGame& mGame, Body& mSeeker, const Vec2i& mTarget, Vec2i& mLastPos)
	{
		Entity* result{nullptr};
		const auto& startPosition(mSeeker.getPosition());
		Vec2f direction(mTarget - startPosition);
		auto mag(ssvs::getMag(direction));
		if(mag == 0) return result;

		auto gridQuery(mGame.getWorld().getQuery<QueryType::RayCast>(startPosition, ssvs::getNormalizedUnsafe(direction, mag)));

		Body* body;
		while((body = gridQuery.next()) != nullptr)
		{
			if(body == &mSeeker) continue;

			Entity* entity{static_cast<Entity*>(body->getUserData())};
			if(entity == nullptr) continue;

			result = entity;
			break;
		}

		mLastPos = Vec2i(gridQuery.getLastPos());
		return result;
	}
	Entity* seekEntity(NRGame& mGame, Body& mSeeker, const Vec2i& mTarget, Group mTargetGroup, const vector<Group>& mIgnoreGroups, Vec2i& mLastPos)
	{
		Entity* result{nullptr};
		const auto& startPosition(mSeeker.getPosition());
		Vec2f direction(mTarget - startPosition);
		if(direction.x == 0 && direction.y == 0) return result;

		auto gridQuery(mGame.getWorld().getQuery<QueryType::Distance>(startPosition, 5000));

		vector<pair<Vec2i, Vec2i>> testvec;

		GroupBitset ignoreGroups;
		for(const auto& l : mIgnoreGroups) ignoreGroups.set(l);

		Body* body;
		while((body = gridQuery.next()) != nullptr)
		{
			if(body == &mSeeker) continue;

			if((body->hasAnyGroup(ignoreGroups))) continue;
			if(!body->hasGroup(mTargetGroup)) break;
			Entity* entity{static_cast<Entity*>(body->getUserData())};
			if(entity == nullptr) continue;

			result = entity;

			testvec.emplace_back(mSeeker.getPosition(), Vec2i(gridQuery.getLastPos()));

			// MUST BREAK FOR NORMAL RESULTS
			// break;
		}

		for(auto& t:testvec) mGame.getFactory().createTrail(t.first, t.second, Color::Green);

		mLastPos = Vec2i(gridQuery.getLastPos());
		//for(const auto& ii : gridQuery.getVisitedIndexes()) mGame.setDebugGrid(ii.x, ii.y);
		return result;
	}
}
