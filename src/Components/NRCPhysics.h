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

			NRGame& game;
			ssvsc::World& world;
			ssvsc::Body& body;
			ssvs::Vec2i lastResolution;
			bool affectedByGravity{true};
			int crushedLeft{0}, crushedRight{0}, crushedTop{0}, crushedBottom{0};
			int maxVelocityY{1000};
			ssvs::Vec2f gravityForce{0, 25};

		public:
			ssvu::Delegate<void, sses::Entity&> onDetection;
			ssvu::Delegate<void, ssvs::Vec2i> onResolution;

			NRCPhysics(sses::Entity& mEntity, NRGame& mGame, ssvsc::World& mWorld, bool mIsStatic, ssvs::Vec2i mPosition, ssvs::Vec2i mSize, bool mAffectedByGravity = true);
			~NRCPhysics();

			void update(float mFrameTime) override;

			// Setters
			void setAffectedByGravity(bool mAffectedByGravity);

			// Getters
			ssvsc::World& getWorld() const;
			ssvsc::Body& getBody() const;
			ssvs::Vec2i getLastResolution() const;
			bool isAffectedByGravity() const;
			bool isCrushedLeft() const;
			bool isCrushedRight() const;
			bool isCrushedTop() const;
			bool isCrushedBottom() const;
			int getCrushedLeft() const;
			int getCrushedRight() const;
			int getCrushedTop() const;
			int getCrushedBottom() const;
	};
}

#endif
