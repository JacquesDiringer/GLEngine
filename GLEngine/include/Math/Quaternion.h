#pragma once

namespace GLEngineMath
{
	class Quaternion
	{
	public:
		Quaternion();
		Quaternion(float x, float y, float z, float w);
		~Quaternion();

		float GetX() const { return _x; }
		float GetY() const { return _y; }
		float GetZ() const { return _z; }
		float GetW() const { return _w; }

		friend class Matrix4;

	private:
		float _x, _y, _z, _w;
	};

}
