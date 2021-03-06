#include <stdafx.h>
#include <Math\Matrix4.h>

#define M_PI 3.14159265358979323846
#include <cmath>

namespace GLEngineMath
{
	Matrix4::Matrix4()
	{
		_m00 = 1; _m01 = 0; _m02 = 0; _m03 = 0;
		_m10 = 0; _m11 = 1; _m12 = 0; _m13 = 0;
		_m20 = 0; _m21 = 0; _m22 = 1; _m23 = 0;
		_m30 = 0; _m31 = 0; _m32 = 0; _m33 = 1;
	}

	Matrix4::Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
	{
		_m00 = m00; _m01 = m01; _m02 = m02; _m03 = m03;
		_m10 = m10; _m11 = m11; _m12 = m12; _m13 = m13;
		_m20 = m20; _m21 = m21; _m22 = m22; _m23 = m23;
		_m30 = m30; _m31 = m31; _m32 = m32; _m33 = m33;
	}

	Matrix4::~Matrix4()
	{
	}

	void Matrix4::CopyFromMatrix4(const Matrix4 * other)
	{
		_m00 = other->_m00;
		_m01 = other->_m01;
		_m02 = other->_m02;
		_m03 = other->_m03;

		_m10 = other->_m10;
		_m11 = other->_m11;
		_m12 = other->_m12;
		_m13 = other->_m13;

		_m20 = other->_m20;
		_m21 = other->_m21;
		_m22 = other->_m22;
		_m23 = other->_m23;

		_m30 = other->_m30;
		_m31 = other->_m31;
		_m32 = other->_m32;
		_m33 = other->_m33;
	}

	void Matrix4::CopyFromValuesArray(const float * values)
	{
		_m00 = values[0];
		_m01 = values[1];
		_m02 = values[2];
		_m03 = values[3];

		_m10 = values[4];
		_m11 = values[5];
		_m12 = values[6];
		_m13 = values[7];

		_m20 = values[8];
		_m21 = values[9];
		_m22 = values[10];
		_m23 = values[11];

		_m30 = values[12];
		_m31 = values[13];
		_m32 = values[14];
		_m33 = values[15];
	}

	Matrix4 Matrix4::Identity()
	{
		return Matrix4();
	}

	Vector3 Matrix4::Position() const
	{
		return Vector3(_m03, _m13, _m23);
	}

	Matrix4 Matrix4::Multiply(const Matrix4& a, const Matrix4& b)
	{
		return a*b;
	}

	void Matrix4::Multiply(const Matrix4 & a, const Matrix4 & b, Matrix4 & result)
	{
		const float values[] = {
		a._m00 * b._m00 + a._m01 * b._m10 + a._m02 * b._m20 + a._m03 * b._m30,
		a._m00 * b._m01 + a._m01 * b._m11 + a._m02 * b._m21 + a._m03 * b._m31,
		a._m00 * b._m02 + a._m01 * b._m12 + a._m02 * b._m22 + a._m03 * b._m32,
		a._m00 * b._m03 + a._m01 * b._m13 + a._m02 * b._m23 + a._m03 * b._m33,
		
		a._m10 * b._m00 + a._m11 * b._m10 + a._m12 * b._m20 + a._m13 * b._m30,
		a._m10 * b._m01 + a._m11 * b._m11 + a._m12 * b._m21 + a._m13 * b._m31,
		a._m10 * b._m02 + a._m11 * b._m12 + a._m12 * b._m22 + a._m13 * b._m32,
		a._m10 * b._m03 + a._m11 * b._m13 + a._m12 * b._m23 + a._m13 * b._m33,
		
		a._m20 * b._m00 + a._m21 * b._m10 + a._m22 * b._m20 + a._m23 * b._m30,
		a._m20 * b._m01 + a._m21 * b._m11 + a._m22 * b._m21 + a._m23 * b._m31,
		a._m20 * b._m02 + a._m21 * b._m12 + a._m22 * b._m22 + a._m23 * b._m32,
		a._m20 * b._m03 + a._m21 * b._m13 + a._m22 * b._m23 + a._m23 * b._m33,
		
		a._m30 * b._m00 + a._m31 * b._m10 + a._m32 * b._m20 + a._m33 * b._m30,
		a._m30 * b._m01 + a._m31 * b._m11 + a._m32 * b._m21 + a._m33 * b._m31,
		a._m30 * b._m02 + a._m31 * b._m12 + a._m32 * b._m22 + a._m33 * b._m32,
		a._m30 * b._m03 + a._m31 * b._m13 + a._m32 * b._m23 + a._m33 * b._m33
		};

		result.CopyFromValuesArray(values);
	}

