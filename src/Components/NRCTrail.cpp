// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Components/NRCTrail.h"
#include "Core/NRGame.h"
#include "Utils/NRUtils.h"

using namespace ssvs;
using namespace ssvs::Utils;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvsc;
using namespace ssvu;

namespace nr
{
	NRCTrail::NRCTrail(NRGame& mGame, Vec2i mA, Vec2i mB, Color mColor) : game(mGame), a{mA}, b{mB}, color{mColor}, vertices{PrimitiveType::Lines, 2} { }

	void NRCTrail::update(float mFrameTime)
	{
		life -= mFrameTime;
		if(life <= 0) getEntity().destroy();
		color.a = life * (255 / 100);
		vertices[0].color = vertices[1].color = color;
		vertices[0].position = toPixels(a + Vec2i{getRnd(-20, 20), getRnd(-20, 20)});
		vertices[1].position = toPixels(b + Vec2i{getRnd(-20, 20), getRnd(-20, 20)});
	}
	void NRCTrail::draw() { game.render(vertices); }
}
