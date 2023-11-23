#include "des.h"
#include "utils.h"
#include <cstdint>

// 初始置换（IP）
const int IP[64] = {58, 50, 42, 34, 26, 18, 10, 2,  60, 52, 44, 36, 28,
                    20, 12, 4,  62, 54, 46, 38, 30, 22, 14, 6,  64, 56,
                    48, 40, 32, 24, 16, 8,  57, 49, 41, 33, 25, 17, 9,
                    1,  59, 51, 43, 35, 27, 19, 11, 3,  61, 53, 45, 37,
                    29, 21, 13, 5,  63, 55, 47, 39, 31, 23, 15, 7};

// 逆初始置换（IP-1）
const int IP_1[64] = {40, 8,  48, 16, 56, 24, 64, 32, 39, 7,  47, 15, 55,
                      23, 63, 31, 38, 6,  46, 14, 54, 22, 62, 30, 37, 5,
                      45, 13, 53, 21, 61, 29, 36, 4,  44, 12, 52, 20, 60,
                      28, 35, 3,  43, 11, 51, 19, 59, 27, 34, 2,  42, 10,
                      50, 18, 58, 26, 33, 1,  41, 9,  49, 17, 57, 25};

// 扩展置换（E）
const int E[48] = {32, 1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
                   8,  9,  10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
                   16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
                   24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};

// S盒
const int S[8][4][16] = {
    // S1
    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
     {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
     {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
     {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    // S2
    {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
     {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
     {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
     {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    // S3
    {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
     {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
     {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
     {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    // S4
    {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
     {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
     {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
     {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    // S5
    {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
     {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
     {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
     {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    // S6
    {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
     {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
     {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
     {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    // S7
    {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
     {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
     {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
     {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    // S8
    {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
     {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
     {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

// P盒置换（P）
const int P[32] = {16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23,
                   26, 5, 18, 31, 10, 2,  8,  24, 14, 32, 27,
                   3,  9, 19, 13, 30, 6,  22, 11, 4,  25};

// PC-1置换
const int PC_1[56] = {57, 49, 41, 33, 25, 17, 9,  1,  58, 50, 42, 34, 26, 18,
                      10, 2,  59, 51, 43, 35, 27, 19, 11, 3,  60, 52, 44, 36,
                      63, 55, 47, 39, 31, 23, 15, 7,  62, 54, 46, 38, 30, 22,
                      14, 6,  61, 53, 45, 37, 29, 21, 13, 5,  28, 20, 12, 4};

// 左移位数
const int shifts[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// PC_2置换
const int PC_2[48] = {14, 17, 11, 24, 1,  5,  3,  28, 15, 6,  21, 10,
                      23, 19, 12, 4,  26, 8,  16, 7,  27, 20, 13, 2,
                      41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
                      44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

#include <bitset>
#include <iostream>
// 密钥调度算法，构造函数
DES::DES(uint64_t key) { subkeys_ = key_schedule(key); }

// 加密函数
uint64_t DES::encrypt(uint64_t plaintext) {

  std::cout << "明文：" << std::hex << plaintext << std::endl;

  // 初始置换
  plaintext = initial_permutation(plaintext);

  std::cout << "初始置换：" << std::hex << plaintext << std::endl;
  // 16轮迭代
  for (int i = 0; i < 16; ++i) {
    uint32_t left, right;
    // split函数将64位的输入分为两个32位的部分
    split(plaintext, left, right);
    uint64_t temp = right;
    // f_function函数实现了F函数
    right = left ^ f_function(right, subkeys_[i]);
    left = temp;
    // join函数将两个32位的部分合并为一个64位的输出
    if (i != 15) {
      plaintext = join(left, right);
    } else {
      // 16轮迭代最后一轮，左右两部分不需要交换
      plaintext = join(right, left);
    }
  }
  std::cout << "16轮迭代：" << std::hex << plaintext << std::endl;

  // 逆初始置换
  uint64_t ciphertext = inverse_initial_permutation(plaintext);
  return ciphertext;
}

// 解密函数
uint64_t DES::decrypt(uint64_t ciphertext) {
  // 解密过程与加密过程类似，只是子密钥的顺序相反

  ciphertext = initial_permutation(ciphertext);
  std::cout << "初始置换：" << std::hex << ciphertext << std::endl;

  for (int i = 15; i >= 0; --i) {
    uint32_t left, right;
    split(ciphertext, left, right);
    uint32_t temp = right;
    right = left ^ f_function(right, subkeys_[i]);
    left = temp;
    if (i != 0) {
      ciphertext = join(left, right);
    } else {
      // 16轮迭代最后一轮，左右两部分不需要交换
      ciphertext = join(right, left);
    }
  }

  std::cout << "16轮迭代：" << std::hex << ciphertext << std::endl;

  uint64_t plaintext = inverse_initial_permutation(ciphertext);
  return plaintext;
}

// 初始置换和逆初始置换
uint64_t DES::initial_permutation(uint64_t input) {
  // 初始置换
  return permutation(input, IP, 64, 64);
}

uint64_t DES::inverse_initial_permutation(uint64_t input) {
  // 逆初始置换
  return permutation(input, IP_1, 64, 64);
}

// F函数
uint64_t DES::f_function(uint64_t input, uint64_t subkey) {
  // 扩展置换
  input = permutation(input, E, 48, 32);
  // 与子密钥异或
  input ^= subkey;
  // S盒代换
  uint32_t output = 0;
  for (int i = 0; i < 8; ++i) {
    output |= substitution((input >> (48 - 6 * (i + 1))) & 0x3F, S[i])
              << (32 - 4 * (i + 1));
  }
  // P盒置换
  output = permutation(output, P, 32, 32);
  return output;
}

// 密钥调度算法
std::array<uint64_t, 16> DES::key_schedule(uint64_t key) {
  // PC-1置换
  // std::cout << std::hex << key << std::endl;
  key = permutation(key, PC_1, 56, 64);
  // std::cout << std::hex << key << std::endl;
  std::array<uint64_t, 16> subkeys;
  for (int i = 0; i < 16; ++i) {
    uint64_t left = (key >> 28) & 0x0000000FFFFFFF;
    uint64_t right = key & 0x0000000FFFFFFF;
    // 循环左移
    left =
        ((left << shifts[i]) | (left >> (28 - shifts[i]))) & 0x0000000FFFFFFF;
    right =
        ((right << shifts[i]) | (right >> (28 - shifts[i]))) & 0x0000000FFFFFFF;
    key = (left << 28) | right;

    // PC-2置换
    subkeys[i] = permutation(key, PC_2, 48, 56);
  }

  // 子密钥验证
  // std::cout << "subkeys:" << std::endl;
  // for (int i = 0; i < 16; ++i) {
  //   // 以二进制形式显示子密钥
  //   std::cout << std::bitset<48>(subkeys[i]).to_string() << std::endl;
  // }
  return subkeys;
}