	Matrix4 Matrix4::operator*(const Matrix4& b) const
	{
		Matrix4 result = Matrix4();

		Multiply(*this, b, result);

		return result;
	}

	Vector3 Matrix4::operator*(const Vector3& position) const
	{
		return Multiply(*this, position);
	}

	Matrix4 Matrix4::operator*(float b) const
	{
		return Matrix4(	this->_m00 * b, this->_m01 * b, this->_m02 * b, this->_m03,
						this->_m10 * b, this->_m11 * b, this->_m12 * b, this->_m13,
						this->_m20 * b, this->_m21 * b, this->_m22 * b, this->_m23,
						this->_m30, this->_m31, this->_m32, this->_m33);
	}

	Vector3 Matrix4::Multiply(const Matrix4& a, const Vector3& position)
	{
		Vector3 result = Vector3();

		Multiply(a, position, result);

		return result;
	}

	void Matrix4::Multiply(const Matrix4& a, const Vector3& position, Vector3& result)
	{
		float pX = position.X();
		float pY = position.Y();
		float pZ = position.Z();

		// We consider that position.W = 1.
		float w = a._m30 * pX;
		w += a._m31 * pY;
		w += a._m32 * pZ;
		w += a._m33;

		float x = a._m00 * pX;
		x += a._m01 * pY;
		x += a._m02 * pZ;
		x += a._m03;
		x /= w;

		float y = a._m10 * pX;
		y += a._m11 * pY;
		y += a._m12 * pZ;
		y += a._m13;
		y /= w;

		float z = a._m20 * pX;
		z += a._m21 * pY;
		z += a._m22 * pZ;
		z += a._m23;
		z /= w;

		result.X(x);
		result.Y(y);
		result.Z(z);
	}

	void Matrix4::Transpose()
	{
		float m01 = this->_m01;
		float m02 = this->_m02;
		float m03 = this->_m03;
		 			
		float m10 = this->_m10;
		float m12 = this->_m12;
		float m13 = this->_m13;
		 			
		float m20 = this->_m20;
		float m21 = this->_m21;
		float m23 = this->_m23;
		 			
		float m30 = this->_m30;
		float m31 = this->_m31;
		float m32 = this->_m32;

		this->_m01 = m10;
		this->_m02 = m20;
		this->_m03 = m30;
				  
		this->_m10 = m01;
		this->_m12 = m21;
		this->_m13 = m31;
				   
		this->_m20 = m02;
		this->_m21 = m12;
		this->_m23 = m32;
				   
		this->_m30 = m03;
		this->_m31 = m13;
		this->_m32 = m23;
	}

