#include <iostream>
#include <fstream>

int main()
{
    std::ofstream file;
    file.open("cout.txt");
    std::streambuf* sbuf = std::cout.rdbuf();
    std::cout.rdbuf(file.rdbuf());
    //cout is now pointing to a file
    std::cout << "test" << std::endl;
    system("pause");
    return 0;
}