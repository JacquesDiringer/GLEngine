#include "stdafx.h"
#include "VBOManager.h"

namespace GLEngine
{

	VBOManager::VBOManager()
	{
		_usedArrayBuffers = unordered_map<GLsizeiptr, unordered_set<GLuint>>();
		_unusedArrayBuffers = map<GLsizeiptr, vector<GLuint>>();
	}


	VBOManager::~VBOManager()
	{
	}

	GLuint VBOManager::RequestBuffer(const GLsizeiptr requestedSize)
	{

		// We browse through array buffers from smallest to biggest, until we find one big enough. The vector also has to actually contain at least one buffer ID.
		auto unusedBufferIterator = _unusedArrayBuffers.begin();
		while (	requestedSize > (*unusedBufferIterator).first &&
				(*unusedBufferIterator).second.size() > 0) {}

		if (unusedBufferIterator != _unusedArrayBuffers.end())
		{
			// If we found a suitable unused buffer vector.
			GLsizeiptr unusedBufferSize = (*unusedBufferIterator).first;
			vector<GLuint> unusedBufferVector = (*unusedBufferIterator).second;
			GLuint unusedBufferId = (*unusedBufferIterator).second.back(); // We take the last ID in the vector, we know that it contains at least one.

			// Erase this ID from unused buffers, hence the last ID of the vector.
			unusedBufferVector.pop_back();

			// Add this ID to the used buffers.
			auto usedBufferIterator = _usedArrayBuffers.find(unusedBufferSize);
			if (usedBufferIterator != _usedArrayBuffers.end())
			{
				// If there is already a vector for this size, add the new ID to it.
				(*usedBufferIterator).second.insert(unusedBufferId);
			}
			else
			{
				// If it does not already exist, create one.
				unordered_set<GLuint> newSizeBufferIdVector = unordered_set<GLuint>();
				newSizeBufferIdVector.insert(unusedBufferId);

				// And insert it in the unordered map.
				_usedArrayBuffers.insert_or_assign(unusedBufferSize, newSizeBufferIdVector);
			}

			return unusedBufferId;
		}
		else
		{
			// Otherwise we create a new buffer.
			GLuint newVBO;
			glGenBuffers(1, &newVBO);
			// Allocate the memory.
			glBufferData(GL_ARRAY_BUFFER, requestedSize, NULL, GL_STREAM_DRAW);

			// Add this ID to the used buffers.
			auto usedBufferIterator = _usedArrayBuffers.find(requestedSize);
			if (usedBufferIterator != _usedArrayBuffers.end())
			{
				// If there is already a vector for this size, add the new ID to it.
				(*usedBufferIterator).second.insert(newVBO);
			}
			else
			{
				// If it does not already exist, create one.
				unordered_set<GLuint> newSizeBufferIdVector = unordered_set<GLuint>();
				newSizeBufferIdVector.insert(newVBO);

				// And insert it in the unordered map.
				_usedArrayBuffers.insert_or_assign(requestedSize, newSizeBufferIdVector);
			}

			return newVBO;
		}
	}

	void VBOManager::FreeBuffer(const GLuint bufferToFree)
	{
		GLsizeiptr foundBufferSize = 0;

		// Find the corresponding id and it's size.
		// Then remove them from the used buffers map.
		for each (auto bufferMapElement in _usedArrayBuffers)
		{
			unordered_set<GLuint>& currentVector = bufferMapElement.second;
			if (currentVector.find(bufferToFree) != currentVector.end())
			{
				// Retrieve the associated size.
				foundBufferSize = bufferMapElement.first;
				// Remove this ID from the ID vector.
				currentVector.erase(bufferToFree);
			}
		}

		if (foundBufferSize > 0)
		{
			// Add this ID to the unused buffers.
			auto unusedBufferIterator = _unusedArrayBuffers.find(foundBufferSize);
			if (unusedBufferIterator != _unusedArrayBuffers.end())
			{
				// If there is already a vector for this size, add the new ID to it.
				(*unusedBufferIterator).second.push_back(bufferToFree);
			}
			else
			{
				// If it does not already exist, create one.
				vector<GLuint> newSizeBufferIdVector = vector<GLuint>();
				newSizeBufferIdVector.push_back(bufferToFree);

				// And insert it in the unordered map.
				_unusedArrayBuffers.insert_or_assign(foundBufferSize, newSizeBufferIdVector);
			}
		}
	}
}