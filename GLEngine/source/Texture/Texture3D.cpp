#include <stdafx.h>
#include <Texture\Texture3D.h>


namespace GLEngine
{
	Texture3D::Texture3D(GLuint id, int width, int height, int depth)
		: _width(width), _height(height), _depth(depth)
	{
		_id = id;
		_boundUnit = -1;
	}

	Texture3D::Texture3D(list<Texture2D*> composerList)
	{
		_boundUnit = -1;

		_width = composerList.front()->GetWidth();
		_height = composerList.front()->GetHeight();
		_depth = composerList.size();

		// Data array generation.
		GLfloat * data = new GLfloat[3 * _width * _height * _depth];

		int texture2DIndex = 0;
		for each (Texture2D* currentTexture2D in composerList)
		{
			if (_width == currentTexture2D->GetWidth() && _height == currentTexture2D->GetHeight())
			{
				glBindTexture(GL_TEXTURE_2D, currentTexture2D->GetId());

				GLfloat * current2DData = new GLfloat[3 * _width * _height];
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, current2DData);

				for (int current2DDataIndex = 0; current2DDataIndex < 3 * _width * _height; current2DDataIndex++)
				{
					data[3 * _width * _height * texture2DIndex + current2DDataIndex] = current2DData[current2DDataIndex];
				}

				// Free templorarily allocated memory.
				delete(current2DData);

				++texture2DIndex;
			}
			else
			{
				throw new std::invalid_argument("Every 2D textures from the argument list should have the same width and height.");
			}
		}

		// 3D texture generation.
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_3D, _id);
		{
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB16F, _width, _height, _depth, 0, GL_RGB, GL_FLOAT, data);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
		glBindTexture(GL_TEXTURE_3D, 0);

		// Free templorarily allocated memory.
		delete data;
	}

	Texture3D::~Texture3D()
	{
		glDeleteTextures(1, &_id);
	}

	const void Texture3D::BindToUnit(const GLint unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);	// Activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_3D, _id);

		_boundUnit = unit;
	}

	const void Texture3D::UnbindFromUnit()
	{
		glActiveTexture(GL_TEXTURE0 + _boundUnit);	// Activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_3D, 0);

		_boundUnit = -1;
	}

}