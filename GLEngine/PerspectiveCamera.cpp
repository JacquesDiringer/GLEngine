#include "stdafx.h"
#include "PerspectiveCamera.h"

#include <math.h>

namespace GLEngine
{
	PerspectiveCamera::PerspectiveCamera(float near, float far, float fov, float ratio)
	{
		// View matrix initialization.
		_view = new Matrix4();
		*_view = Matrix4::Identity();

		// Projection matrix initialization.
		fov = 3.14159265358979323846 * fov / 180.0f;
		float width = 2.0f * near * tanf(fov * 0.5f);
		float height = width * ratio;
		_projection = new Matrix4();
		*_projection = Matrix4::CreateSymetricProjectionFrustum(near, far, height, width);

		_cameraPosition = new Vector3();
	}

	PerspectiveCamera::~PerspectiveCamera()
	{
	}

	void PerspectiveCamera::SetPositionAndTarget(Vector3 cameraPosition, Vector3 targetPosition)
	{
		_view->UpdateTargetPositionCameraYAxis(cameraPosition, targetPosition);
		*_cameraPosition = cameraPosition;
	}

}