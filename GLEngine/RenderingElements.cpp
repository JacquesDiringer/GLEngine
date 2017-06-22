#include "stdafx.h"
#include "RenderingElements.h"


namespace GLEngine
{
	RenderingElements::RenderingElements()
	{
		_models = list<Model*>();
	}

	RenderingElements::~RenderingElements()
	{
	}

	Model * RenderingElements::PopModel()
	{
		if (_models.size() > 0)
		{
			Model* result = _models.front();
			_models.pop_front();

			return result;
		}
		else
		{
			return nullptr;
		}
	}
}