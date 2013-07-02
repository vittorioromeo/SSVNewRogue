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
	Entity* seekEntity(NRGame& mGame, Grid& mGrid, Body& mSeeker, Vector2i mTarget, Vector2i& mLastPos)
	{
		Entity* result{nullptr};
		const auto& startPosition(mSeeker.getPosition());
		Vector2f direction(mTarget - startPosition);
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

		mLastPos = Vector2i(gridQuery.getLastPos());
		return result;
	}
	Entity* seekEntity(NRGame& mGame, Grid& mGrid, Body& mSeeker, Vector2i mTarget, const string& mTargetGroup, const vector<string>& mIgnoreGroups, Vector2i& mLastPos)
	{
		Entity* result{nullptr};
		const auto& startPosition(mSeeker.getPosition());
		Vector2f direction(mTarget - startPosition);
		if(direction.x == 0 && direction.y == 0) return result;

		auto gridQuery(mGrid.getQuery<GridQueryTypes::Distance>(startPosition, 5000));

		vector<pair<Vector2i, Vector2i>> testvec;

		Body* body;
		while((body = gridQuery.next()) != nullptr)
		{
			if(body == &mSeeker) continue;

			if(containsAny(body->getGroups(), mIgnoreGroups)) continue;
			if(!contains(body->getGroups(), mTargetGroup)) break;
			Entity* entity{static_cast<Entity*>(body->getUserData())};
			if(entity == nullptr) continue;

			result = entity;


			testvec.push_back({mSeeker.getPosition(), Vector2i(gridQuery.getLastPos())});

			// break;
		}

		for(auto& t:testvec) mGame.getFactory().createTrail(t.first, t.second, Color::Green);

		mLastPos = Vector2i(gridQuery.getLastPos());
		//for(const auto& ii : gridQuery.getVisitedIndexes()) mGame.setDebugGrid(ii.x, ii.y);
		return result;
	}
}
