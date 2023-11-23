//
//  des.cpp
//  step3
//
//  Created by ljpc on 2018/10/17.
//  Copyright © 2018年 ljpc. All rights reserved.
//

#include "des.h"

bool flag = true;

void print_bool(char *s, const bool *out, int len) {
  printf("%s: ", s);
  for (int i = 0; i < len; i++) {
    printf("%d", out[i]);
  }
  printf("\n");
}

void SETKEY(const char Key_C[8], bool Key_B[64])
// Key_C: 2018helo
// ascii: 0x32 0x30 0x31 0x38 0x68 0x65 0x6c 0x6f
// 8bits: 00110010 00110000 00110001 00111000 01101000 01100101 01101100
// 01101111 Key_B:
// 0011001000110000001100010011100001101000011001010110110001101111
{
  // 请在这里补充代码，完成本关任务
  /********* Begin *********/
  ByteToBit(Key_B, Key_C, 8);
  /********* End *********/
}

void ByteToBit(bool *Outs, const char *In, int bits)
// In:    password
// ascii: 0x70 0x61 0x73 0x73 0x77 0x6f 0x72 0x64
// 8bits: 01110000 01100001 01110011 01110011 01110111 01101111 01110010
// 01100100 Outs:
// 0111000001100001011100110111001101110111011011110111001001100100
{
  // 请在这里补充代码，完成本关任务
  /********* Begin *********/
  for (int i = 0; i < bits; i++) {
    for (int j = 0; j < bits; j++) {
      Outs[i * bits + bits - j - 1] = (In[i] >> j) & 1;
    }
  }
  /********* End *********/
}

void BitToByte(char *Outs, const bool *In, int bits)
// In:    0111000001100001011100110111001101110111011011110111001001100100
// 8bits: 01110000 01100001 01110011 01110011 01110111 01101111 01110010
// 01100100 ascii: 0x70 0x61 0x73 0x73 0x77 0x6f 0x72 0x64 Outs:  password
{
  // 请在这里补充代码，完成本关任务
  /********* Begin *********/
  for (int i = 0; i < bits; i++) {
    int val = 0;
    for (int j = 0; j < bits; j++) {
      val = (val << 1) | In[i * bits + j];
    }
    Outs[i] = val;
  }
  Outs[bits] = '\0';
  /********* End *********/
}

void CYCLELEFT(bool *In, int len, int loop) // 循环左移函数
// before: 0000000011110000111111110000
// loop:   1
// after:  0000000111100001111111100000
{
  // 请在这里补充代码，完成本关任务
  /********* Begin *********/
  for (int i = 0; i < loop; i++) {
    bool temp = In[0];
    for (int j = 0; j < len - 1; j++) {
      In[j] = In[j + 1];
    }
    In[len - 1] = temp;
  }
  /********* End *********/
}

void Set_SubKey(bool subKey[16][48], bool Key[64]) // 设置子密钥
// Key:    0011001000110000001100010011100001101000011001010110110001101111
// SubKey: 011000000011110001100100010111000101100101000100
// SubKey: 010000001011010001110100010111001000100011100100
// SubKey: 110001001100010001110110110000001110110011011001
// SubKey: 111001101100001100100010001010111011011000011001
// SubKey: 101010101001001100100011101110110101010100100010
// SubKey: 101010010001001001011011000011000100101100100110
// SubKey: 001001010101001011011000110101000110100011010100
// SubKey: 000101100101100111010000111000011000001011011001
// SubKey: 000101100100100101010001111000111010011010011000
// SubKey: 000011110110100100010101001110010001011100001111
// SubKey: 000011110010010110001101000111100101000010100110
// SubKey: 010110110000010010101001010001000110100111100101
// SubKey: 110110011000100010101000101000101010100011011001
// SubKey: 100100001010101010001110111000111001011100010011
// SubKey: 001100000011111000000110000111110000011100101010
// SubKey: 011100000011111000000100000101000101011101100110
{
  // 请在这里补充代码，完成本关任务
  /********* Begin *********/
  bool temp[56];
  bool left[28];
  bool right[28];
  for (int i = 0; i < 56; i++) {
    temp[i] = Key[TRANS_64to56[i] - 1];
  }

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 28; j++) {
      left[j] = temp[j];
      right[j] = temp[j + 28];
    }
    CYCLELEFT(left, 28, SHIFT_TAB[i]);
    CYCLELEFT(right, 28, SHIFT_TAB[i]);

    for (int merge = 0; merge < 28; merge++) {
      temp[merge] = left[merge];
      temp[merge + 28] = right[merge];
    }
    for (int j = 0; j < 48; j++) {
      subKey[i][j] = temp[TRANS_56to48[j] - 1];
    }
  }
  /********* End *********/
}

