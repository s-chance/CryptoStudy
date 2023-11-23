#include "utils.h"

// 将64位的输入分为两个32位的部分
void split(uint64_t input, uint32_t &left, uint32_t &right) {
    // 将64位的输入分为两个32位的部分
    left = (input >> 32) & 0xFFFFFFFF;
    right = input & 0xFFFFFFFF;
}

uint64_t join(uint32_t left, uint32_t right) {
    // 将两个32位的部分合并为一个64位的输出
    return ((uint64_t)left << 32) | right;
}

uint64_t permutation(uint64_t input, const int *table, int size, int inpput_size) {
    // 置换函数
    uint64_t output = 0;
    for (int i = 0; i < size; ++i) {
        // 根据置换表进行置换
        if (input & (1ULL << (inpput_size  - table[i]))) {
            output |= 1ULL << (size - 1 - i);
        }
    } 
    return output;
}

// 代换函数
uint32_t substitution(uint32_t input, const int box[4][16]) {
    int row = ((input & 0x20) >> 4) | (input & 0x1);
    int col = (input & 0x1E) >> 1;
    return box[row][col];
}
