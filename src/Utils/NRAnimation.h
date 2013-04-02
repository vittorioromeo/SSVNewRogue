#ifndef SSVNR_UTILS_ANIMATION
#define SSVNR_UTILS_ANIMATION

#include "Core/NRDependencies.h"

namespace nr
{
	struct NRAnimationStep { std::string label; float time; };

	class NRAnimation
	{
		private:
			std::vector<NRAnimationStep> steps;
			int currentIndex{0};
			float speed{1}, currentTime{0};
			bool loop{true}, pingPong{false}, reverse{false};

			void nextStep();

		public:
			NRAnimation() = default;
			void update(float mFrameTime);
			void addStep(NRAnimationStep mStep);
			void addSteps(std::vector<NRAnimationStep> mSteps);
			void addSteps(std::vector<std::string> mStepLabels, float mStepTime);

			// Getters
			NRAnimationStep& getCurrentStep();
			std::string getCurrentLabel();

			// Setters
			void setSpeed(float mSpeed);
			void setLoop(bool mLoop);
			void setPingPong(bool mPingPong);
			void setReverse(bool mReverse);
	};

	// TODO: move this class to SSVStart!
}

#endif
