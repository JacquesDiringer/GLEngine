#pragma once

#include "Matrix4.h"
#include "Vector3.h"
#include"SceneElement.h"
#include"SceneElementVisitor.h"

using Math::Matrix4;
using Math::Vector3;

namespace GLEngine
{
	class PerspectiveCamera
		: public SceneElement
	{
	public:
		PerspectiveCamera(float near, float far, float fov, float ratio);
		~PerspectiveCamera();

		const Matrix4* GetView();
		const Matrix4* GetProjection() const { return _projection; }
		Matrix4 GetViewProjection() const { return *_projection * *_view; };

		Vector3 GetPosition();

		virtual void Accept(SceneElementVisitor* visitor);

		//void SetPositionAndTarget(Vector3 cameraPosition, Vector3 targetPosition);

	private:
		Matrix4* _view, * _projection;
		bool _viewMatrixIsUpToDate;
	};
}