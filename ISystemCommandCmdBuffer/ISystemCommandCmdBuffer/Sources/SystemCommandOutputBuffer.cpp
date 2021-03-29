#include "../Headers/SystemCommandOutputBuffer.h"
#include<cstdio>
#include<fstream>
#include<iostream>
void SystemCommandOutputBuffer::BufferCommandOutput() {
	std::string tmpBuffer;
	std::ifstream filestream(BufferedFilename);
	if (filestream.is_open()) {
		while (getline(filestream,tmpBuffer))
		{
			std::cout << tmpBuffer << std::endl;
			Buffer.push_back(tmpBuffer);
		}
	}
	else {
		std::cout << "error" << std::endl;
	}
}

void SystemCommandOutputBuffer::CreateTempCommandOutputFile(const std::string systemcommand, const std::string filename) {
	BufferedFilename = (std::string)filename;
#ifdef _WIN32
	std::string BufferedSystemCommand = (std::string)systemcommand + " 1> " + (std::string)filename + " 2>&1";
#else
	std::string BufferedSystemCommand = (std::string)systemcommand + " 2>&1 | tee " + (std::string)filename;
#endif
	system(BufferedSystemCommand.c_str());
}

void SystemCommandOutputBuffer::DeleteTempCommandOutputFile() {
	std::remove(BufferedFilename.c_str());
}