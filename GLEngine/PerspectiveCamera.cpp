#include "stdafx.h"
#include "PerspectiveCamera.h"
#include "SceneNode.h"

#include <math.h>
#include <exception>

namespace GLEngine
{
	PerspectiveCamera::PerspectiveCamera(float near, float far, float fov, float ratio)
	{
		// View matrix initialization.
		_view = new Matrix4();
		*_view = Matrix4::Identity();
		_viewMatrixIsUpToDate = false;

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

	const Matrix4 * PerspectiveCamera::GetView()
	{
		if (!_viewMatrixIsUpToDate)
		{
			SceneNode* parentNode = GetParentNode();
			if (parentNode != nullptr)
			{
				_view->UpdateTargetPositionCameraYAxis(parentNode->GetWorldTransformation()->Position(), targetPosition);

				_viewMatrixIsUpToDate = true;
			}
			else
			{
				throw new std::exception("Camera should have a parent node.");
			}
		}

		return _view;
	}

	Vector3 PerspectiveCamera::GetPosition()
	{
		SceneNode* parentNode = GetParentNode();
		if (parentNode != nullptr)
		{
			return parentNode->GetWorldTransformation()->Position();
		}
		else
		{
			throw new std::exception("Camera should have a parent node.");
		}
	}

	void PerspectiveCamera::Accept(SceneElementVisitor * visitor)
	{
		visitor->Visit(this);
	}

	//void PerspectiveCamera::SetPositionAndTarget(Vector3 cameraPosition, Vector3 targetPosition)
	//{
	//	_view->UpdateTargetPositionCameraYAxis(cameraPosition, targetPosition);
	//	*_cameraPosition = cameraPosition;
	//}

}