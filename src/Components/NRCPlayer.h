#ifndef SSVNR_COMPONENTS_PLAYER
#define SSVNR_COMPONENTS_PLAYER

#include "Core/NRDependencies.h"

namespace nr
{
	class NRGame;
	class NRCHumanoid;

	class NRCPlayer : public sses::Component
	{
		private:
			NRGame& game;
			NRCHumanoid& cHumanoid;

		public:
			NRCPlayer(sses::Entity& mEntity, NRGame& mGame, NRCHumanoid& mCHumanoid);

			void update(float) override;
	};

	class NRCWanderer : public sses::Component
	{
		private:
			NRGame& game;
			NRCHumanoid& cHumanoid;
			float time{200};
			int dir{0};
			bool tired{false};

		public:
			NRCWanderer(sses::Entity& mEntity, NRGame& mGame, NRCHumanoid& mCHumanoid);

			void update(float mFrameTime) override;
	};

	class NRCTrail : public sses::Component
	{
		private:
			NRGame& game;
			float life{75};
			sf::Vector2i a, b;
			sf::Color color;
			sf::VertexArray vertices;

		public:
			NRCTrail(sses::Entity& mEntity, NRGame& mGame, sf::Vector2i mA, sf::Vector2i mB, sf::Color mColor);

			void update(float mFrameTime) override;
			void draw() override;
	};

	template<typename T> class NRStat;

	template<typename T> struct NRStatModifier
	{
		ssvu::Delegate<void, NRStat<T>&> onAttach;
		ssvu::Delegate<void, NRStat<T>&> onDetach;
		ssvu::Delegate<void, T, T&, const T&> onChange;
		ssvu::Delegate<void, T&> onGet;
		ssvu::Delegate<void, T&> onGetMin;
		ssvu::Delegate<void, T&> onGetMax;

		NRStatModifier() = default;
	};

	template<typename T> class NRStat
	{
		private:
			std::vector<NRStatModifier<T>*> modifiers;
			T baseMin, baseMax, baseValue;

			void clampBaseToTotal()
			{
				if(baseValue < getMinTotal()) baseValue = getMinTotal();
				else if(baseValue > getMaxTotal()) baseValue = getMaxTotal();
			}

		public:
			NRStat(T mMin, T mMax, T mValue) : baseMin{mMin}, baseMax{mMax} { setBase(mValue); }
			NRStat(T mValue) : NRStat{std::numeric_limits<T>::min(), std::numeric_limits<T>::max(), mValue} { }

			void attach(NRStatModifier<T>* mModifier) { modifiers.push_back(mModifier); mModifier->onAttach(*this); }
			void detach(NRStatModifier<T>* mModifier) { mModifier->onDetach(*this); ssvu::eraseRemove(modifiers, mModifier); }

			void setBase(T mValue)
			{
				for(const auto& m : modifiers) m->onChange(mValue - baseValue, mValue, baseValue);
				baseValue = mValue;
				clampBaseToTotal();
			}
			void addToBase(T mValue) { setBase(baseValue + mValue); }

			T getValueTotal()		{ T result{baseValue}; for(const auto& m : modifiers) m->onGet(result); clampBaseToTotal(); return result; }
			T getMinTotal() const	{ T result{baseMin}; for(const auto& m : modifiers) m->onGetMin(result); return result; }
			T getMaxTotal() const	{ T result{baseMax}; for(const auto& m : modifiers) m->onGetMax(result); return result; }
	};
	// TODO: put classes in their own files
}

#endif
