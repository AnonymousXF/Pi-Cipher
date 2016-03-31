#include "stdafx.h"

word16 mu[4] = { 0xf0e8, 0xe4e2, 0xe1d8, 0xd4d2 };
word16 nu[4] = { 0xd1cc, 0xcac9, 0xc6c5, 0xc3b8 };
word16 C1[4] = { 0xb4b2, 0xb1ac, 0xaaa9, 0xa6a5 };
word16 C2[4] = { 0xa39c, 0x9a99, 0x9695, 0x938e };
word16 C3[4] = { 0x8d8b, 0x8778, 0x7472, 0x716c };
word16 C4[4] = { 0x6a69, 0x6665, 0x635c, 0x5a59 };
word16 C5[4] = { 0x5655, 0x534a, 0x4d4b, 0x473c };
word16 C6[4] = { 0x3a39, 0x3635, 0x332e, 0x2d2b };

/*  μ变换  X[4],T[12]  */
void muTransformation_16(word16 *X, word16 *T)
{
	T[0] = ROTL16(1, 16, (X[0] + X[1] + X[2] + mu[0]) & mod_16);
	T[1] = ROTL16(4, 16, (X[0] + X[1] + X[3] + mu[1]) & mod_16);
	T[2] = ROTL16(9, 16, (X[0] + X[2] + X[3] + mu[2]) & mod_16);
	T[3] = ROTL16(11, 16, (X[1] + X[2] + X[3] + mu[3]) & mod_16);
	T[4] = T[0] ^ T[1] ^ T[3];
	T[5] = T[0] ^ T[1] ^ T[2];
	T[6] = T[1] ^ T[2] ^ T[3];
	T[7] = T[0] ^ T[2] ^ T[3];
}

/*  ν变换  Y[4],T[12]  */
void nuTransformation_16(word16 *Y, word16 *T)
{
	T[0] = ROTL16(2, 16, (Y[0] + Y[2] + Y[3] + nu[0]) & mod_16);
	T[1] = ROTL16(5, 16, (Y[1] + Y[2] + Y[3] + nu[1]) & mod_16);
	T[2] = ROTL16(7, 16, (Y[0] + Y[1] + Y[2] + nu[2]) & mod_16);
	T[3] = ROTL16(13, 16, (Y[0] + Y[1] + Y[3] + nu[3]) & mod_16);
	T[8] = T[1] ^ T[2] ^ T[3];
	T[9] = T[0] ^ T[2] ^ T[3];
	T[10] = T[0] ^ T[1] ^ T[3];
	T[11] = T[0] ^ T[1] ^ T[2];
}

/*  σ变换  Z[4],T[12]*/
void sigmaTransformation_16(word16 *Z, word16 *T)
{
	Z[3] = (T[4] + T[8]) & mod_16;
	Z[0] = (T[5] + T[9]) & mod_16;
	Z[1] = (T[6] + T[10]) & mod_16;
	Z[2] = (T[7] + T[11]) & mod_16;
}

//     * operation
// 	输入X[4],Y[4]     输出Z[4]
void arxOperation_16(word16 *X, word16 *Y, word16 *Z)
{
	word16 T[12] = { 0 };
	muTransformation_16(X, T);
	nuTransformation_16(Y, T);
	sigmaTransformation_16(Z, T);
}

/*  E1函数  */
void e1Function_16(word16 *C, word16 *I, word16 *output)
{
	word16 J[4], Ii[4];
	/* J1 = C * I1  */
	for (int j = 0; j < 4; j++)
		Ii[j] = I[j];
	arxOperation_16(C, Ii, J);
	for (int i = 0; i < 4; i++)
		output[i] = J[i];
	/*  Ji = Ji-1 * Ii  */
	for (int i = 1; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			Ii[j] = I[4 * i + j];
		arxOperation_16(J, Ii, J);
		for (int k = 0; k < 4; k++)
			output[4 * i + k] = J[k];
	}
}

/*  E2函数  */
void e2Function_16(word16 *C, word16 *I, word16 *output)
{
	word16 In[4], Jn[4];
	/* Jn = In * C  */
	for (int i = 0; i < 4; i++)
		In[i] = I[12 + i];
	arxOperation_16(In, C, Jn);
	for (int i = 0; i < 4; i++)
		output[12 + i] = Jn[i];
	/* Jn-i = In-i * Jn-i+1  */
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++)
			In[j] = I[4 * (2 - i) + j];
		arxOperation_16(In, Jn, Jn);
		for (int k = 0; k < 4; k++)
			output[4 * (2 - i) + k] = Jn[k];
	}
}

