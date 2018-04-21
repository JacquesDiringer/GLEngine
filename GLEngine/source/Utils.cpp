#include <stdafx.h>
#include "Utils.h"


namespace GLEngine
{
	Utils::Utils()
	{
	}


	Utils::~Utils()
	{
	}

	string Utils::ReadFile(string fileName)
	{
		std::string fileSource;
		std::ifstream file = std::ifstream(fileName);
		std::string currentLine;
		if (file.is_open())
		{
			while (file.good())
			{
				std::getline(file, currentLine);
				fileSource.append(currentLine + '\n');
			}
		}
		else
		{
			throw new std::invalid_argument("Could not find file.");
		}
		
		return fileSource;
	}

}