	void Matrix4::Invert()
	{
		float m00 = this->_m00;
		float m01 = this->_m01;
		float m02 = this->_m02;
		float m03 = this->_m03;
		float m10 = this->_m10;
		float m11 = this->_m11;
		float m12 = this->_m12;
		float m13 = this->_m13;
		float m20 = this->_m20;
		float m21 = this->_m21;
		float m22 = this->_m22;
		float m23 = this->_m23;
		float m30 = this->_m30;
		float m31 = this->_m31;
		float m32 = this->_m32;
		float m33 = this->_m33;

		float num23 = m22 * m33 - m23 * m32;
		float num22 = m21 * m33 - m23 * m31;
		float num21 = m21 * m32 - m22 * m31;
		float num20 = m20 * m33 - m23 * m30;
		float num19 = m20 * m32 - m22 * m30;
		float num18 = m20 * m31 - m21 * m30;

		float num39 = m11 * num23 - m12 * num22 + m13 * num21;
		float num38 = -(m10 * num23 - m12 * num20 + m13 * num19);
		float num37 = m10 * num22 - m11 * num20 + m13 * num18;
		float num36 = -(m10 * num21 - m11 * num19 + m12 * num18);

		float num = 1.0f / (m00 * num39 + m01 * num38 + m02 * num37 + m03 * num36);

		this->_m00 = num39 * num;
		this->_m10 = num38 * num;
		this->_m20 = num37 * num;
		this->_m30 = num36 * num;

		this->_m01 = -(m01 * num23 - m02 * num22 + m03 * num21) * num;
		this->_m11 = (m00 * num23 - m02 * num20 + m03 * num19) * num;
		this->_m21 = -(m00 * num22 - m01 * num20 + m03 * num18) * num;
		this->_m31 = (m00 * num21 - m01 * num19 + m02 * num18) * num;

		float num35 = m12 * m33 - m13 * m32;
		float num34 = m11 * m33 - m13 * m31;
		float num33 = m11 * m32 - m12 * m31;
		float num32 = m10 * m33 - m13 * m30;
		float num31 = m10 * m32 - m12 * m30;
		float num30 = m10 * m31 - m11 * m30;
		this->_m02 = (m01 * num35 - m02 * num34 + m03 * num33) * num;
		this->_m12 = -(m00 * num35 - m02 * num32 + m03 * num31) * num;
		this->_m22 = (m00 * num34 - m01 * num32 + m03 * num30) * num;
		this->_m32 = -(m00 * num33 - m01 * num31 + m02 * num30) * num;

		float num29 = m12 * m23 - m13 * m22;
		float num28 = m11 * m23 - m13 * m21;
		float num27 = m11 * m22 - m12 * m21;
		float num26 = m10 * m23 - m13 * m20;
		float num25 = m10 * m22 - m12 * m20;
		float num24 = m10 * m21 - m11 * m20;
		this->_m03 = -(m01 * num29 - m02 * num28 + m03 * num27) * num;
		this->_m13 = (m00 * num29 - m02 * num26 + m03 * num25) * num;
		this->_m23 = -(m00 * num28 - m01 * num26 + m03 * num24) * num;
		this->_m33 = (m00 * num27 - m01 * num25 + m02 * num24) * num;
	}

	void Matrix4::InvertRT()
	{
		float m00 = this->_m00;
		float m01 = this->_m01;
		float m02 = this->_m02;
		float m10 = this->_m10;
		float m11 = this->_m11;
		float m12 = this->_m12;
		float m20 = this->_m20;
		float m21 = this->_m21;
		float m22 = this->_m22;

		float x = this->_m03;
		float y = this->_m13;
		float z = this->_m23;

		// Orthonormal basis: transposes the nine upper-left coefficients
		this->_m00 = m00;
		this->_m01 = m10;
		this->_m02 = m20;
		this->_m10 = m01;
		this->_m11 = m11;
		this->_m12 = m21;
		this->_m20 = m02;
		this->_m21 = m12;
		this->_m22 = m22;

		// Translation: uses transposed nine upper-left coefficients
		this->_m03 = -(m00 * x + m10 * y + m20 * z);
		this->_m13 = -(m01 * x + m11 * y + m21 * z);
		this->_m23 = -(m02 * x + m12 * y + m22 * z);

		// Last line
		this->_m30 = this->_m31 = this->_m32 = 0.0f;
		this->_m33 = 1.0f;
	}

	Matrix4 Matrix4::CreateTranslation(const Vector3& translation)
	{
		return Matrix4(translation);
	}

	Matrix4* Matrix4::CreateTranslation(Vector3* translation)
	{
		return new Matrix4(*translation);
	}

	Matrix4 Matrix4::CreateRotationY(float angle)
	{
		float rad = angle * M_PI / 180.0f;
		float cosinus = cos(rad);
		float sinus = sin(rad);

		return Matrix4(	cosinus, 0, sinus, 0,
						0, 1, 0, 0,
						-sinus, 0, cosinus, 0,
						0, 0, 0, 1);
	}

	Matrix4 Matrix4::CreateRotationMatrixFromQuaternion(const Quaternion& quaternion)
	{
		float invs = 1 / sqrtf(quaternion.GetX() * quaternion.GetX() + quaternion.GetY() * quaternion.GetY() + quaternion.GetZ() * quaternion.GetZ() + quaternion.GetW() * quaternion.GetW());
		//float invs = 1;
		float x = quaternion.GetX() * invs;
		float y = quaternion.GetY() * invs;
		float z = quaternion.GetZ() * invs;
		float w = quaternion.GetW() * invs;

		Matrix4 tempMatrix = Matrix4(	1 - 2 * y*y - 2 * z*z,	2 * x*y - 2 * z*w,		2 * x*z + 2 * y*w,		0,
										2 * x*y + 2 * z*w,		1 - 2 * x*x - 2 * z*z,	2 * y*z - 2 * x*w,		0,
										2 * x*z - 2 * y*w,		2 * y*z + 2 * x*w,		1 - 2 * x*x - 2 * y*y,	0,
										0,						0,						0,						1);

		/*Matrix4 tempMatrix = Matrix4(	x*x + y*y - z*z - w*w,	2 * y*z - 2 * x*w,		2 * x*z + 2 * y*w,			0,
										2 * y*z + 2 * x*w,		x*x - y*y + z*z - w*w,	2 * z*w - 2 * x*y,			0,
										2 * y*w - 2 * x*z,		2 * z*w + 2 * x*y,		x*x - y*y - z*z + w*w,		0,
										0,						0,						0,							1);*/

		//tempMatrix.Transpose();

		return tempMatrix;
	}