/*  π函数  */
void piFunction_16(word16 *IS, word16 *output)
{
	word16 temp[16];
	e1Function_16(C1, IS, temp);
	e2Function_16(C2, temp, temp);
	e1Function_16(C3, temp, temp);
	e2Function_16(C4, temp, temp);
	e1Function_16(C5, temp, temp);
	e2Function_16(C6, temp, temp);
	for (int i = 0; i < 16; i++)
		output[i] = temp[i];
}

/***********    初始化    ***********
K:96 bits	PMN:32 bits		CIS:256 bits		ctr:64 bits

CIS[16]=(I1,I2,I3,I4)
=((IS[0],IS[1],IS[2],IS[3]), (IS[4],IS[5],IS[6],IS[7]), (IS[8],IS[9],IS[10],IS[11]) ,
(IS[12],IS[13],IS[14],IS[15]))
***********             ************/
word64 initialization_16(word16 *K, word16 *PMN, word16 *CIS)
{
	//填充阶段 IS = K || PMN || 10*
	word16 padding[16];
	for (int i = 0; i < 6; i++)
		padding[i] = K[i];
	for (int i = 0; i < 2; i++)
		padding[6 + i] = PMN[i];
	padding[8] = 0x0100;
	for (int i = 9; i < 16; i++)
		padding[i] = 0x0000;
	//生成CIS
	piFunction_16(padding, padding);
	for (int i = 0; i < 16; i++)
		CIS[i] = padding[i];
	//初始化计数器ctr
	word64 ctr = 0;
	for (int i = 0; i < 4; i++)
		ctr = (ctr << 16) | padding[4 + i];
	return ctr;
}

/***********    处理AD分组    ***********
CIS为通用内部状态，ADi为AD的一个分组，ctr为计数器，i为当前AD分组的的组号，ti为分组的摘要
CIS：word16[16],ADi：word16[8],  ti:word16[8]
*****************************************/
void processADBlock_16(word16 *CIS, word16 *ADi, word64 ctr, int i, word16 *ti)
{
	word16 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//计数器 (ctr+i) XOR ISrate
	word64 CTR = ctr + i;
	for (int i = 1; i <= 4; i++)
		ISrate[8 - i] ^= ((0x000000000000ffff << ((i - 1) * 16)) ^ CTR) >> ((i - 1) * 16);
	//新的内部状态IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//第一次π函数
	word16 output[16];
	piFunction_16(IS, output);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = output[8 * i + j];
			IScapacity[4 * i + j] = output[8 * i + j + 4];
		}
	}
	//处理ADi
	for (int i = 0; i < 8; i++)
		ISrate[i] ^= ADi[i];
	//新的内部状态IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//第二次π函数
	piFunction_16(IS, output);
	//ADi分组的摘要
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ti[4 * i + j] = output[8 * i + j];
	}
}

/***********    处理SMN(加密)    ***********
CIS为通用内部状态，SMN为秘密消息编号，ctr为计数器，ADBlockNum为AD分组的组数
C0为密文第一个分组，T_1为AD的认证摘要，T_2为处理完SMN的认证摘要, newCIS为更新的通用内部状态
CIS：word16[16], SMN：word16[8],  C0:word16[8]， T_1:word16[8], T_2:word16[8]， newCIS:word16[16]
*******************************************/
void processSMN4Encryption_16(word16 *CIS, word16 *SMN, word64 ctr, int ADBlockNum, word16 *C0, word16 *T_1, word16 *T_2, word16 *newCIS)
{
	word16 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//计数器 (ctr+a+1) XOR ISrate
	word64 CTR = ctr + ADBlockNum + 1;
	for (int i = 1; i <= 4; i++)
		ISrate[8 - i] ^= ((0x000000000000ffff << ((i - 1) * 16)) ^ CTR) >> ((i - 1) * 16);
	//新的内部状态IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//第一次π函数
	word16 output[16];
	piFunction_16(IS, output);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = output[8 * i + j];
			IScapacity[4 * i + j] = output[8 * i + j + 4];
		}
	}
	//输出C0
	for (int i = 0; i < 8; i++) {
		C0[i] = ISrate[i] ^ SMN[i];
		ISrate[i] ^= SMN[i];
	}
	//新的内部状态IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//第二次π函数
	piFunction_16(IS, output);
	//计算T_2
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ISrate[4 * i + j] = output[8 * i + j];
	}
	for (int i = 0; i < 8; i++)
		T_2[i] = (T_1[i] + ISrate[i]) & mod_16;
	//更新CIS
	for (int i = 0; i < 16; i++)
		newCIS[i] = output[i];
}

