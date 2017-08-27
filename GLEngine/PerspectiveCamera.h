#pragma once

#include <list>

#include "Matrix4.h"
#include "Vector3.h"
#include "SceneElement.h"
#include "SceneElementVisitor.h"

using Math::Matrix4;
using Math::Vector3;

using std::list;

namespace GLEngine
{
	class PostProcess;

	class PerspectiveCamera
		: public SceneElement
	{
	public:
		PerspectiveCamera(float near, float far, float fov, float ratio);
		~PerspectiveCamera();

		const Matrix4* GetView();
		const Matrix4* GetProjection() const { return _projection; }

		Vector3 GetPosition();

		virtual void Accept(SceneElementVisitor* visitor);

		void AddPostProcess(PostProcess* newPostProcess);
		list<PostProcess*> GetPostProcesses() { return _postProcesses; }

	private:
		Matrix4* _view, * _projection;
		Matrix4* _previousParentWorld;
		list<PostProcess*> _postProcesses;
	};
}