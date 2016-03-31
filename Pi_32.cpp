#include "stdafx.h"

word32 mu[4] = { 0xf0e8e4e2, 0xe1d8d4d2, 0xd1cccac9, 0xc6c5c3b8 };
word32 nu[4] = { 0xb4b2b1ac, 0xaaa9a6a5, 0xa39c9a99, 0x9695938e };
word32 C1[4] = { 0x8d8b8778, 0x7472716c, 0x6a696665, 0x635c5a59 };
word32 C2[4] = { 0x5655534e, 0x4d4b473c, 0x3a393635, 0x332e2d2b };
word32 C3[4] = { 0x271e1d1b, 0x170ff0e8, 0xe4e2e1d8, 0xd4d2d1cc };
word32 C4[4] = { 0xcac9c6c5, 0xc3b8b4b2, 0xb1acaaa9, 0xa6a5a39c };
word32 C5[4] = { 0x9a999695, 0x938e8d8b, 0x87787472, 0x716c6a69 };
word32 C6[4] = { 0x6665635c, 0x5a595655, 0x534e4d4b, 0x473c3a39 };

/*  �̱任  X[4],T[12]  */
void muTransformation_32(word32 *X, word32 *T)
{
	T[0] = ROTL32(5, 32, (X[0] + X[1] + X[2] + mu[0]) & mod_32);
	T[1] = ROTL32(11, 32, (X[0] + X[1] + X[3] + mu[1]) & mod_32);
	T[2] = ROTL32(17, 32, (X[0] + X[2] + X[3] + mu[2]) & mod_32);
	T[3] = ROTL32(23, 32, (X[1] + X[2] + X[3] + mu[3]) & mod_32);
	T[4] = T[0] ^ T[1] ^ T[3];
	T[5] = T[0] ^ T[1] ^ T[2];
	T[6] = T[1] ^ T[2] ^ T[3];
	T[7] = T[0] ^ T[2] ^ T[3];
}


/*  �ͱ任  Y[4],T[12]  */
void nuTransformation_32(word32 *Y, word32 *T)
{
	T[0] = ROTL32(3, 32, (Y[0] + Y[2] + Y[3] + nu[0]) & mod_32);
	T[1] = ROTL32(10, 32, (Y[1] + Y[2] + Y[3] + nu[1]) & mod_32);
	T[2] = ROTL32(19, 32, (Y[0] + Y[1] + Y[2] + nu[2]) & mod_32);
	T[3] = ROTL32(29, 32, (Y[0] + Y[1] + Y[3] + nu[3]) & mod_32);
	T[8] = T[1] ^ T[2] ^ T[3];
	T[9] = T[0] ^ T[2] ^ T[3];
	T[10] = T[0] ^ T[1] ^ T[3];
	T[11] = T[0] ^ T[1] ^ T[2];
}


/*  �ұ任  Z[4],T[12]*/
void sigmaTransformation_32(word32 *Z, word32 *T)
{
	Z[3] = (T[4] + T[8]) & mod_32;
	Z[0] = (T[5] + T[9]) & mod_32;
	Z[1] = (T[6] + T[10]) & mod_32;
	Z[2] = (T[7] + T[11]) & mod_32;
}


//     * operation
//  ����X[4],Y[4]     ���Z[4]
void arxOperation_32(word32 *X, word32 *Y, word32 *Z)
{
	word32 T[12] = { 0 };
	muTransformation_32(X, T);
	nuTransformation_32(Y, T);
	sigmaTransformation_32(Z, T);
}


/*  E1����  */
void e1Function_32(word32 *C, word32 *I, word32 *output)
{
	word32 J[4], Ii[4];
	/* J1 = C * I1  */
	for (int j = 0; j < 4; j++)
		Ii[j] = I[j];
	arxOperation_32(C, Ii, J);
	for (int i = 0; i < 4; i++)
		output[i] = J[i];
	/*  Ji = Ji-1 * Ii  */
	for (int i = 1; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			Ii[j] = I[4 * i + j];
		arxOperation_32(J, Ii, J);
		for (int k = 0; k < 4; k++)
			output[4 * i + k] = J[k];
	}
}


