#pragma once

#include "Component.h"

union ColorRGBA {
	struct {
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};
	uint32_t value;
	uint8_t rgba[sizeof(uint32_t) / sizeof(uint8_t)];
};

class PixelSpriteComponent : public Component
{
public:
	PixelSpriteComponent(GameActor* actor, int width, int height);
	~PixelSpriteComponent();

	void DrawPixels(uint32_t* buffer, int bufferWidth, int bufferHeight);

	ColorRGBA GetDrawColor() const { return m_DrawColor; }
	void SetDrawColor(ColorRGBA color) { m_DrawColor = color; }

private:
	ColorRGBA m_DrawColor{255, 255, 255, 255};

	int m_Width;
	int m_Height;
	// drawer ptr
};

