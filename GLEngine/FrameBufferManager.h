#pragma once

#include "FrameBuffer.h"

namespace GLEngine
{
	class FrameBufferManager
	{
	public:
		FrameBufferManager();
		~FrameBufferManager();

		bool IsDefaultFrameBufferBound() { return _defaultBound; }

		FrameBuffer* GetBoundFrameBuffer() { return _boundFrameBuffer; }

		void Bind(FrameBuffer* bufferToBind);
		void SetDefaultFrameBuffer();

	private:
		FrameBuffer* _boundFrameBuffer = nullptr;
		bool _defaultBound = true;
	};
}