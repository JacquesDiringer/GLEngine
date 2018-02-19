#include <stdafx.h>
#include "GraphicsDeviceManager.h"


namespace GLEngine
{
	GraphicsDeviceManager::GraphicsDeviceManager()
	{
		_booleanStates = unordered_map<GLenum, GLboolean>();

		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &_maxWorkGroupsCount_x);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &_maxWorkGroupsCount_y);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &_maxWorkGroupsCount_z);

		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &_maxWorkGroupsSize_x);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &_maxWorkGroupsSize_y);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &_maxWorkGroupsSize_z);
	}


	GraphicsDeviceManager::~GraphicsDeviceManager()
	{
	}

	GLboolean GraphicsDeviceManager::GetBooleanState(const GLenum state)
	{
		// Look for this state in the map.
		auto resultIt = _booleanStates.find(state);

		if (resultIt != _booleanStates.end())
		{
			return (*resultIt).second;
		}
		else
		{
			// If the state has never been set before but we try to get it, set it to false, then return this false.
			SetBooleanState(state, false);
			return false;
		}
	}

	void GraphicsDeviceManager::SetBooleanState(const GLenum state, const GLboolean value)
	{
		auto boolIt = _booleanStates.find(state);

		// Change the state only if the state needs to be changed.
		// Thus, if the state has never been set before, or it's current value is different from the new one.
		if (boolIt == _booleanStates.end() || (*boolIt).second != value)
		{
			// Call the GL function.
			value ? glEnable(state) : glDisable(state);

			// Insert the element or replace it if it already exists.
			_booleanStates.insert_or_assign(state, value);
		}
	}

	void GraphicsDeviceManager::SetDepthWriteMask(const GLboolean value)
	{
		// Change the state only if the state needs to be changed.
		if (value != _depth_writemask)
		{
			// Call the GL function.
			glDepthMask(value);

			// Keep the value.
			_depth_writemask = value;
		}
	}

	void GraphicsDeviceManager::SetDepthFunc(const GLint depthFunc)
	{
		// Change the state only if the state needs to be changed.
		if (depthFunc != _depthFunc)
		{
			// Call the GL function.
			glDepthFunc(depthFunc);

			// Keep the value.
			_depthFunc = depthFunc;
		}
	}

	void GraphicsDeviceManager::SetCullFace(const GLint cullFace)
	{
		// Change the state only if the state needs to be changed.
		if (cullFace != _cullFace)
		{
			// Call the GL function.
			glCullFace(cullFace);

			// Keep the value.
			_cullFace = cullFace;
		}
	}
}