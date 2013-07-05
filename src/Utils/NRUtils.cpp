// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Utils/NRUtils.h"
#include "Core/NRGame.h"

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvuj;
using namespace ssvs;
using namespace ssvs::Utils;
using namespace ssvsc;
using namespace ssvsc::Utils;
using namespace sses;

namespace nr
{
	Entity* seekEntity(NRGame&, Grid& mGrid, Body& mSeeker, Vec2i mTarget, Vec2i& mLastPos)
	{
		Entity* result{nullptr};
		const auto& startPosition(mSeeker.getPosition());
		Vec2f direction(mTarget - startPosition);
		if(direction.x == 0 && direction.y == 0) return result;

		auto gridQuery(mGrid.getQuery<GridQueryTypes::RayCast>(startPosition, direction));

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
	Entity* seekEntity(NRGame& mGame, Grid& mGrid, Body& mSeeker, Vec2i mTarget, const string& mTargetGroup, const vector<string>& mIgnoreGroups, Vec2i& mLastPos)
	{
		Entity* result{nullptr};
		const auto& startPosition(mSeeker.getPosition());
		Vec2f direction(mTarget - startPosition);
		if(direction.x == 0 && direction.y == 0) return result;

		auto gridQuery(mGrid.getQuery<GridQueryTypes::Distance>(startPosition, 5000));

		vector<pair<Vec2i, Vec2i>> testvec;

		auto& world(mSeeker.getWorld());
		auto targetGroup(world.getGroup(mTargetGroup));
		Bitset ignoreGroups;
		for(const auto& l : mIgnoreGroups) ignoreGroups.set(world.getGroup(l));

		Body* body;
		while((body = gridQuery.next()) != nullptr)
		{
			if(body == &mSeeker) continue;

			if((body->hasAnyGroup(ignoreGroups))) continue;
			if(!body->hasGroup(targetGroup)) break;
			Entity* entity{static_cast<Entity*>(body->getUserData())};
			if(entity == nullptr) continue;

			result = entity;

			testvec.push_back({mSeeker.getPosition(), Vec2i(gridQuery.getLastPos())});

			// MUST BREAK FOR NORMAL RESULTS
			// break;
		}

		for(auto& t:testvec) mGame.getFactory().createTrail(t.first, t.second, Color::Green);

		mLastPos = Vec2i(gridQuery.getLastPos());
		//for(const auto& ii : gridQuery.getVisitedIndexes()) mGame.setDebugGrid(ii.x, ii.y);
		return result;
	}
}
