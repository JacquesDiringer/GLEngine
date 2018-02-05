#include <stdafx.h>
#include <FrameBuffer\FrameBuffer.h>
#include <FrameBuffer\FrameBufferManager.h>


namespace GLEngine
{
	FrameBuffer::FrameBuffer(int width, int height, FrameBufferManager* manager)
		: _manager(manager)
	{
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &_id);
	}

	void FrameBuffer::Bind() const
	{
		_manager->Bind(this);
	}

	void FrameBuffer::UnBind() const
	{
		_manager->SetDefaultFrameBuffer();
	}
}