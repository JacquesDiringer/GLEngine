#pragma once

#include <vector>

#include "Matrix4.h"
#include "Vector3.h"
#include "SceneElement.h"
#include "SceneElementVisitor.h"

using Math::Matrix4;
using Math::Vector3;

using std::vector;

namespace GLEngine
{
	class PostProcess;

	class PerspectiveCamera
		: public SceneElement
	{
	public:
		PerspectiveCamera(float near, float far, float fov, float ratio);
		~PerspectiveCamera();

		const Matrix4* GetView() const;
		const Matrix4* GetProjection() const { return _projection; }

		const Matrix4* GetIView() const;
		const Matrix4* GetIProjection() const { return _iProjection; }

		Vector3 GetPosition();

		virtual void Accept(SceneElementVisitor* visitor);

		void AddPostProcess(PostProcess* newPostProcess);
		vector<PostProcess*> GetPostProcesses() { return _postProcesses; }

	private:
		Matrix4* _view,  * _iView, * _projection, * _iProjection;
		vector<PostProcess*> _postProcesses;

		void UpdateViewAndIView() const;
	};
}