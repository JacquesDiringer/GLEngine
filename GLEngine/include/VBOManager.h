#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include <unordered_set>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

using std::map;
using std::unordered_map;
using std::vector;
using std::unordered_set;

namespace GLEngine
{
	// This class manages array buffers.
	// It implements pooling, to make sure created array buffers that are not currently used get reused.
	class VBOManager
	{
	public:
		VBOManager();
		virtual ~VBOManager();

		// Returns the id of an array buffer with requestedSize as it's minimum allocated storage.
		GLuint RequestBuffer(const GLsizeiptr requestedSize);

		// Tags the buffer as unused.
		void FreeBuffer(const GLuint bufferToFree);

	private:
		unordered_map<GLsizeiptr, unordered_set<GLuint>> _usedArrayBuffers;
		// Unused buffers need to a map because we are going to browse through them from smallest to biggest.
		map<GLsizeiptr, vector<GLuint>> _unusedArrayBuffers;
	};

}