/*  E2����  */
void e2Function_32(word32 *C, word32 *I, word32 *output)
{
	word32 In[4], Jn[4];
	/* Jn = In * C  */
	for (int i = 0; i < 4; i++)
		In[i] = I[12 + i];
	arxOperation_32(In, C, Jn);
	for (int i = 0; i < 4; i++)
		output[12 + i] = Jn[i];
	/* Jn-i = In-i * Jn-i+1  */
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++)
			In[j] = I[4 * (2 - i) + j];
		arxOperation_32(In, Jn, Jn);
		for (int k = 0; k < 4; k++)
			output[4 * (2 - i) + k] = Jn[k];
	}
}


/*  �к���  */
void piFunction_32(word32 *IS, word32 *output)
{
	word32 temp[16];
	e1Function_32(C1, IS, temp);
	e2Function_32(C2, temp, temp);
	e1Function_32(C3, temp, temp);
	e2Function_32(C4, temp, temp);
	e1Function_32(C5, temp, temp);
	e2Function_32(C6, temp, temp);
	for (int i = 0; i < 16; i++)
		output[i] = temp[i];
}


/***********    ��ʼ��    ***********
K:128 bits	PMN:128 bits		CIS:512 bits		ctr:64 bits

CIS[16]=(I1,I2,I3,I4)
=((IS[0],IS[1],IS[2],IS[3]), (IS[4],IS[5],IS[6],IS[7]), (IS[8],IS[9],IS[10],IS[11]) ,
(IS[12],IS[13],IS[14],IS[15]))
***********             ************/
word64 initialization_32(word32 *K, word32 *PMN, word32 *CIS)
{
	//���׶� IS = K || PMN || 10*
	word32 padding[16];
	for (int i = 0; i < 4; i++)
		padding[i] = K[i];
	for (int i = 0; i < 4; i++)
		padding[4 + i] = PMN[i];
	padding[8] = 0x01000000;
	for (int i = 9; i < 16; i++)
		padding[i] = 0x00000000;
	//����CIS
	piFunction_32(padding, padding);
	for (int i = 0; i < 16; i++)
		CIS[i] = padding[i];
	//��ʼ��������ctr
	word64 ctr = 0;
	for (int i = 0; i < 2; i++)
		ctr = (ctr << 32) | padding[4 + i];
	return ctr;
}


/***********    ����AD����    ***********
CISΪͨ���ڲ�״̬��ADiΪAD��һ�����飬ctrΪ��������iΪ��ǰAD����ĵ���ţ�tiΪ�����ժҪ
CIS��word32[16],ADi��word32[8],  ti:word32[8]
*****************************************/
void processADBlock_32(word32 *CIS, word32 *ADi, word64 ctr, int i, word32 *ti)
{
	word32 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//������ (ctr+i) XOR ISrate
	word64 CTR = ctr + i;
	for (int i = 1; i <= 2; i++)
		ISrate[8 - i] ^= ((0x00000000ffffffff << ((i - 1) * 32)) ^ CTR) >> ((i - 1) * 32);
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//��һ�Φк���
	word32 output[16];
	piFunction_32(IS, output);
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
	piFunction_32(IS, output);
	//ADi�����ժҪ
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ti[4 * i + j] = output[8 * i + j];
	}
}


/***********    ����SMN(����)    ***********
CISΪͨ���ڲ�״̬��SMNΪ������Ϣ��ţ�ctrΪ��������ADBlockNumΪAD���������
C0Ϊ���ĵ�һ�����飬T_1ΪAD����֤ժҪ��T_2Ϊ������SMN����֤ժҪ, newCISΪ���µ�ͨ���ڲ�״̬
CIS��word32[16], SMN��word32[8],  C0:word32[8]�� T_1:word32[8], T_2:word32[8]�� newCIS:word32[16]
*******************************************/
void processSMN4Encryption_32(word32 *CIS, word32 *SMN, word64 ctr, int ADBlockNum, word32 *C0, word32 *T_1, word32 *T_2, word32 *newCIS)
{
	word32 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//������ (ctr+a+1) XOR ISrate
	word64 CTR = ctr + ADBlockNum + 1;
	for (int i = 1; i <= 2; i++)
		ISrate[8 - i] ^= ((0x00000000ffffffff << ((i - 1) * 32)) ^ CTR) >> ((i - 1) * 32);
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//��һ�Φк���
	word32 output[16];
	piFunction_32(IS, output);
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
	piFunction_32(IS, output);
	//����T_2
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ISrate[4 * i + j] = output[8 * i + j];
	}
	for (int i = 0; i < 8; i++)
		T_2[i] = (T_1[i] + ISrate[i]) & mod_32;
	//����CIS
	for (int i = 0; i < 16; i++)
		newCIS[i] = output[i];
}


