#ifndef FILE_INCLUDER_H
#define FILE_INCLUDER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

namespace Helper {

	const char* ImportFile(const std::string path)
	{
		std::ifstream stream;

		try
		{
			stream.open(path);

			std::stringstream strStream;
			strStream << stream.rdbuf();
			stream.close();

			std::string str = strStream.str();

			return strdup(str.c_str());



		}
		catch (std::stringstream::failure e)
		{
			std::cout << "Could not open a file!" << std::endl;
		}
	}
}

#endif
