//
//  main.cpp
//  step3
//
//  Created by ljpc on 2018/10/16.
//  Copyright © 2018年 ljpc. All rights reserved.
//
#include "des.cpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

void print_result(char *word, char *text, bool *bits) {
  if (strcmp(word, "密文") != 0) {
    printf("%s：%s\n", word, text);
  }
  printf("%s：", word);
  for (int i = 0; i < 8; i++) {
    int val = 0;
    for (int j = 0; j < 8; j++) {
      val = (val << 1) | bits[i * 8 + j];
    }
    printf("0x%x ", val);
  }
  printf("\n");
  printf("%s：", word);
  for (int i = 0; i < 64; i++) {
    printf("%d", bits[i]);
  }
  printf("\n");
}

int main() {

  char ClearWord[] = "明文";
  char ClearText[10];   // 8字节字符明文
  bool ClearText_B[64]; // 64位明文

  char CipherWord[] = "密文";
  char CipherText[10];   // 8字节字符密文
  bool CipherText_B[64]; // 64为密文

  char ClearWord2[] = "解密";
  char ClearText2[10];   // 8字节字符解密
  bool ClearText2_B[64]; // 64位解密

  char KeyWord[] = "密钥";
  char KEY_C[10];      // 8字节字符密钥
  bool KEY_B[64];      // 64位密钥
  bool SUBKEY[16][48]; // 存放16轮48位子密钥

  scanf("%s", ClearText);
  scanf("%s", KEY_C);

  ByteToBit(ClearText_B, ClearText, 8);
  print_result(ClearWord, ClearText, ClearText_B);

  SETKEY(KEY_C, KEY_B);
  Set_SubKey(SUBKEY, KEY_B);
  print_result(KeyWord, KEY_C, KEY_B);

  DES(CipherText, ClearText, SUBKEY, true);

  // ByteToBit(CipherText_B, CipherText, 8);
  // print_result(CipherWord, CipherText, CipherText_B);

  DES(ClearText2, CipherText, SUBKEY, false);
  ByteToBit(ClearText2_B, ClearText2, 8);
  print_result(ClearWord2, ClearText2, ClearText2_B);

  return 0;
}
