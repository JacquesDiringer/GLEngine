#pragma once

namespace Math
{
	class Vector2
	{
	public:
		Vector2();
		Vector2(float x, float y);
		~Vector2();

		static float Distance(const Vector2 a, const Vector2 b);
		static Vector2 Add(const Vector2 a, const Vector2 b);
		float Length() const;
		Vector2 operator+(const Vector2 b);
		static Vector2 Multiply(const Vector2 a, const Vector2 b);
		Vector2 operator*(const Vector2 b);
		Vector2 operator*(const float multiplier);
		bool operator==(const Vector2 other);
		bool operator<(const Vector2 other) const; // Necessary operator to be able to but Vector3 in a map structure.


												   // Properties
		float X() const { return _x; }
		void X(const float value) { _x = value; }

		float Y() const { return _y; }
		void Y(const float value) { _y = value; }

	private:
		float _x, _y;
	};
}