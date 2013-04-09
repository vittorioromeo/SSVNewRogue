#include "Utils/NRUtils.h"

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvuj;
using namespace ssvs;
using namespace ssvs::Utils;
using namespace ssvsc;
using namespace ssvsc::Utils;

namespace nr
{
	Tileset getTilesetFromJSON(const Json::Value mRoot)
	{
		Json::Reader(); Json::Value(); // BUG: these useless lines are needed to avoid linker errors (WTF)
		Vector2i tileSize{as<int>(mRoot, "tileWidth"), as<int>(mRoot, "tileHeight")};
		Tileset result{tileSize};

		Json::Value labels{mRoot["labels"]};
		for(unsigned int iY{0}; iY < labels.size(); ++iY)
			for(unsigned int iX{0}; iX < mRoot["labels"][iY].size(); ++iX)
				result.setLabel(mRoot["labels"][iY][iX].asString(), iX, iY);

		return result;
	}

	NRAnimation getAnimationFromJSON(const Json::Value mRoot)
	{
		NRAnimation result;

		for(auto& frame : mRoot["frames"]) result.addStep({as<string>(frame, 0), as<float>(frame, 1)});

		result.setLoop(asOrDefault(mRoot, "loop", true));
		result.setPingPong(asOrDefault(mRoot, "pingPong", false));
		result.setReverse(asOrDefault(mRoot, "reverse", false));
		result.setSpeed(asOrDefault(mRoot, "speed", 1.f));

		return result;
	}

	Input::Combo getInputComboFromJSON(const Json::Value mArray)
	{
		Input::Combo result;

		for(auto& inputName : asVector<string>(mArray))
		{
			if(isKeyNameValid(inputName)) result.addKey(getKey(inputName));
			else if(isButtonNameValid(inputName)) result.addButton(getButton(inputName));
			else log("<" + inputName + "> is not a valid input name", "getInputComboFromJSON");
		}

		return result;
	}

}
