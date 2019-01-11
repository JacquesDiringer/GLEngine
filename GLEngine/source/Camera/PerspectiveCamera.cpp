#include <stdafx.h>
#include <Camera\PerspectiveCamera.h>
#include <SceneGraph\SceneNode.h>

#include <math.h>
#include <exception>

namespace GLEngine
{
	PerspectiveCamera::PerspectiveCamera(float near, float far, float fov, float ratio)
	{
		// View matrix initialization.
		_view = GLEngineMath::Matrix4::Identity();

		// IView matrix initialization.
		_iView = GLEngineMath::Matrix4::Identity();

		// Projection matrix initialization.
		fov = 3.14159265358979323846 * fov / 180.0f;
		float width = 2.0f * near * tanf(fov * 0.5f);
		float height = width * ratio;
		_projection = GLEngineMath::Matrix4::CreateSymetricProjectionFrustum(near, far, height, width);

		// Set the inverse of the projection.
		_iProjection = _projection;
		_iProjection.Invert();

		_viewProjection = _projection * _view;

		_postProcesses = vector<PostProcess*>();
	}

	PerspectiveCamera::~PerspectiveCamera()
	{

	}

	void PerspectiveCamera::UpdateViewAndIView()
	{
		// Cache the SceneNode parent if found.
		if (_parentNode == nullptr)
		{
			_parentNode = dynamic_cast<SceneNode*>(GetParent());

			if(_parentNode == nullptr)
			{
				throw new std::exception("Camera should have a parent node.");
			}
		}
		
		GLEngineMath::Matrix4 newView = _parentNode->GetWorldTransformation();
		if (!(_iView == newView))
		{
			// Update the inverse of the view.
			_iView = newView;

			newView.InvertRT();
			// Update the view.
			_view = newView;

			// Update the viewProjection.
			_viewProjection = _projection * _view;
		}
	}

	const GLEngineMath::Matrix4& PerspectiveCamera::GetView()
	{
		UpdateViewAndIView();

		return _view;
	}

	const GLEngineMath::Matrix4& PerspectiveCamera::GetIView()
	{
		UpdateViewAndIView();

		return _iView;
	}

	GLEngineMath::Vector3 PerspectiveCamera::GetPosition()
	{
		SceneNode* parentNode = dynamic_cast<SceneNode*>(GetParent());
		if (parentNode != nullptr)
		{
			return parentNode->GetWorldTransformation().Position();
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