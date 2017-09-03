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

		// IView matrix initialization.
		_iView = new Matrix4();
		*_iView = Matrix4::Identity();

		// Projection matrix initialization.
		fov = 3.14159265358979323846 * fov / 180.0f;
		float width = 2.0f * near * tanf(fov * 0.5f);
		float height = width * ratio;
		_projection = new Matrix4();
		*_projection = Matrix4::CreateSymetricProjectionFrustum(near, far, height, width);

		// Set the inverse of the projection.
		_iProjection = new Matrix4();
		_iProjection->CopyFromMatrix4(_projection);
		_iProjection->Invert();

		_postProcesses = list<PostProcess*>();
	}

	PerspectiveCamera::~PerspectiveCamera()
	{

	}

	void PerspectiveCamera::UpdateViewAndIView()
	{
		SceneNode* parentNode = GetParentNode();
		if (parentNode != nullptr)
		{
			Matrix4 newView = Matrix4(*parentNode->GetWorldTransformation());
			if (!(*_iView == newView))
			{
				// Update the inverse of the view.
				*_iView = newView;

				newView.InvertRT();
				// Update the view.
				*_view = newView;
			}
		}
		else
		{
			throw new std::exception("Camera should have a parent node.");
		}
	}

	const Matrix4 * PerspectiveCamera::GetView()
	{
		UpdateViewAndIView();

		return _view;
	}

	const Matrix4 * PerspectiveCamera::GetIView()
	{
		UpdateViewAndIView();

		return _iView;
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

	void PerspectiveCamera::AddPostProcess(PostProcess * newPostProcess)
	{
		_postProcesses.push_back(newPostProcess);
	}
}