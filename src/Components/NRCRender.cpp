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
		for(auto& sprite : sprites)
		{
			auto& rect(sprite.getTextureRect());
			sprite.setOrigin({rect.width / 2.f, rect.height / 2.f});

			sprite.setPosition(position + offset);
			sprite.setScale(flippedX ? -1 : 1, flippedY ? -1 : 1);
			if(scaleWithBody) sprite.setScale(size.x / 16.f, size.y / 16.f);
		}
	}
	void NRCRender::draw() { for(auto& sprite : sprites) game.render(sprite); }

	void NRCRender::addSprite(Sprite mSprite) { sprites.push_back(mSprite); }

	// Getters
	bool NRCRender::isFlippedX() const			{ return flippedX; }
	bool NRCRender::isFlippedY() const			{ return flippedY; }
	vector<Sprite>& NRCRender::getSprites()		{ return sprites; }

	// Setters
	void NRCRender::setRotation(float mDegrees)	{ for(auto& sprite : sprites) sprite.setRotation(mDegrees); }
	void NRCRender::setFlippedX(bool mFlippedX)	{ flippedX = mFlippedX; }
	void NRCRender::setFlippedY(bool mFlippedY)	{ flippedY = mFlippedY; }
	void NRCRender::setScaleWithBody(bool mScaleWithBody) { scaleWithBody = mScaleWithBody; }
	void NRCRender::setOffset(Vector2f mOffset)	{ offset = mOffset; }
}