void XOR(bool *InA, const bool *InB, int len) // 异或函数
// Before InA: 000000000001011111111110100100000000001111111000
// Before InB: 011000000011110001100100010111000101100101000100
// Before InA: 011000000010101110011010110011000101101010111100
// Before InB: 011000000011110001100100010111000101100101000100
{

  // 请在这里补充代码，完成本关任务
  /********* Begin *********/
  for (int i = 0; i < len; i++) {
    InA[i] = InA[i] ^ InB[i];
  }
  /********* End *********/
}

void S_BOXF(bool Out[32],
            const bool In[48]) // S-盒代替函数
{
  // 请在这里补充代码，完成本关任务
  /********* Begin *********/
  for (int i = 0; i < 8; i++) {
    int x = i * 6; // 48位输入分为8组，每组6位
    int y = i * 4; // 32位输出分为8组，每组4位
    int row = In[x] * 2 + In[x + 5];
    int col = In[x + 1] * 8 + In[x + 2] * 4 + In[x + 3] * 2 + In[x + 4];
    int num = S_BOX[i][row][col];
    Out[y] = (num >> 3) & 1; // &1取最后一位
    Out[y + 1] = (num >> 2) & 1;
    Out[y + 2] = (num >> 1) & 1;
    Out[y + 3] = num & 1;
  }
  /********* End *********/
}

void F_FUNCTION(bool In[32],
                const bool Ki[48]) // f 函数完成扩展置换、S-盒代替和P盒置换
{
  // 请在这里补充代码，完成本关任务
  /********* Begin *********/
  // 第一步：扩展置换，32 -> 48
  bool expand[48];
  for (int i = 0; i < 48; i++) {
    expand[i] = In[EXPAND_32to48[i] - 1];
  }

  XOR(expand, Ki, 48);
  bool output[32];
  S_BOXF(output, expand);

  for (int i = 0; i < 32; i++) {
    In[i] = output[TRANS_32to32[i] - 1];
  }
  /********* End *********/
}

#include <iostream>

void DES(char Out[8], char In[8], const bool subKey[16][48],
         bool Type) // 标准DES Type: True加密/False解密
{
  // 请在这里补充代码，完成本关任务
  /********* Begin *********/
  bool m[64];
  bool left[32];
  bool right[32];
  bool newL[32];
  bool c[64];
  bool currB[64];
  ByteToBit(m, In, 8);
  for (int i = 0; i < 64; i++) {
    currB[i] = m[TRANS_INIT[i] - 1];
  }

  for (int i = 0; i < 32; i++) {
    left[i] = currB[i];
    right[i] = currB[i + 32];
  }
  if (Type) {
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 32; j++) {
        newL[j] = right[j];
      }
      F_FUNCTION(right, subKey[i]);
      XOR(right, left, 32);
      for (int j = 0; j < 32; j++) {
        left[j] = newL[j];
      }
    }
  } else {
    for (int i = 15; i >= 0; i--) {
      for (int j = 0; j < 32; j++) {
        newL[j] = right[j];
      }

      F_FUNCTION(right, subKey[i]);
      XOR(right, left, 32);
      for (int j = 0; j < 32; j++) {
        left[j] = newL[j];
      }
    }
  }

  for (int i = 0; i < 32; i++) {
    c[i] = right[i];
    c[i + 32] = left[i];
  }
  
  for (int i=0;i<64;i++) {
    currB[i] = c[i];
  }
  for (int i = 0; i < 64; i++) {
    c[i] = currB[TRANS_END[i] - 1];
  }
  BitToByte(Out, c, 8);
  /********* End *********/
}
