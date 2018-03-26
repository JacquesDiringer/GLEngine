#include <stdafx.h>
#include <Math\Quaternion.h>


namespace GLEngineMath
{
	Quaternion::Quaternion()
	{
	}


	Quaternion::~Quaternion()
	{
	}

	Quaternion::Quaternion(float x, float y, float z, float w)
		: _x(x), _y(y), _z(z), _w(w)
	{
		// Void
	}
}