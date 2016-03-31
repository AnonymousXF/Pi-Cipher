#include "stdafx.h"

//ѭ������(16bits)
word16 ROTL16(int rho, int bit, word16 value)
{
	word16 result, temp;
	temp = value >> (bit - rho);
	value = value << rho;
	result = value | temp;
	return result;
}

//ѭ������(32bits)
word32 ROTL32(int rho, int bit, word32 value)
{
	word32 result, temp;
	temp = value >> (bit - rho);
	value = value << rho;
	result = value | temp;
	return result;
}

//ѭ������(64bits)
word64 ROTL64(int rho, int bit, word64 value)
{
	word64 result, temp;
	temp = value >> (bit - rho);
	value = value << rho;
	result = value | temp;
	return result;
}