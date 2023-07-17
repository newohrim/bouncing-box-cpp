#pragma once

#include <limits>
#include <cmath>
#include <cassert>

namespace Math {
	struct Vec2D;
	struct Vec2DF;
	struct Vec2DFN;

	static constexpr float EPSILON = std::numeric_limits<float>::epsilon() * 10;

#pragma region VEC2D_INT
	// Vector with integer components
	struct Vec2D 
	{
		int x = 0;
		int y = 0;

		explicit operator Vec2DF() const;
	};

	static Vec2D operator+(const Vec2D& lhs, int rhs)
	{
		return Vec2D{ lhs.x + rhs, lhs.y + rhs };
	}

	static Vec2D operator*(const Vec2D& lhs, int rhs)
	{
		return Vec2D{ lhs.x * rhs, lhs.y * rhs };
	}

	static Vec2D operator*(const Vec2D& lhs, float rhs) 
	{
		return Vec2D{ (int)std::round(lhs.x * rhs), (int)std::round(lhs.y * rhs) };
	}

	static Vec2D operator+(const Vec2D& lhs, const Vec2D& rhs)
	{
		return Vec2D{ lhs.x + rhs.x, lhs.y + rhs.y };
	}

	static Vec2D operator-(const Vec2D& lhs, const Vec2D& rhs)
	{
		return Vec2D{ lhs.x - rhs.x, lhs.y - rhs.y };
	}

	static Vec2D operator*(const Vec2D& lhs, const Vec2D& rhs) 
	{
		return Vec2D{lhs.x * rhs.x, lhs.y * rhs.y};
	}
#pragma endregion
#pragma region VEC2D_FLOAT
	// Vector with floating point components
	struct Vec2DF 
	{
		float x = 0.0f;
		float y = 0.0f;

		explicit operator Vec2D() const;
	};

	static Vec2DF operator+(const Vec2DF& lhs, float rhs)
	{
		return Vec2DF{ lhs.x + rhs, lhs.y + rhs };
	}

	static Vec2DF operator*(const Vec2DF& lhs, float rhs)
	{
		return Vec2DF{ lhs.x * rhs, lhs.y * rhs };
	}

	static Vec2DF operator+(const Vec2DF& lhs, const Vec2DF& rhs)
	{
		return Vec2DF{ lhs.x + rhs.x, lhs.y + rhs.y };
	}

	static Vec2DF operator-(const Vec2DF& lhs, const Vec2DF& rhs)
	{
		return Vec2DF{ lhs.x - rhs.x, lhs.y - rhs.y };
	}

	static Vec2DF operator*(const Vec2DF& lhs, const Vec2DF& rhs)
	{
		return Vec2DF{ lhs.x * rhs.x, lhs.y * rhs.y };
	}
#pragma endregion
#pragma region VEC2D_FLOAT_NORMALIZED
	// Normalized verison of Vec2DF
	struct Vec2DFN : public Vec2DF {
		Vec2DFN() = default;
		Vec2DFN(float x, float y) : Vec2DF{ x, y }
		{
			const float test = abs(1.0f - x * x + y * y);
			assert(abs(1.0f - (x * x + y * y)) < EPSILON);
		}

		operator Vec2DF() const;
	};
#pragma endregion

	template<class T>
	static T Clamp(T min, T max, T value) {
		if (value < min) {
			value = min;
		}
		if (value > max) {
			value = max;
		}

		return value;
	}

	inline Vec2D::operator Vec2DF() const
	{
		return Vec2DF{ (float)x, (float)y };
	}

	inline Vec2DF::operator Vec2D() const
	{
		return Vec2D{ (int)x, (int)y };
	}

	inline Vec2DFN::operator Vec2DF() const
	{
		return Vec2DF{ x, y };
	}

	static float Dot(Vec2DF vecA, Vec2DF vecB) 
	{
		return vecA.x * vecB.x + vecA.y * vecB.y;
	}

	static float LengthSq(Vec2DF vec) 
	{
		return Dot(vec, vec);
	}

	static float Length(Vec2DF vec) 
	{
		return sqrt(LengthSq(vec));
	}

	static Vec2DFN Normalize(Vec2DF vec) 
	{
		// TODO: rewrite this mess
		const float vecLength = Length(vec);
		assert(vecLength > EPSILON);
		vec = vec * (1 / vecLength);
		return Vec2DFN{vec.x, vec.y};
	}
}