	Matrix4 Matrix4::CreateScale(float scale)
	{
		return Matrix4(	scale,	0,		0,		0,
						0,		scale,	0,		0,
						0,		0,		scale,	0,
						0,		0,		0,		1);
	}

	Matrix4 Matrix4::CreateSymetricProjectionFrustum(float near, float far, float height, float width)
	{
		return Matrix4(		0.5f * near/width,	0,					0,							0,
							0,					0.5f * near/height, 0,							0,
							0,					0,					-(far + near)/ (far - near), (- 2 * far * near)/(far - near),
							0,					0,					-1,							0);
	}

	Matrix4 Matrix4::CreateTargetPositionCameraYAxis(const Vector3& cameraPosition, const Vector3& targetPosition)
	{
		Matrix4 result = Matrix4::Identity();
		result.UpdateTargetPositionCameraYAxis(cameraPosition, targetPosition);

		return result;
	}

	void Matrix4::UpdateTargetPositionCameraYAxis(const Vector3& cameraPosition, const Vector3& targetPosition)
	{
		Vector3 forward = (cameraPosition - targetPosition).Normalize();
		Vector3 yAxis = Vector3(0, 1, 0);
		Vector3 right = Vector3::Cross(yAxis, forward);
		Vector3 up = Vector3::Cross(forward, right);

		right.Normalize();
		up.Normalize();

		Matrix4 rotation = Identity();
		rotation._m00 = right.X();
		rotation._m01 = right.Y();
		rotation._m02 = right.Z();

		rotation._m10 = up.X();
		rotation._m11 = up.Y();
		rotation._m12 = up.Z();

		rotation._m20 = forward.X();
		rotation._m21 = forward.Y();
		rotation._m22 = forward.Z();

		Matrix4 translation = Matrix4::CreateTranslation(Vector3() - cameraPosition);

		CopyFromMatrix4(&(rotation * translation));
	}

	Matrix4::Matrix4(const Vector3& position)
	{
		_m00 = 1; _m01 = 0; _m02 = 0; _m03 = position.X();
		_m10 = 0; _m11 = 1; _m12 = 0; _m13 = position.Y();
		_m20 = 0; _m21 = 0; _m22 = 1; _m23 = position.Z();
		_m30 = 0; _m31 = 0; _m32 = 0; _m33 = 1;

	}

	Quaternion Matrix4::ComputeQuaternion()
	{
		float x, y, z, w;

		float tr = _m00 + _m11 + _m22;

		if (tr > 0)
		{
			float S = sqrt(tr + 1.0f) * 2; // S=4*qw 
			w = 0.25f * S;
			x = (_m21 - _m12) / S;
			y = (_m02 - _m20) / S;
			z = (_m10 - _m01) / S;
		}
		else if ((_m00 > _m11)&(_m00 > _m22)) {
			float S = sqrt(1.0f + _m00 - _m11 - _m22) * 2; // S=4*qx 
			w = (_m21 - _m12) / S;
			x = 0.25f * S;
			y = (_m01 + _m10) / S;
			z = (_m02 + _m20) / S;
		}
		else if (_m11 > _m22) {
			float S = sqrt(1.0f + _m11 - _m00 - _m22) * 2; // S=4*qy
			w = (_m02 - _m20) / S;
			x = (_m01 + _m10) / S;
			y = 0.25f * S;
			z = (_m12 + _m21) / S;
		}
		else {
			float S = sqrt(1.0f + _m22 - _m00 - _m11) * 2; // S=4*qz
			w = (_m10 - _m01) / S;
			x = (_m02 + _m20) / S;
			y = (_m12 + _m21) / S;
			z = 0.25f * S;
		}

		return Quaternion(x, y, z, w);
	}

