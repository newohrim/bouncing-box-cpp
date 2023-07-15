#pragma once

enum class InputState {
	NONE = 0,
	KEY_SPACE = 1 << 0,
	KEY_LEFT = 1 << 1,
	KEY_RIGHT = 1 << 2,
	KEY_UP = 1 << 3,
	KEY_DOWN = 1 << 4,
};

inline InputState operator|(InputState lhs, InputState rhs)
{
	return static_cast<InputState>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

inline InputState& operator |=(InputState& lhs, InputState rhs)
{
	lhs = lhs | rhs;
	return lhs;
}

inline bool IsKeyDown(InputState state, InputState key) 
{
	return ((int)state & (int)key) > 0;
}