/***********    ����Message����    ***********
CISΪͨ���ڲ�״̬��MiΪMessage��һ�����飬ctrΪ��������ADBlockNumΪAD�ķ�������
mΪ��ǰMessage����ĵ���ţ�CiΪ��ǰMi������ܺ�����ģ�tiΪ�����ժҪ
CIS��word32[16], Mi��word32[8],  ti:word32[8], Ci:word32[16]
*****************************************/
void processMessageBlock_32(word32 *CIS, word64 ctr, int ADBlockNum, int m, word32 *Ci, word32 *Mi, word32 *ti)
{
	word32 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//������ (ctr+a+1+m) XOR ISrate
	word64 CTR = ctr + ADBlockNum + 1 + m;
	for (int i = 1; i <= 2; i++)
		ISrate[8 - i] ^= ((0x00000000ffffffff << ((i - 1) * 32)) ^ CTR) >> ((i - 1) * 32);
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//��һ�Φк���
	word32 output[16];
	piFunction_32(IS, output);

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
	piFunction_32(IS, output);
	//Mi�ķ���ժҪti[]
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ti[4 * i + j] = output[8 * i + j];
	}
}


/***********    ����SMN(����)    ***********
CISΪͨ���ڲ�״̬��SMNΪ������Ϣ��ţ�ctrΪ��������ADBlockNumΪAD���������
C0Ϊ���ĵ�һ�����飬T_1ΪAD����֤ժҪ��T_2Ϊ������SMN����֤ժҪ, newCISΪ���µ�ͨ���ڲ�״̬
CIS��word32[16], SMN��word32[8],  C0:word32[8]�� T_1:word32[8], T_2:word32[8]�� newCIS:word32[16]
*******************************************/
void processSMN4Decryption_32(word32 *CIS, word32 *C0, word64 ctr, int ADBlockNum, word32 *SMN, word32 *T_1, word32 *T_2, word32 *newCIS)
{
	word32 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//������ (ctr+a+1) XOR ISrate
	word64 CTR = ctr + ADBlockNum + 1;
	for (int i = 1; i <= 2; i++)
		ISrate[8 - i] ^= ((0x00000000ffffffff << ((i - 1) * 32)) ^ CTR) >> ((i - 1) * 32);
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//��һ�Φк���
	word32 output[16];
	piFunction_32(IS, output);
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
	piFunction_32(IS, output);
	//����T_2
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ISrate[4 * i + j] = output[8 * i + j];
	}
	for (int i = 0; i < 8; i++)
		T_2[i] = (T_1[i] + ISrate[i]) & mod_32;
	//����CIS
	for (int i = 0; i < 16; i++)
		newCIS[i] = output[i];
}


/***********    ����Cipher����    ***********
CISΪͨ���ڲ�״̬��CiΪCipher��һ�����飬ctrΪ��������ADBlockNumΪAD�ķ�������
mΪ��ǰCipher����ĵ���ţ�MiΪ��ǰCi������ܺ�����ģ�tiΪ�����ժҪ
CIS��word32[16], Mi��word32[8],  ti:word32[8], Ci:word32[16]
*****************************************/
void processCipherBlock_32(word32 *CIS, word64 ctr, int ADBlockNum, int m, word32 *Ci, word32 *Mi, word32 *ti)
{
	word32 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//������ (ctr+a+1+m) XOR ISrate
	word64 CTR = ctr + ADBlockNum + 1 + m;
	for (int i = 1; i <= 2; i++)
		ISrate[8 - i] ^= ((0x00000000ffffffff << ((i - 1) * 32)) ^ CTR) >> ((i - 1) * 32);
	//�µ��ڲ�״̬IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//��һ�Φк���
	word32 output[16];
	piFunction_32(IS, output);
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
	piFunction_32(IS, output);
	//Mi�ķ���ժҪti[]
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ti[4 * i + j] = output[8 * i + j];
	}
}