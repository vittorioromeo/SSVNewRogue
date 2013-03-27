#include "Utils/NRAnimation.h"

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvuj;
using namespace ssvs;
using namespace ssvsc;
using namespace ssvsc::Utils;

namespace nr
{	
	void NRAnimation::nextStep()
	{
		int lastIndex{currentIndex};
		reverse ? --currentIndex : ++currentIndex;
		
		if(currentIndex >= static_cast<int>(steps.size()))
		{
			if(loop)
			{
				if(pingPong) { reverse = !reverse; currentIndex = lastIndex; }
				else currentIndex = 0;
			}
			else currentIndex = lastIndex;
		}
		else if(currentIndex < 0)
		{
			if(loop)
			{
				if(pingPong) { reverse = !reverse; currentIndex = lastIndex; }
				else currentIndex = steps.size() - 1;
			}
			else currentIndex = lastIndex;
		}
		
		currentTime = 0;
	}
	
	void NRAnimation::update(float mFrameTime)
	{
		if(steps.empty()) return;
		
		currentTime += mFrameTime * speed;
		if(currentTime >= getCurrentStep().time) nextStep();
	}
	
	void NRAnimation::addStep(NRAnimationStep mStep)								{ steps.push_back(mStep); }
	void NRAnimation::addSteps(std::vector<NRAnimationStep> mSteps)					{ for(auto& step : mSteps) steps.push_back(step); }
	void NRAnimation::addSteps(std::vector<string> mStepLabels, float mStepTime)	{ for(auto& label : mStepLabels) steps.push_back({label, mStepTime}); }
	
	// Getters
	NRAnimationStep& NRAnimation::getCurrentStep()	{ return steps[currentIndex]; }
	string NRAnimation::getCurrentLabel()			{ return getCurrentStep().label; }
	
	// Setters
	void NRAnimation::setSpeed(float mSpeed)		{ speed = mSpeed; }
	void NRAnimation::setLoop(bool mLoop)			{ loop = mLoop; }
	void NRAnimation::setPingPong(bool mPingPong)	{ pingPong = mPingPong; }
	void NRAnimation::setReverse(bool mReverse)		{ reverse = mReverse; }
}
