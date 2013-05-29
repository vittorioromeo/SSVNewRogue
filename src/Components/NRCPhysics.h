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
			sf::Vector2i lastResolution;
			bool affectedByGravity{true};
			int crushedLeft{0}, crushedRight{0}, crushedTop{0}, crushedBottom{0};
			int maxVelocityY{1000};
			sf::Vector2f gravityForce{0, 25};

		public:
			ssvu::Delegate<void, sses::Entity&> onDetection;
			ssvu::Delegate<void, sf::Vector2i> onResolution;

			NRCPhysics(sses::Entity& mEntity, NRGame& mGame, ssvsc::World& mWorld, bool mIsStatic, sf::Vector2i mPosition, sf::Vector2i mSize, bool mAffectedByGravity = true);
			~NRCPhysics();

			void update(float mFrameTime) override;

			// Setters
			void setAffectedByGravity(bool mAffectedByGravity);

			// Getters
			ssvsc::World& getWorld() const;
			ssvsc::Body& getBody() const;
			sf::Vector2i getLastResolution() const;
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
