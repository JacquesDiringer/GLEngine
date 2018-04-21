#pragma once

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngineMath
{
	class GLENGINE_API Vector3
	{
	public:
		Vector3();
		Vector3(float x, float y, float z);
		~Vector3();

		Vector3 Normalize();
		static float Distance(const Vector3 a, const Vector3 b);
		static Vector3 Add(const Vector3 a, const Vector3 b);
		static Vector3 Cross(const Vector3 left, const Vector3 right);
		float Length() const;
		Vector3 operator+(const Vector3 b);
		Vector3 operator-(const Vector3 right) const;
		static Vector3 Multiply(const Vector3 a, const Vector3 b);
		Vector3 operator*(const Vector3 b);
		Vector3 operator*(const float multiplier);
		bool operator==(const Vector3 other);
		bool operator<(const Vector3 other) const; // Necessary operator to be able to but Vector3 in a map structure.


		// Properties
		float X() const { return _x; }
		void X(const float value) { _x = value; }

		float Y() const { return _y; }
		void Y(const float value) { _y = value; }

		float Z() const { return _z; }
		void Z(const float value) { _z = value; }

	private:
		float _x, _y, _z;
	};
}