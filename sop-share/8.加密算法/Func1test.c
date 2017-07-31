/*
 ============================================================================
 Name        : Func1test.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : function 1 test program
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Function1.h"

int main(void)
{
	/******************************test sample 1******************************/
	//input1
	UCHAR T = 0;
	UCHAR P[5] =
	{ 0 };
	UCHAR K1[20] =
	{ 0 };
	UCHAR K2[20] =
	{ 0 };
	Function1(T, P, K1, K2);
	printf(
			"K2= {%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x}\n",
			K2[0], K2[1], K2[2], K2[3], K2[4], K2[5], K2[6], K2[7], K2[8],
			K2[9], K2[10], K2[11], K2[12], K2[13], K2[14], K2[15], K2[16],
			K2[17], K2[18], K2[19]);
	//output1 K2= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

	/******************************test sample 2******************************/
	//input2
	T = 1;
	P[0] = 1;
	P[1] = 3;
	P[2] = 4;
	P[3] = 3;
	P[4] = 2;
	K1[0] = 0x43;
	K1[1] = 0xe9;
	K1[2] = 0xa8;
	K1[3] = 0x27;
	K1[4] = 0x2c;
	K1[5] = 0x5b;
	K1[6] = 0x16;
	K1[7] = 0xe2;
	K1[8] = 0x9a;
	K1[9] = 0x01;
	K1[10] = 0x29;
	K1[11] = 0x0a;
	K1[12] = 0x77;
	K1[13] = 0xc6;
	K1[14] = 0xb3;
	K1[15] = 0x10;
	K1[16] = 0xe0;
	K1[17] = 0x71;
	K1[18] = 0x39;
	K1[19] = 0xdd;
	Function1(T, P, K1, K2);
	printf(
			"K2= {%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x}\n",
			K2[0], K2[1], K2[2], K2[3], K2[4], K2[5], K2[6], K2[7], K2[8],
			K2[9], K2[10], K2[11], K2[12], K2[13], K2[14], K2[15], K2[16],
			K2[17], K2[18], K2[19]);
	//	output2 K2={10,21,10,21,30,63,40,84,30,63,20,42,b,b2,a2,fa,84,30,de,60}

	/******************************test sample 3******************************/
	//input3
	T = 2;
	P[0] = 2;
	P[1] = 7;
	P[2] = 2;
	P[3] = 4;
	P[4] = 5;
	K1[0] = 0xd2;
	K1[1] = 0x3a;
	K1[2] = 0x86;
	K1[3] = 0x3c;
	K1[4] = 0x67;
	K1[5] = 0x0b;
	K1[6] = 0x95;
	K1[7] = 0x1e;
	K1[8] = 0x3a;
	K1[9] = 0x26;
	K1[10] = 0x66;
	K1[11] = 0xe1;
	K1[12] = 0x3b;
	K1[13] = 0x86;
	K1[14] = 0xc2;
	K1[15] = 0x35;
	K1[16] = 0xea;
	K1[17] = 0x30;
	K1[18] = 0x65;
	K1[19] = 0x74;
	Function1(T, P, K1, K2);
	printf(
			"K2= {%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x}\n",
			K2[0], K2[1], K2[2], K2[3], K2[4], K2[5], K2[6], K2[7], K2[8],
			K2[9], K2[10], K2[11], K2[12], K2[13], K2[14], K2[15], K2[16],
			K2[17], K2[18], K2[19]);
	//output3 K2= {20,42,20,42,70,e7,20,42,40,84,50,a5,1,ee,ec,ef,3c,a7,ad,cc}

	/******************************test sample 4******************************/
	//input4
	T = 3;
	P[0] = 3;
	P[1] = 1;
	P[2] = 3;
	P[3] = 7;
	P[4] = 2;
	K1[0] = 0x6d;
	K1[1] = 0x26;
	K1[2] = 0xc3;
	K1[3] = 0x98;
	K1[4] = 0xa7;
	K1[5] = 0xcc;
	K1[6] = 0x04;
	K1[7] = 0xd2;
	K1[8] = 0x42;
	K1[9] = 0x56;
	K1[10] = 0xd3;
	K1[11] = 0x21;
	K1[12] = 0x5c;
	K1[13] = 0x53;
	K1[14] = 0xdc;
	K1[15] = 0x39;
	K1[16] = 0x06;
	K1[17] = 0xa7;
	K1[18] = 0x80;
	K1[19] = 0x25;
	Function1(T, P, K1, K2);
	printf(
			"K2= {%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x}\n",
			K2[0], K2[1], K2[2], K2[3], K2[4], K2[5], K2[6], K2[7], K2[8],
			K2[9], K2[10], K2[11], K2[12], K2[13], K2[14], K2[15], K2[16],
			K2[17], K2[18], K2[19]);
	//output4 K2= {30,63,30,63,10,21,30,63,70,e7,20,42,15,84,cf,36,1b,ad,ca,a7}
	return 0;
}
