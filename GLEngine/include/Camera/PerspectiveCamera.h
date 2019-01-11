#pragma once

#include <vector>

#include <Math\Matrix4.h>
#include <Math\Vector3.h>
#include <SceneGraph\SceneElement.h>
#include <SceneGraph\SceneElementVisitor.h>

using std::vector;

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class PostProcess;

	class GLENGINE_API PerspectiveCamera
		: public SceneElement
	{
	public:
		PerspectiveCamera(float near, float far, float fov, float ratio);
		~PerspectiveCamera();

		const GLEngineMath::Matrix4& GetView();
		const GLEngineMath::Matrix4& GetProjection() const { return _projection; }

		const GLEngineMath::Matrix4& GetIView();
		const GLEngineMath::Matrix4& GetIProjection() const { return _iProjection; }

		const GLEngineMath::Matrix4& GetViewProjection() const { return _viewProjection; }

		GLEngineMath::Vector3 GetPosition();

		virtual void Accept(SceneElementVisitor* visitor);

		void AddPostProcess(PostProcess* newPostProcess);
		const vector<PostProcess*>& GetPostProcesses() const { return _postProcesses; }

	private:
		GLEngineMath::Matrix4 _view,  _iView, _projection, _iProjection, _viewProjection;
		vector<PostProcess*> _postProcesses;

		void UpdateViewAndIView();
	};
}