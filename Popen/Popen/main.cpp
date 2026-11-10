#include <cstdio>

int main()
{
    FILE *pipe = _popen("echo Pipe capture demo", "rt");
    if (!pipe)
    {
        std::perror("_popen");
        return 1;
    }
    char buffer[256]{};
    while (std::fgets(buffer, sizeof(buffer), pipe))
        std::fputs(buffer, stdout); // Captured bytes are data, never a printf format string.
    const bool readFailed = std::ferror(pipe) != 0;
    const int status = _pclose(pipe);
    if (readFailed || status != 0)
    {
        std::fprintf(stderr, "Command/read failed (status %d).\n", status);
        return 1;
    }
    return 0;
}
