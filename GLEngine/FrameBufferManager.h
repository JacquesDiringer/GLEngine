#pragma once

#include "FrameBuffer.h"

namespace GLEngine
{
	class FrameBufferManager
	{
	public:
		FrameBufferManager();
		~FrameBufferManager();

		bool IsDefaultFrameBufferBound() const { return _defaultBound; }

		const FrameBuffer* GetBoundFrameBuffer() const { return _boundFrameBuffer; }

		void Bind(const FrameBuffer* bufferToBind);
		void SetDefaultFrameBuffer();

	private:
		const FrameBuffer* _boundFrameBuffer = nullptr;
		bool _defaultBound = true;
	};
}