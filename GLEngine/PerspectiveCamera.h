#pragma once

#include "Matrix4.h"
#include "Vector3.h"

using Math::Matrix4;
using Math::Vector3;

namespace GLEngine
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float near, float far, float fov, float ratio);
		~PerspectiveCamera();

		const Matrix4* GetView() const { return _view; }
		const Matrix4* GetProjection() const { return _projection; }
		Matrix4 GetViewProjection() const { return *_projection * *_view; };

		Vector3* GetPosition() { return _cameraPosition; }

		void SetPositionAndTarget(Vector3 cameraPosition, Vector3 targetPosition);

	private:
		Matrix4* _view, * _projection;
		Vector3* _cameraPosition;
	};
}