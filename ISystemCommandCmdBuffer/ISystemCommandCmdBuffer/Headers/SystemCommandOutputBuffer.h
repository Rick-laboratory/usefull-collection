#pragma once
#include"Interfaces/ISystemCommandOutputBuffer.h"
#include <vector>
#include<string>
class SystemCommandOutputBuffer : public ISystemCommandOutputBuffer
{
public:
	explicit SystemCommandOutputBuffer() = default;
	void BufferCommandOutput() override;
	void CreateTempCommandOutputFile(const std::string systemcommand, const std::string filename) override;
	void DeleteTempCommandOutputFile() override;
private:
	std::vector<std::string> Buffer;
	std::string BufferedFilename;
};

