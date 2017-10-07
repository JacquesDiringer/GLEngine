#pragma once

#include "FrameBuffer.h"

namespace GLEngine
{
	class FrameBufferManager
	{
	public:
		FrameBufferManager();
		~FrameBufferManager();

		bool IsDefaultFrameBufferBound() { return _boundFrameBuffer == nullptr; }

		FrameBuffer* GetBoundFrameBuffer() { return _boundFrameBuffer; }

		void Bind(FrameBuffer* bufferToBind);
		void SetDefaultFrameBuffer();

	private:
		FrameBuffer* _boundFrameBuffer = nullptr;
	};
}