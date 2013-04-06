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
		Vector2i tileSize{getValue<int>(mRoot, "tileWidth"), getValue<int>(mRoot, "tileHeight")};
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
		Json::Value frames(mRoot["frames"]);

		for(unsigned int i{0}; i < frames.size(); ++i)
			result.addStep({getArrayValue<string>(frames[i], 0), getArrayValue<float>(frames[i], 1)});

		result.setLoop(getValueOrDefault<bool>(mRoot, "loop", true));
		result.setPingPong(getValueOrDefault<bool>(mRoot, "pingPong", false));
		result.setReverse(getValueOrDefault<bool>(mRoot, "reverse", false));
		result.setSpeed(getValueOrDefault<float>(mRoot, "speed", 1.f));

		return result;
	}
}
