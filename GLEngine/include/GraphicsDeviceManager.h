#pragma once

// GLEW
#define GLEW_STATIC
#include<GL\glew.h>

#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;

namespace GLEngine
{
	/// Represents the graphics device current GL states and allows to change them.
	class GraphicsDeviceManager
	{
	public:
		GraphicsDeviceManager();
		virtual ~GraphicsDeviceManager();

		GLboolean GetBooleanState(const GLenum state);
		void SetBooleanState(const GLenum state, const GLboolean value);

		GLboolean GetDepthWriteMask()const { return _depth_writemask; }
		void SetDepthWriteMask(const GLboolean value);

		GLint GetDepthFunc()const { return _depthFunc; }
		void SetDepthFunc(const GLint depthFunc);

		GLint GetCullFace()const { return _cullFace; }
		void SetCullFace(const GLint cullFace);

		GLint GetMaxWorkGroupsCount_x() const { return _maxWorkGroupsCount_x; }
		GLint GetMaxWorkGroupsCount_y() const { return _maxWorkGroupsCount_y; }
		GLint GetMaxWorkGroupsCount_z() const { return _maxWorkGroupsCount_z; }

		GLint GetMaxWorkGroupsSize_x() const { return _maxWorkGroupsSize_x; }
		GLint GetMaxWorkGroupsSize_y() const { return _maxWorkGroupsSize_y; }
		GLint GetMaxWorkGroupsSize_z() const { return _maxWorkGroupsSize_z; }

	private:
		unordered_map<GLenum, GLboolean> _booleanStates;
		GLboolean _depth_writemask = true;
		GLint _depthFunc = GL_LESS;
		GLint _cullFace = GL_BACK;

		GLint _maxWorkGroupsCount_x, _maxWorkGroupsCount_y, _maxWorkGroupsCount_z;
		GLint _maxWorkGroupsSize_x, _maxWorkGroupsSize_y, _maxWorkGroupsSize_z;
	};
}