	float* Matrix4::GetArray() const
	{
		float* result = new float[16];

		GetArrayCopy(result);

		return result;
	}

	void Matrix4::GetArrayCopy(float* arrayToFill) const
	{
		arrayToFill[0] = _m00;
		arrayToFill[1] = _m01;
		arrayToFill[2] = _m02;
		arrayToFill[3] = _m03;

		arrayToFill[4] = _m10;
		arrayToFill[5] = _m11;
		arrayToFill[6] = _m12;
		arrayToFill[7] = _m13;

		arrayToFill[8] = _m20;
		arrayToFill[9] = _m21;
		arrayToFill[10] = _m22;
		arrayToFill[11] = _m23;

		arrayToFill[12] = _m30;
		arrayToFill[13] = _m31;
		arrayToFill[14] = _m32;
		arrayToFill[15] = _m33;
	}

	void Matrix4::GetTransposedArrayCopy(float* arrayToFill) const
	{
		arrayToFill[0] = _m00;
		arrayToFill[1] = _m10;
		arrayToFill[2] = _m20;
		arrayToFill[3] = _m30;

		arrayToFill[4] = _m01;
		arrayToFill[5] = _m11;
		arrayToFill[6] = _m21;
		arrayToFill[7] = _m31;

		arrayToFill[8] = _m02;
		arrayToFill[9] = _m12;
		arrayToFill[10] = _m22;
		arrayToFill[11] = _m32;

		arrayToFill[12] = _m03;
		arrayToFill[13] = _m13;
		arrayToFill[14] = _m23;
		arrayToFill[15] = _m33;
	}

	bool Matrix4::operator== (Matrix4 const &other) const
	{
		return (
			_m00 == other._m00 &&
			_m01 == other._m01 &&
			_m02 == other._m02 &&
			_m03 == other._m03 &&

			_m10 == other._m10 &&
			_m11 == other._m11 &&
			_m12 == other._m12 &&
			_m13 == other._m13 &&

			_m20 == other._m20 &&
			_m21 == other._m21 &&
			_m22 == other._m22 &&
			_m23 == other._m23 &&

			_m30 == other._m30 &&
			_m31 == other._m31 &&
			_m32 == other._m32 &&
			_m33 == other._m33
			);
	}
	bool Matrix4::IsValid() const
	{
		return	std::isfinite(_m00) &&
			std::isfinite(_m01) &&
			std::isfinite(_m02) &&
			std::isfinite(_m03) &&

			std::isfinite(_m10) &&
			std::isfinite(_m11) &&
			std::isfinite(_m12) &&
			std::isfinite(_m13) &&

			std::isfinite(_m20) &&
			std::isfinite(_m21) &&
			std::isfinite(_m22) &&
			std::isfinite(_m23) &&

			std::isfinite(_m30) &&
			std::isfinite(_m31) &&
			std::isfinite(_m32) &&
			std::isfinite(_m33) &&


			_m00 > -100000000000000 &&
			_m01 > -100000000000000 &&
			_m02 > -100000000000000 &&
			_m03 > -100000000000000 &&
				 
			_m10 > -100000000000000 &&
			_m11 > -100000000000000 &&
			_m12 > -100000000000000 &&
			_m13 > -100000000000000 &&
				 
			_m20 > -100000000000000 &&
			_m21 > -100000000000000 &&
			_m22 > -100000000000000 &&
			_m23 > -100000000000000 &&
				 
			_m30 > -100000000000000 &&
			_m31 > -100000000000000 &&
			_m32 > -100000000000000 &&
			_m33 > -100000000000000 &&


			_m00 < 100000000000000 &&
			_m01 < 100000000000000 &&
			_m02 < 100000000000000 &&
			_m03 < 100000000000000 &&
				 
			_m10 < 100000000000000 &&
			_m11 < 100000000000000 &&
			_m12 < 100000000000000 &&
			_m13 < 100000000000000 &&
				 
			_m20 < 100000000000000 &&
			_m21 < 100000000000000 &&
			_m22 < 100000000000000 &&
			_m23 < 100000000000000 &&
				 
			_m30 < 100000000000000 &&
			_m31 < 100000000000000 &&
			_m32 < 100000000000000 &&
			_m33 < 100000000000000
			;
	}
}