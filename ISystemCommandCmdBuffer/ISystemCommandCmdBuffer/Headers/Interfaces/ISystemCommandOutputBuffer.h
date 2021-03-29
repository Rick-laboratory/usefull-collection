#pragma once
#include<string>
class ISystemCommandOutputBuffer {
public:
	virtual void BufferCommandOutput() = 0;
	virtual void CreateTempCommandOutputFile(const std::string systemcommand, const std::string filename) = 0;
	virtual void DeleteTempCommandOutputFile() = 0;
};