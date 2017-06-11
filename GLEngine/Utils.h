#pragma once

#include <string>
#include <iostream>
#include <fstream>

using std::string;
using std::fread;

namespace GLEngine
{
	class Utils
	{
	public:
		Utils();
		~Utils();

		// Returns the content of the file located at fileName.
		// Throws an invalid parameter exception when file cannot be found.
		static string ReadFile(string fileName);
	};
}