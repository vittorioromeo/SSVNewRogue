// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVNR_COMPONENTS_PHYSICS
#define SSVNR_COMPONENTS_PHYSICS

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;

	class NRCPhysics : public sses::Component
	{
		private:
			static constexpr int crushedMax{3}, crushedTolerance{1};

			ssvsc::World& world;
			ssvsc::Body& body;
			ssvs::Vec2i lastResolution;
			bool affectedByGravity{true};
			int crushedLeft{0}, crushedRight{0}, crushedTop{0}, crushedBottom{0};
			int maxVelocityY{1000};
			ssvs::Vec2f gravityForce{0, 25};

		public:
			ssvu::Delegate<void(sses::Entity&)> onDetection;
			ssvu::Delegate<void(ssvs::Vec2i)> onResolution;

			NRCPhysics(ssvsc::World& mWorld, bool mIsStatic, const ssvs::Vec2i& mPosition, const ssvs::Vec2i& mSize, bool mAffectedByGravity = true);
			~NRCPhysics();

			void init() override;
			void update(float mFrameTime) override;

			inline void setAffectedByGravity(bool mAffectedByGravity) { affectedByGravity = mAffectedByGravity; }

			inline ssvsc::World& getWorld() const					{ return world; }
			inline ssvsc::Body& getBody() const						{ return body; }
			inline const ssvs::Vec2i& getLastResolution() const		{ return lastResolution; }
			inline bool isAffectedByGravity() const					{ return affectedByGravity; }
			inline bool isCrushedLeft() const						{ return crushedLeft > crushedTolerance; }
			inline bool isCrushedRight() const						{ return crushedRight > crushedTolerance; }
			inline bool isCrushedTop() const						{ return crushedTop > crushedTolerance; }
			inline bool isCrushedBottom() const						{ return crushedBottom > crushedTolerance; }
			inline int getCrushedLeft() const						{ return crushedLeft; }
			inline int getCrushedRight() const						{ return crushedRight; }
			inline int getCrushedTop() const						{ return crushedTop; }
			inline int getCrushedBottom() const						{ return crushedBottom; }
	};
}

#endif
