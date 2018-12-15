#pragma once

#include <Math\Vector3.h>
#include <Math\Quaternion.h>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngineMath
{
	/// This class describes 4x4 translation, rotation, scale matrices
	class GLENGINE_API Matrix4
	{
	public:
		Matrix4();
		Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
		Matrix4(const Vector3& position);
		~Matrix4();
		
		void CopyFromMatrix4(const Matrix4* other);

		static Matrix4 Identity();
		static Matrix4 Multiply(const Matrix4& a, const Matrix4& b);
		Matrix4 operator* (const Matrix4& b) const;
		Vector3 operator* (const Vector3& b) const;
		Matrix4 operator* (float b) const;
		static Vector3 Multiply(const Matrix4& a, const Vector3& position);
		void Transpose();
		// Inverts a generic invertible matrix.
		void Invert();
		//Inverts a rigid transformation matrix. Its last line is supposed to be (0, 0, 0, 1) and the nine upper-left coefficients are supposed to represent an orthonormal basis.
		void InvertRT();

		static Matrix4 CreateTranslation(const Vector3& translation);
		static Matrix4* CreateTranslation(Vector3* translation);
		static Matrix4 CreateRotationY(float angle); // angle in degrees
		static Matrix4 CreateRotationMatrixFromQuaternion(const Quaternion& quaternion);
		static Matrix4 CreateScale(float scale);
		static Matrix4 CreateSymetricProjectionFrustum(float near, float far, float height, float width);
		static Matrix4 CreateTargetPositionCameraYAxis(const Vector3& cameraPosition, const Vector3& targetPosition);

		void UpdateTargetPositionCameraYAxis(const Vector3& cameraPosition, const Vector3& targetPosition);

		// Interface
		Vector3 Position() const;
		Quaternion ComputeQuaternion();
		float* GetArray() const;

		// Operators
		bool operator== (Matrix4 const &other) const;

		// Returns true if all it's values are non degenerate.
		bool IsValid() const;


	private:
		float _m00, _m01, _m02, _m03;
		float _m10, _m11, _m12, _m13;
		float _m20, _m21, _m22, _m23;
		float _m30, _m31, _m32, _m33;

		friend class Matrix4;
	};
}