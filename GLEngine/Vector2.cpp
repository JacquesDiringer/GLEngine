#include "stdafx.h"
#include "Vector2.h"

#include <math.h>

namespace Math
{
	Vector2::Vector2()
	{
		_x = _y = 0;
	}

	Vector2::Vector2(float x, float y)
		: _x(x), _y(y)
	{
		// void
	}


	Vector2::~Vector2()
	{
	}

	float Vector2::Distance(const Vector2 a, const Vector2 b)
	{
		return sqrtf(
			pow(a.X() - b.X(), 2) +
			pow(a.Y() - b.Y(), 2));
	}

	Vector2 Vector2::Add(const Vector2 a, const Vector2 b)
	{
		return Vector2(
			a.X() + b.X(),
			a.Y() + b.Y());
	}

	float Vector2::Length() const
	{
		return Vector2::Distance(*this, Vector2());
	}

	Vector2 Vector2::operator+(const Vector2 b)
	{
		return Vector2(
			X() + b.X(),
			Y() + b.Y());
	}

	Vector2 Vector2::Multiply(const Vector2 a, const Vector2 b)
	{
		return Vector2(
			a.X() * b.X(),
			a.Y() * b.Y());
	}

	Vector2 Vector2::operator*(const Vector2 b)
	{
		return Vector2(
			X() * b.X(),
			Y() * b.Y());
	}

	Vector2 Vector2::operator*(const float multiplier)
	{
		return Vector2(
			X() * multiplier,
			Y() * multiplier);
	}

	bool Vector2::operator==(const Vector2 other)
	{
		return _x == other.X() && _y == other.Y();
	}

	bool Vector2::operator<(const Vector2 other) const
	{
		if (_x != other.X())
		{
			return _x < other.X();
		}

		return _y < other.Y();
	}
}