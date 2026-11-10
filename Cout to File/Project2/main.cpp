#include <fstream>
#include <iostream>
#include <string_view>

class RedirectOutput final
{
  public:
    explicit RedirectOutput(std::streambuf *buffer) : original_(std::cout.rdbuf(buffer))
    {
    }
    ~RedirectOutput()
    {
        std::cout.rdbuf(original_);
    }
    RedirectOutput(const RedirectOutput &) = delete;
    RedirectOutput &operator=(const RedirectOutput &) = delete;

  private:
    std::streambuf *original_;
};
int main(int argc, char *argv[])
{
    if (argc != 2 || std::string_view(argv[1]) == "--help")
    {
        std::cout << "cout-to-file OUTPUT\nWrites a demonstration line to OUTPUT (overwrites the file).\n";
        return argc > 2 ? 2 : 0;
    }
    std::ofstream file(argv[1]);
    if (!file)
    {
        std::cerr << "Cannot open output file.\n";
        return 1;
    }
    {
        RedirectOutput redirect(file.rdbuf());
        std::cout << "Redirected through std::cout\n" << std::flush;
    }
    file.close();
    if (!file)
    {
        std::cerr << "Output write failed.\n";
        return 1;
    }
    std::cout << "Console output restored.\n";
    return 0;
}
