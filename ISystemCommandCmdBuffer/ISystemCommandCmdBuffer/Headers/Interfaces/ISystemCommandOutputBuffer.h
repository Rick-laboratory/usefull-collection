#pragma once
#include <string>

class ISystemCommandOutputBuffer
{
  public:
    virtual ~ISystemCommandOutputBuffer() = default;
    virtual void BufferCommandOutput() = 0;
    // command is trusted shell code; filename is an ordinary path and is never inserted into it.
    virtual void CreateTempCommandOutputFile(std::string command, std::string filename) = 0;
    virtual void DeleteTempCommandOutputFile() = 0;
};
