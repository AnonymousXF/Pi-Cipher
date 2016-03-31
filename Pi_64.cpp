#include "stdafx.h"

word64 mu[4] = { 0xF0E8E4E2E1D8D4D2, 0xD1CCCAC9C6C5C3B8, 0xB4B2B1ACAAA9A6A5, 0xA39C9A999695938E };
word64 nu[4] = { 0x8D8B87787472716C, 0x6A696665635C5A59, 0x5655534E4D4B473C, 0x3A393635332E2D2B };
word64 C1[4] = { 0x271E1D1B170FF0E8, 0xE4E2E1D8D4D2D1CC, 0xCAC9C6C5C3B8B4B2, 0xB1ACAAA9A6A5A39C };
word64 C2[4] = { 0x9A999695938E8D8B, 0x87787472716C6A69, 0x6665635C5A595655, 0x534E4D4B473C3A39 };
word64 C3[4] = { 0x3635332E2D2B271E, 0x1D1B170FF0E8E4E2, 0xE1D8D4D2D1CCCAC9, 0xC6C5C3B8B4B2B1AC };
word64 C4[4] = { 0xAAA9A6A5A39C9A99, 0x9695938E8D8B8778, 0x7472716C6A696665, 0x635C5A595655534E };
word64 C5[4] = { 0x4D4B473C3A393635, 0x332E2D2B271E1D1B, 0x170FF0E8E4E2E1D8, 0xD4D2D1CCCAC9C6C5 };
word64 C6[4] = { 0xC3B8B4B2B1ACAAA9, 0xA6A5A39C9A999695, 0x938E8D8B87787472, 0x716C6A696665635C };



/*  �̱任  X[4],T[12]  */
void muTransformation_64(word64 *X, word64 *T)
{
	T[0] = ROTL64(7, 64, (X[0] + X[1] + X[2] + mu[0]) & mod);
	T[1] = ROTL64(19, 64, (X[0] + X[1] + X[3] + mu[1]) & mod);
	T[2] = ROTL64(31, 64, (X[0] + X[2] + X[3] + mu[2]) & mod);
	T[3] = ROTL64(53, 64, (X[1] + X[2] + X[3] + mu[3]) & mod);
	T[4] = T[0] ^ T[1] ^ T[3];
	T[5] = T[0] ^ T[1] ^ T[2];
	T[6] = T[1] ^ T[2] ^ T[3];
	T[7] = T[0] ^ T[2] ^ T[3];
}

/*  �ͱ任  Y[4],T[12]  */
void nuTransformation_64(word64 *Y, word64 *T)
{
	T[0] = ROTL64(11, 64, (Y[0] + Y[2] + Y[3] + nu[0]) & mod);
	T[1] = ROTL64(23, 64, (Y[1] + Y[2] + Y[3] + nu[1]) & mod);
	T[2] = ROTL64(37, 64, (Y[0] + Y[1] + Y[2] + nu[2]) & mod);
	T[3] = ROTL64(59, 64, (Y[0] + Y[1] + Y[3] + nu[3]) & mod);
	T[8] = T[1] ^ T[2] ^ T[3];
	T[9] = T[0] ^ T[2] ^ T[3];
	T[10] = T[0] ^ T[1] ^ T[3];
	T[11] = T[0] ^ T[1] ^ T[2];
}

/*  �ұ任  Z[4],T[12]*/
void sigmaTransformation_64(word64 *Z, word64 *T)
{
	Z[3] = (T[4] + T[8]) & mod;
	Z[0] = (T[5] + T[9]) & mod;
	Z[1] = (T[6] + T[10]) & mod;
	Z[2] = (T[7] + T[11]) & mod;
}

//     * operation
// ����X[4],Y[4]     ���Z[4]
void arxOperation_64(word64 *X, word64 *Y, word64 *Z)
{
	word64 T[12] = { 0 };
	muTransformation_64(X, T);
	nuTransformation_64(Y, T);
	sigmaTransformation_64(Z, T);
}

/*  E1����  */
void e1Function_64(word64 *C, word64 *I, word64 *output)
{
	word64 J[4], Ii[4];
	/* J1 = C * I1  */
	for (int j = 0; j < 4; j++)
		Ii[j] = I[j];
	arxOperation_64(C, Ii, J);
	for (int i = 0; i < 4; i++)
		output[i] = J[i];
	/*  Ji = Ji-1 * Ii  */
	for (int i = 1; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			Ii[j] = I[4 * i + j];
		arxOperation_64(J, Ii, J);
		for (int k = 0; k < 4; k++)
			output[4 * i + k] = J[k];
	}
}

/*  E2����  */
void e2Function_64(word64 *C, word64 *I, word64 *output)
{
	word64 In[4], Jn[4];
	/* Jn = In * C  */
	for (int i = 0; i < 4; i++)
		In[i] = I[12 + i];
	arxOperation_64(In, C, Jn);
	for (int i = 0; i < 4; i++)
		output[12 + i] = Jn[i];
	/* Jn-i = In-i * Jn-i+1  */
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++)
			In[j] = I[4 * (2 - i) + j];
		arxOperation_64(In, Jn, Jn);
		for (int k = 0; k < 4; k++)
			output[4 * (2 - i) + k] = Jn[k];
	}
}

