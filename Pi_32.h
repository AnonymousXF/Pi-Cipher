#pragma once
#include "defs.h"

void muTransformation_32(word32 *X, word32 *T);
void nuTransformation_32(word32 *Y, word32 *T);
void sigmaTransformation_32(word32 *Z, word32 *T);
void arxOperation_32(word32 *X, word32 *Y, word32 *Z);
void e1Function_32(word32 *C, word32 *I, word32 *output);
void e2Function_32(word32 *C, word32 *I, word32 *output);
void piFunction_32(word32 *IS, word32 *output);

word64 initialization_32(word32 *K, word32 *PMN, word32 *CIS);
void processADBlock_32(word32 *CIS, word32 *ADi, word64 ctr, int i, word32 *ti);
void processSMN4Encryption_32(word32 *CIS, word32 *SMN, word64 ctr, int ADBlockNum, word32 *C0, word32 *T_1, word32 *T_2, word32 *newCIS);
void processMessageBlock_32(word32 *CIS, word64 ctr, int ADBlockNum, int m, word32 *Ci, word32 *Mi, word32 *ti);
void processSMN4Decryption_32(word32 *CIS, word32 *C0, word64 ctr, int ADBlockNum, word32 *SMN, word32 *T_1, word32 *T_2, word32 *newCIS);
void processCipherBlock_32(word32 *CIS, word64 ctr, int ADBlockNum, int m, word32 *Ci, word32 *Mi, word32 *ti);