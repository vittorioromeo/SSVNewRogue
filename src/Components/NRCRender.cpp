// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Core/NRGame.h"
#include "Components/NRCRender.h"
#include "Utils/NRUtils.h"

using namespace ssvs;
using namespace sses;
using namespace std;
using namespace sf;
using namespace ssvsc;
using namespace ssvu;

namespace nr
{
	NRCRender::NRCRender(Entity& mEntity, NRGame& mGame, Body& mBody) : Component(mEntity, "render"), game(mGame), body(mBody) { }

	void NRCRender::update(float)
	{
		Vector2f position{toPixels(body.getPosition())}, size{toPixels(body.getSize())};
		for(auto& s : sprites)
		{
			const auto& rect(s.getTextureRect());
			s.setOrigin({rect.width / 2.f, rect.height / 2.f});
			s.setPosition(position + offset);
			s.setScale(flippedX ? -1 : 1, flippedY ? -1 : 1);
			if(scaleWithBody) s.setScale(size.x / 16.f, size.y / 16.f); // TODO: what is 16?
		}
	}
	void NRCRender::draw() { for(const auto& s : sprites) game.render(s); }

	void NRCRender::addSprite(const Sprite& mSprite) { sprites.push_back(mSprite); }

	// Getters
	bool NRCRender::isFlippedX() const						{ return flippedX; }
	bool NRCRender::isFlippedY() const						{ return flippedY; }
	vector<Sprite>& NRCRender::getSprites()					{ return sprites; }

	// Setters
	void NRCRender::setRotation(float mDegrees)				{ for(auto& s : sprites) s.setRotation(mDegrees); }
	void NRCRender::setFlippedX(bool mFlippedX)				{ flippedX = mFlippedX; }
	void NRCRender::setFlippedY(bool mFlippedY)				{ flippedY = mFlippedY; }
	void NRCRender::setScaleWithBody(bool mScaleWithBody)	{ scaleWithBody = mScaleWithBody; }
	void NRCRender::setOffset(Vector2f mOffset)				{ offset = mOffset; }
}
