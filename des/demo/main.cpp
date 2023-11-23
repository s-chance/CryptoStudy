#include "des.h"
#include <cstdint>
#include <iostream>
#include <string>

// 将字符串转换为uint64_t
uint64_t string_to_uint64(const std::string &str) {
  uint64_t result = 0;
  for (char c : str) {
    result = (result << 8) | static_cast<unsigned char>(c);
  }
  return result;
}

// 将uint64_t转换为字符串
std::string uint64_to_string(uint64_t value) {
  std::string result;
  for (int i = 0; i < 8; ++i) {
    result.push_back(static_cast<char>(value >> (56 - i * 8)));
  }
  return result;
}

int main() {

  /**
  一个参考的标准测试数据，用于验证DES算法的正确性
  Plaintext: 0123456789ABCDEF
  Key: 133457799BBCDFF1
  Ciphertext: 85E813540F0AB405
  */

  // 创建一个DES对象
  uint64_t key = 0x0123456789ABCDEF;
  // uint64_t key = string_to_uint64("2018helo");
  DES des(key);

  // 加密一个明文块
  uint64_t plaintext = 0x0123456789ABCDEF;
  // uint64_t plaintext = string_to_uint64("romantic");
  uint64_t ciphertext = des.encrypt(plaintext);
  std::cout << "密文: " << std::hex << ciphertext << std::endl;
  // std::cout << "Ciphertext: " << uint64_to_string(ciphertext) << std::endl;

  // 解密一个密文块
  uint64_t decrypted = des.decrypt(ciphertext);
  std::cout << "解密后明文: " << std::hex << decrypted << std::endl;
  // std::cout << "Decrypted: " << uint64_to_string(decrypted) << std::endl;

  return 0;
}
