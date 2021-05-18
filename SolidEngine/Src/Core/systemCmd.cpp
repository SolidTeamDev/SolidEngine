//
// Created by ryan1 on 16/05/2021.
//
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include "Core/systemCmd.hpp"

#ifdef _WIN32


int wincmd(std::string cmd, std::vector<std::string> & output, unsigned int maxOutput)
{

	FILE* out = _popen((cmd+ " 2>&1").c_str(), "rt");

	while (!std::feof(out))
	{
		std::string outputS;
		outputS.clear();
		outputS.resize(maxOutput, '\0');
		std::fgets(outputS.data(),maxOutput,out);

		int find =outputS.find_first_of('\0');


		if(find != std::string::npos)
			outputS.resize(find);
		outputS.shrink_to_fit();
		output.push_back(std::move(outputS));
	}

	int errorCOde = _pclose(out);
	return errorCOde;

}
#endif
