#pragma once
#include "defs.h"

void muTransformation_64(word64 *X, word64 *T);
void nuTransformation_64(word64 *Y, word64 *T);
void sigmaTransformation_64(word64 *Z, word64 *T);
void arxOperation_64(word64 *X, word64 *Y, word64 *Z);
void e1Function_64(word64 *C, word64 *I, word64 *output);
void e2Function_64(word64 *C, word64 *I, word64 *output);
void piFunction_64(word64 *IS, word64 *output);

word64 initialization_64_128(word64 *K, word64 *PMN, word64 *CIS);
word64 initialization_64_256(word64 *K, word64 *PMN, word64 *CIS);
void processADBlock_64(word64 *CIS, word64 *ADi, word64 ctr, int i, word64 *ti);
void processSMN4Encryption_64(word64 *CIS, word64 *SMN, word64 ctr, int ADBlockNum, word64 *C0, word64 *T_1, word64 *T_2, word64 *newCIS);
void processMessageBlock_64(word64 *CIS, word64 ctr, int ADBlockNum, int m, word64 *Ci, word64 *Mi, word64 *ti);
void processSMN4Decryption_64(word64 *CIS, word64 *C0, word64 ctr, int ADBlockNum, word64 *SMN, word64 *T_1, word64 *T_2, word64 *newCIS);
void processCipherBlock_64(word64 *CIS, word64 ctr, int ADBlockNum, int m, word64 *Ci, word64 *Mi, word64 *ti);