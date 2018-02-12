#pragma once

#include <vector>

#include <Math\Matrix4.h>
#include <Math\Vector3.h>
#include <SceneGraph\SceneElement.h>
#include <SceneGraph\SceneElementVisitor.h>

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

		const Matrix4& GetView();
		const Matrix4& GetProjection() const { return _projection; }

		const Matrix4& GetIView();
		const Matrix4& GetIProjection() const { return _iProjection; }

		Vector3 GetPosition();

		virtual void Accept(SceneElementVisitor* visitor);

		void AddPostProcess(PostProcess* newPostProcess);
		const vector<PostProcess*>& GetPostProcesses() const { return _postProcesses; }

	private:
		Matrix4 _view,  _iView, _projection, _iProjection;
		vector<PostProcess*> _postProcesses;

		void UpdateViewAndIView();
	};
}