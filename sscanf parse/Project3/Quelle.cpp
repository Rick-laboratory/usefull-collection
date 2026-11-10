#include <cstdio>

int main()
{
    const char sentence[] = "Rudolph is 12 years old";
    char name[20]{};
    int age = 0;
    if (std::sscanf(sentence, "%19s %*s %d", name, &age) != 2)
    {
        std::fputs("Could not parse the demonstration sentence.\n", stderr);
        return 1;
    }
    std::printf("%s -> %d\n", name, age);
    return 0;
}
