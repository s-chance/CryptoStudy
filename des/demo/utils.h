#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

// 将64位的输入分为两个32位的部分
void split(uint64_t input, uint32_t &left, uint32_t &right);

// 将两个32位的部分合并为一个64位的输出
uint64_t join(uint32_t left, uint32_t right);

// 置换函数
uint64_t permutation(uint64_t input, const int *table, int size, int inpput_size);

// 代换函数
uint32_t substitution(uint32_t input, const int box[4][16]);

#endif // UTILS_H
