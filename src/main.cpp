#include "Core/NRDependencies.h"
#include "Core/NRAssets.h"
#include "Core/NRGame.h"

using namespace std;
using namespace ssvu;
using namespace ssvs;
using namespace nr;

int main()
{
	NRAssets assets;	
	GameWindow gameWindow{"SSVNewRogue", 320, 240, 3, false};
	NRGame game{gameWindow, assets};
	
	gameWindow.setGameState(game.getGameState());
	gameWindow.run();
	
	return 0;
}
