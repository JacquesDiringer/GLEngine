#include "stdafx.h"
#include "SceneElement.h"


namespace GLEngine
{
	SceneElement::SceneElement()
	{
		_parentNode = nullptr;
	}


	SceneElement::~SceneElement()
	{
	}

	void SceneElement::SetIsUpToDate(const bool value)
	{
		// Void
	}
}