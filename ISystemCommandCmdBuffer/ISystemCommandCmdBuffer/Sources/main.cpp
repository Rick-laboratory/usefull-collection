#include "../Headers/SystemCommandOutputBuffer.h"
#include <iostream>
#include <string_view>

int main(int argc, char *argv[])
{
    if (argc != 2 || std::string_view(argv[1]) == "--help")
    {
        std::cout << "command-buffer NEW_OUTPUT_FILE\nCaptures a fixed echo command, prints it and removes "
                     "the file.\n";
        return argc > 2 ? 2 : 0;
    }
    try
    {
        SystemCommandOutputBuffer buffer;
        buffer.CreateTempCommandOutputFile("echo System command output", argv[1]);
        buffer.BufferCommandOutput();
        buffer.DeleteTempCommandOutputFile();
        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
