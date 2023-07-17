#include "PixelSpriteComponent.h"

#include <cassert>
#include "GameActor.h"
#include "GameLevel.h"

PixelSpriteComponent::PixelSpriteComponent(GameActor* actor, int width, int height)
	: Component(actor),
	  m_Width(width),
	  m_Height(height)
{
	actor->GetLevel()->AddEntity(this);
}

PixelSpriteComponent::~PixelSpriteComponent()
{
	GetOwner()->GetLevel()->RemoveEntity(this);
}

void PixelSpriteComponent::DrawPixels(uint32_t* buffer, int bufferWidth, int bufferHeight)
{
	using namespace Math;
	if (bufferWidth <= 0 || bufferHeight <= 0) {
		return;
	}

	ColorBGRA* drawBuffer = reinterpret_cast<ColorBGRA*>(buffer);
	const GameActor* actor = GetOwner();
	assert(actor);

	// sprite's middle point
	const Vec2DF midPos = actor->GetPosition();
	const Vec2DF scaledOffset = Vec2DF{ (float)m_Width, (float)m_Height } * actor->GetScale();
	const Vec2D upperLeftPos = (Vec2D)(midPos - scaledOffset * 0.5f);
	const Vec2D bottomRightPos = upperLeftPos + (Vec2D)scaledOffset;

	// clamp by screen size
	const Vec2D upperLeftPosClamped = Vec2D{ 
		Clamp(0, bufferWidth, upperLeftPos.x), 
		Clamp(0, bufferHeight, upperLeftPos.y) 
	};
	const Vec2D bottomRightPosClamped = Vec2D{
		Clamp(0, bufferWidth, bottomRightPos.x),
		Clamp(0, bufferHeight, bottomRightPos.y)
	};

	if (upperLeftPos.x > bottomRightPos.x || upperLeftPos.y > bottomRightPos.y) {
		return;
	}
	
	// TODO: rewrite as memcpy
	for (int y = upperLeftPosClamped.y; y < bottomRightPosClamped.y; ++y) {
		for (int x = upperLeftPosClamped.x; x < bottomRightPosClamped.x; ++x) {
			const int index = y * bufferWidth + x;
			drawBuffer[index] = m_DrawColor;
		}
	}
}
