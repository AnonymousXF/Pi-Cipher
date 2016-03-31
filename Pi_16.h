#pragma once
#include "defs.h"

void muTransformation_16(word16 *X, word16 *T);
void nuTransformation_16(word16 *Y, word16 *T);
void sigmaTransformation_16(word16 *Z, word16 *T);
void arxOperation_16(word16 *X, word16 *Y, word16 *Z);
void e1Function_16(word16 *C, word16 *I, word16 *output);
void e2Function_16(word16 *C, word16 *I, word16 *output);
void piFunction_16(word16 *IS, word16 *output);

word64 initialization_16(word16 *K, word16 *PMN, word16 *CIS);
void processADBlock_16(word16 *CIS, word16 *ADi, word64 ctr, int i, word16 *ti);
void processSMN4Encryption_16(word16 *CIS, word16 *SMN, word64 ctr, int ADBlockNum, word16 *C0, word16 *T_1, word16 *T_2, word16 *newCIS);
void processMessageBlock_16(word16 *CIS, word64 ctr, int ADBlockNum, int m, word16 *Ci, word16 *Mi, word16 *ti);
void processSMN4Decryption_16(word16 *CIS, word16 *C0, word64 ctr, int ADBlockNum, word16 *SMN, word16 *T_1, word16 *T_2, word16 *newCIS);
void processCipherBlock_16(word16 *CIS, word64 ctr, int ADBlockNum, int m, word16 *Ci, word16 *Mi, word16 *ti);