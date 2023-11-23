#ifndef DES_H
#define DES_H

#include <cstdint>
#include <array>



class DES {
public:
    DES(uint64_t key);
    uint64_t encrypt(uint64_t plaintext);
    uint64_t decrypt(uint64_t ciphertext);

private:
    uint64_t initial_permutation(uint64_t input);
    uint64_t inverse_initial_permutation(uint64_t input);
    uint64_t f_function(uint64_t input, uint64_t subkey);
    std::array<uint64_t, 16> key_schedule(uint64_t key);

private:
    std::array<uint64_t, 16> subkeys_;
};

#endif // DES_H