/*  �к���  */
void piFunction_64(word64 *IS, word64 *output)
{
	word64 temp[16];
	e1Function_64(C1, IS, temp);
	e2Function_64(C2, temp, temp);
	e1Function_64(C3, temp, temp);
	e2Function_64(C4, temp, temp);
	e1Function_64(C5, temp, temp);
	e2Function_64(C6, temp, temp);
	for (int i = 0; i < 16; i++)
		output[i] = temp[i];
}


/***********    ��ʼ��    ***********
K:128 bits	PMN:128 bits		CIS:1024 bits		ctr:64 bits
K:word64[2], PMN:word64[2], CIS:word64[16]
CIS[16]=(I1,I2,I3,I4)
=((IS[0],IS[1],IS[2],IS[3]), (IS[4],IS[5],IS[6],IS[7]), (IS[8],IS[9],IS[10],IS[11]) ,
(IS[12],IS[13],IS[14],IS[15]))
***********             ************/
word64 initialization_64_128(word64 *K, word64 *PMN, word64 *CIS)
{
	//���׶� IS = K || PMN || 10*
	word64 padding[16];
	for (int i = 0; i < 2; i++)
		padding[i] = K[i];
	for (int i = 0; i < 2; i++)
		padding[2 + i] = PMN[i];
	padding[4] = 0x0100000000000000;
	for (int i = 5; i < 16; i++)
		padding[i] = 0x0000000000000000;
	//����CIS
	piFunction_64(padding, padding);
	for (int i = 0; i < 16; i++)
		CIS[i] = padding[i];
	//��ʼ��������ctr
	word64 ctr = 0;
	ctr = padding[4];
	return ctr;
}

/***********    ��ʼ��    ***********
K:256 bits	PMN:128 bits		CIS:1024 bits		ctr:64 bits
K:word64[2], PMN:word64[2], CIS:word64[16]
CIS[16]=(I1,I2,I3,I4)
=((IS[0],IS[1],IS[2],IS[3]), (IS[4],IS[5],IS[6],IS[7]), (IS[8],IS[9],IS[10],IS[11]) ,
(IS[12],IS[13],IS[14],IS[15]))
***********             ************/
word64 initialization_64_256(word64 *K, word64 *PMN, word64 *CIS)
{
	//���׶� IS = K || PMN || 10*
	word64 padding[16];
	for (int i = 0; i < 4; i++)
		padding[i] = K[i];
	for (int i = 0; i < 2; i++)
		padding[4 + i] = PMN[i];
	padding[6] = 0x0100000000000000;
	for (int i = 7; i < 16; i++)
		padding[i] = 0x0000000000000000;
	//����CIS
	piFunction_64(padding, padding);
	for (int i = 0; i < 16; i++)
		CIS[i] = padding[i];
	//��ʼ��������ctr
	word64 ctr = 0;
	ctr = padding[4];
	return ctr;
}

/***********    ����AD����    ***********
CISΪͨ���ڲ�״̬��ADiΪAD��һ�����飬ctrΪ��������iΪ��ǰAD����ĵ���ţ�tiΪ�����ժҪ
CIS��word32[16],ADi��word32[8],  ti:word32[8]
*****************************************/
void processADBlock_64(word64 *CIS, word64 *ADi, word64 ctr, int i, word64 *ti)
{
	word64 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//������ (ctr+i) XOR ISrate
	word64 CTR = ctr + i;
	ISrate[7] ^= CTR;
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//��һ�Φк���
	word64 output[16];
	piFunction_64(IS, output);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = output[8 * i + j];
			IScapacity[4 * i + j] = output[8 * i + j + 4];
		}
	}
	//����ADi
	for (int i = 0; i < 8; i++)
		ISrate[i] ^= ADi[i];
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//�ڶ��Φк���
	piFunction_64(IS, output);
	//ADi�����ժҪ
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ti[4 * i + j] = output[8 * i + j];
	}
}

/***********    ����SMN(����)    ***********
CISΪͨ���ڲ�״̬��SMNΪ������Ϣ��ţ�ctrΪ��������ADBlockNumΪAD���������
C0Ϊ���ĵ�һ�����飬T_1ΪAD����֤ժҪ��T_2Ϊ������SMN����֤ժҪ, newCISΪ���µ�ͨ���ڲ�״̬
CIS��word64[16], SMN��word64[8],  C0:word64[8]�� T_1:word64[8], T_2:word64[8]�� newCIS:word64[16]
*******************************************/
void processSMN4Encryption_64(word64 *CIS, word64 *SMN, word64 ctr, int ADBlockNum, word64 *C0, word64 *T_1, word64 *T_2, word64 *newCIS)
{
	word64 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//������ (ctr+a+1) XOR ISrate
	word64 CTR = ctr + ADBlockNum + 1;
	ISrate[7] ^= CTR;
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//��һ�Φк���
	word64 output[16];
	piFunction_64(IS, output);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = output[8 * i + j];
			IScapacity[4 * i + j] = output[8 * i + j + 4];
		}
	}
	//���C0
	for (int i = 0; i < 8; i++) {
		C0[i] = ISrate[i] ^ SMN[i];
		ISrate[i] ^= SMN[i];
	}
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//�ڶ��Φк���
	piFunction_64(IS, output);
	//����T_2
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ISrate[4 * i + j] = output[8 * i + j];
	}
	for (int i = 0; i < 8; i++)
		T_2[i] = (T_1[i] + ISrate[i]) & mod;
	//����CIS
	for (int i = 0; i < 16; i++)
		newCIS[i] = output[i];
}

