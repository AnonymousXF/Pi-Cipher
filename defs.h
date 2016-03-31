#pragma once
typedef unsigned char word8;
typedef unsigned short word16;
typedef unsigned int word32;
typedef unsigned _int64 word64;

const unsigned _int64 mod = 0xffffffffffffffff;
const unsigned int mod_32 = 0xffffffff;
const unsigned short mod_16 = 0xffff;

#define ENCRYPT_SUCCEED 1
#define DECRYPT_SUCCEED 1
#define FILE_OPEN_FAILED 2
#define MSG_FILE_OPEN_FAILED 3
#define CIPHER_FILE_OPEN_FAILED 4
#define AD_LENGTH_NOT_ENOUGH 5
#define AUTHEN_FAILED 6