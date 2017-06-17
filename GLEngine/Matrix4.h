#pragma once

#include "Vector3.h"
#include "Quaternion.h"

namespace Math
{
	/// This class describes 4x4 translation, rotation, scale matrices
	class Matrix4
	{
	public:
		Matrix4();
		Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
		Matrix4(Vector3 position);
		~Matrix4();
		
		void CopyFromMatrix4(Matrix4* other);

		static Matrix4 Identity();
		static Matrix4 Multiply(Matrix4 a, Matrix4 b);
		Matrix4 operator* (Matrix4 b);
		Matrix4 operator* (Vector3 b);
		Matrix4 operator* (float b);
		static Vector3 Multiply(Matrix4 a, Vector3 position);
		Matrix4 Transpose();

		static Matrix4 CreateTranslation(Vector3 translation);
		static Matrix4 CreateRotationY(float angle); // angle in degrees
		static Matrix4 CreateRotationMatrixFromQuaternion(Quaternion quaternion);
		static Matrix4 CreateSymetricProjectionFrustum(float near, float far, float height, float width);
		static Matrix4 CreateTargetPositionCameraYAxis(const Vector3 cameraPosition, const Vector3 targetPosition);

		void UpdateTargetPositionCameraYAxis(const Vector3 cameraPosition, const Vector3 targetPosition);

		// Interface
		Vector3 Position() const;
		Quaternion ComputeQuaternion();
		float* GetArray() const;

		// Operators
		bool operator== (Matrix4 const &other) const;


	private:
		float _m00, _m01, _m02, _m03;
		float _m10, _m11, _m12, _m13;
		float _m20, _m21, _m22, _m23;
		float _m30, _m31, _m32, _m33;

		friend class Matrix4;
	};
}