/***********    ����Message����    ***********
CISΪͨ���ڲ�״̬��MiΪMessage��һ�����飬ctrΪ��������ADBlockNumΪAD�ķ�������
mΪ��ǰMessage����ĵ���ţ�CiΪ��ǰMi������ܺ�����ģ�tiΪ�����ժҪ
CIS��word64[16], Mi��word64[8],  ti:word64[8], Ci:word64[16]
*****************************************/
void processMessageBlock_64(word64 *CIS, word64 ctr, int ADBlockNum, int m, word64 *Ci, word64 *Mi, word64 *ti)
{
	word64 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//������ (ctr+a+1+m) XOR ISrate
	word64 CTR = ctr + ADBlockNum + 1 + m;
	ISrate[7] ^= CTR;
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//��һ�Φк���
	word64 output[16];
	piFunction_64(IS, output);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = output[8 * i + j];
			IScapacity[4 * i + j] = output[8 * i + j + 4];
		}
	}
	//��������Ci[]
	for (int i = 0; i < 8; i++) {
		Ci[i] = ISrate[i] ^ Mi[i];
		ISrate[i] ^= Mi[i];
	}
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//�ڶ��Φк���
	piFunction_64(IS, output);
	//Mi�ķ���ժҪti[]
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ti[4 * i + j] = output[8 * i + j];
	}
}

/***********    ����SMN(����)    ***********
CISΪͨ���ڲ�״̬��SMNΪ������Ϣ��ţ�ctrΪ��������ADBlockNumΪAD���������
C0Ϊ���ĵ�һ�����飬T_1ΪAD����֤ժҪ��T_2Ϊ������SMN����֤ժҪ, newCISΪ���µ�ͨ���ڲ�״̬
CIS��word64[16], SMN��word64[8],  C0:word64[8]�� T_1:word64[8], T_2:word64[8]�� newCIS:word64[16]
*******************************************/
void processSMN4Decryption_64(word64 *CIS, word64 *C0, word64 ctr, int ADBlockNum, word64 *SMN, word64 *T_1, word64 *T_2, word64 *newCIS)
{
	word64 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//������ (ctr+a+1) XOR ISrate
	word64 CTR = ctr + ADBlockNum + 1;
	ISrate[7] ^= CTR;
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//��һ�Φк���
	word64 output[16];
	piFunction_64(IS, output);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = output[8 * i + j];
			IScapacity[4 * i + j] = output[8 * i + j + 4];
		}
	}
	//���SMN
	for (int i = 0; i < 8; i++) {
		SMN[i] = ISrate[i] ^ C0[i];
		ISrate[i] = C0[i];
	}
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//�ڶ��Φк���
	piFunction_64(IS, output);
	//����T_2
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ISrate[4 * i + j] = output[8 * i + j];
	}
	for (int i = 0; i < 8; i++)
		T_2[i] = (T_1[i] + ISrate[i]) & mod;
	//����CIS
	for (int i = 0; i < 16; i++)
		newCIS[i] = output[i];
}

/***********    ����Cipher����    ***********
CISΪͨ���ڲ�״̬��CiΪCipher��һ�����飬ctrΪ��������ADBlockNumΪAD�ķ�������
mΪ��ǰCipher����ĵ���ţ�MiΪ��ǰCi������ܺ�����ģ�tiΪ�����ժҪ
CIS��word64[16], Mi��word64[8],  ti:word64[8], Ci:word64[16]
*****************************************/
void processCipherBlock_64(word64 *CIS, word64 ctr, int ADBlockNum, int m, word64 *Ci, word64 *Mi, word64 *ti)
{
	word64 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//������ (ctr+a+1+m) XOR ISrate
	word64 CTR = ctr + ADBlockNum + 1 + m;
	ISrate[7] ^= CTR;
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//��һ�Φк���
	word64 output[16];
	piFunction_64(IS, output);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = output[8 * i + j];
			IScapacity[4 * i + j] = output[8 * i + j + 4];
		}
	}
	//��������Mi[]
	for (int i = 0; i < 8; i++) {
		Mi[i] = ISrate[i] ^ Ci[i];
		ISrate[i] = Ci[i];
	}
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//�ڶ��Φк���
	piFunction_64(IS, output);
	//Mi�ķ���ժҪti[]
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ti[4 * i + j] = output[8 * i + j];
	}
}