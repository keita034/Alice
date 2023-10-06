#pragma once

//n番目のビットを立てたビットマスクを生成
#define ALICE_BIT(n) (1u << (n))

//マスクで表された部分のビットのみを取り出したもの
#define ALICE_BIT_MASK_AND(bit,mask)(bit & mask)

//マスクで表された部分のビットをまとめて立てる
#define ALICE_BIT_MASK_OR(bit,mask)(bit ｜= mask)

//マスクで表された部分のビットをまとめて消す
#define ALICE_BIT_MASK_AND_NOT(bit,mask)(bit &= ~mask)

