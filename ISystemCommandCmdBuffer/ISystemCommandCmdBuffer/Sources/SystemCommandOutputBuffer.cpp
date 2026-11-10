#include "../Headers/SystemCommandOutputBuffer.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

void SystemCommandOutputBuffer::BufferCommandOutput()
{
    std::ifstream input(BufferedFilename);
    if (!input)
        throw std::runtime_error("Cannot read captured output.");
    Buffer.clear();
    std::string line;
    while (std::getline(input, line))
        Buffer.push_back(line);
    if (input.bad())
        throw std::runtime_error("Captured output read failed.");
    for (const auto &entry : Buffer)
        std::cout << entry << '\n';
}
void SystemCommandOutputBuffer::CreateTempCommandOutputFile(const std::string command,
                                                            const std::string filename)
{
    if (filename.empty() || std::filesystem::exists(filename))
        throw std::runtime_error("Choose an output file that does not already exist.");
    std::ofstream output(filename, std::ios::binary);
    if (!output)
        throw std::runtime_error("Cannot create output file.");
    FILE *pipe = _popen((command + " 2>&1").c_str(), "rt");
    if (!pipe)
        throw std::runtime_error("Cannot open command pipe.");
    char block[4096];
    std::size_t count;
    while ((count = std::fread(block, 1, sizeof(block), pipe)) != 0)
        output.write(block, static_cast<std::streamsize>(count));
    const bool readFailed = std::ferror(pipe) != 0;
    const int status = _pclose(pipe);
    output.close();
    if (readFailed || status != 0 || !output)
        throw std::runtime_error("Command or output capture failed.");
    BufferedFilename = filename;
}
void SystemCommandOutputBuffer::DeleteTempCommandOutputFile()
{
    if (!BufferedFilename.empty() && std::remove(BufferedFilename.c_str()) != 0)
        throw std::runtime_error("Cannot remove captured output file.");
    BufferedFilename.clear();
    Buffer.clear();
}