/***********    处理Message分组    ***********
CIS为通用内部状态，Mi为Message的一个分组，ctr为计数器，ADBlockNum为AD的分组数，
m为当前Message分组的的组号，Ci为当前Mi分组加密后的密文，ti为分组的摘要
CIS：word16[16], Mi：word16[8],  ti:word16[8], Ci:word16[16]
*****************************************/
void processMessageBlock_16(word16 *CIS, word64 ctr, int ADBlockNum, int m, word16 *Ci, word16 *Mi, word16 *ti)
{
	word16 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//计数器 (ctr+a+1+m) XOR ISrate
	word64 CTR = ctr + ADBlockNum + 1 + m;
	for (int i = 1; i <= 4; i++)
		ISrate[8 - i] ^= ((0x000000000000ffff << ((i - 1) * 16)) ^ CTR) >> ((i - 1) * 16);
	//新的内部状态IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//第一次π函数
	word16 output[16];
	piFunction_16(IS, output);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = output[8 * i + j];
			IScapacity[4 * i + j] = output[8 * i + j + 4];
		}
	}
	//计算密文Ci[]
	for (int i = 0; i < 8; i++) {
		Ci[i] = ISrate[i] ^ Mi[i];
		ISrate[i] ^= Mi[i];
	}
	//新的内部状态IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//第二次π函数
	piFunction_16(IS, output);
	//Mi的分组摘要ti[]
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ti[4 * i + j] = output[8 * i + j];
	}
}

/***********    处理SMN(解密)    ***********
CIS为通用内部状态，SMN为秘密消息编号，ctr为计数器，ADBlockNum为AD分组的组数
C0为密文第一个分组，T_1为AD的认证摘要，T_2为处理完SMN的认证摘要, newCIS为更新的通用内部状态
CIS：word16[16], SMN：word16[8],  C0:word16[8]， T_1:word16[8], T_2:word16[8]， newCIS:word16[16]
*******************************************/
void processSMN4Decryption_16(word16 *CIS, word16 *C0, word64 ctr, int ADBlockNum, word16 *SMN, word16 *T_1, word16 *T_2, word16 *newCIS)
{
	word16 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//计数器 (ctr+a+1) XOR ISrate
	word64 CTR = ctr + ADBlockNum + 1;
	for (int i = 1; i <= 4; i++)
		ISrate[8 - i] ^= ((0x000000000000ffff << ((i - 1) * 16)) ^ CTR) >> ((i - 1) * 16);
	//新的内部状态IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//第一次π函数
	word16 output[16];
	piFunction_16(IS, output);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = output[8 * i + j];
			IScapacity[4 * i + j] = output[8 * i + j + 4];
		}
	}
	//输出SMN
	for (int i = 0; i < 8; i++) {
		SMN[i] = ISrate[i] ^ C0[i];
		ISrate[i] = C0[i];
	}
	//新的内部状态IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//第二次π函数
	piFunction_16(IS, output);
	//计算T_2
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ISrate[4 * i + j] = output[8 * i + j];
	}
	for (int i = 0; i < 8; i++)
		T_2[i] = (T_1[i] + ISrate[i]) & mod_16;
	//更新CIS
	for (int i = 0; i < 16; i++)
		newCIS[i] = output[i];
}

/***********    处理Cipher分组    ***********
CIS为通用内部状态，Ci为Cipher的一个分组，ctr为计数器，ADBlockNum为AD的分组数，
m为当前Cipher分组的的组号，Mi为当前Ci分组解密后的明文，ti为分组的摘要
CIS：word16[16], Mi：word16[8],  ti:word16[8], Ci:word16[16]
*****************************************/
void processCipherBlock_16(word16 *CIS, word64 ctr, int ADBlockNum, int m, word16 *Ci, word16 *Mi, word16 *ti)
{
	word16 ISrate[8], IScapacity[8], IS[16];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = CIS[8 * i + j];
			IScapacity[4 * i + j] = CIS[8 * i + j + 4];
		}
	}
	//计数器 (ctr+a+1+m) XOR ISrate
	word64 CTR = ctr + ADBlockNum + 1 + m;
	for (int i = 1; i <= 4; i++)
		ISrate[8 - i] ^= ((0x000000000000ffff << ((i - 1) * 16)) ^ CTR) >> ((i - 1) * 16);
	//新的内部状态IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//第一次π函数
	word16 output[16];
	piFunction_16(IS, output);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			ISrate[4 * i + j] = output[8 * i + j];
			IScapacity[4 * i + j] = output[8 * i + j + 4];
		}
	}
	//计算明文Mi[]
	for (int i = 0; i < 8; i++) {
		Mi[i] = ISrate[i] ^ Ci[i];
		ISrate[i] = Ci[i];
	}
	//新的内部状态IS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			IS[8 * i + j] = ISrate[4 * i + j];
			IS[8 * i + j + 4] = IScapacity[4 * i + j];
		}
	}
	//第二次π函数
	piFunction_16(IS, output);
	//Mi的分组摘要ti[]
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++)
			ti[4 * i + j] = output[8 * i + j];
	}
}