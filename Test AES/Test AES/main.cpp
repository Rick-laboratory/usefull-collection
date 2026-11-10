#include "AES.h"
#include <array>
#include <cstring>
#include <iostream>
#include <memory>

int main()
{
    AES aes(256);
    std::array<unsigned char, 16> plain{'H', 'E', 'L', 'L', 'O'};
    std::array<unsigned char, 32> key{};
    for (std::size_t i = 0; i < key.size(); ++i)
        key[i] = static_cast<unsigned char>(i);
    unsigned length = 0;
    const std::unique_ptr<unsigned char[]> encrypted(
        aes.EncryptECB(plain.data(), static_cast<unsigned>(plain.size()), key.data(), length));
    if (length != plain.size())
        return 1;
    const std::unique_ptr<unsigned char[]> decrypted(aes.DecryptECB(encrypted.get(), length, key.data()));
    if (std::memcmp(plain.data(), decrypted.get(), plain.size()) != 0)
        return 1;
    std::cout << "One complete AES-256 block roundtrip passed (educational ECB example).\n";
    return 0;
}
