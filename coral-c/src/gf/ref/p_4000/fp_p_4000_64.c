
// Automatically generated modular arithmetic C code
// Command line : python monty.py 64 
// 0x3efffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
// Python Script by Mike Scott (Technology Innovation Institute, UAE, 2025)

#include <stdio.h>
#include <stdint.h>

#define sspint int64_t
#define spint uint64_t
#define udpint __uint128_t
#define dpint __uint128_t

#define Wordlength 64
#define Nlimbs 69
#define Radix 60
#define Nbits 4090
#define Nbytes 512

#define MONTGOMERY
//propagate carries
static inline spint prop(spint *n) {
	int i;
	spint mask=((spint)1<<60u)-(spint)1;
	sspint carry=(sspint)n[0];
	carry>>=60u;
	n[0]&=mask;
	for (i=1;i<68;i++) {
		carry+=(sspint)n[i];
		n[i] = (spint)carry & mask;
		carry>>=60u;
	}
	n[68]+=(spint)carry;
	return -((n[68]>>1)>>62u);
}

//propagate carries and add p if negative, propagate carries again
static inline int flatten(spint *n) {
	spint carry=prop(n);
	n[0]-=(spint)1u&carry;
	n[68]+=((spint)0x3f0u)&carry;
	(void)prop(n);
	return (int)(carry&1);
}

//Montgomery final subtract
static int modfsb(spint *n) {
	n[0]+=(spint)1u;
	n[68]-=(spint)0x3f0u;
	return flatten(n);
}

//Modular addition - reduce less than 2p
static void modadd(const spint *a,const spint *b,spint *n) {
	spint carry;
	n[0]=a[0]+b[0];
	n[1]=a[1]+b[1];
	n[2]=a[2]+b[2];
	n[3]=a[3]+b[3];
	n[4]=a[4]+b[4];
	n[5]=a[5]+b[5];
	n[6]=a[6]+b[6];
	n[7]=a[7]+b[7];
	n[8]=a[8]+b[8];
	n[9]=a[9]+b[9];
	n[10]=a[10]+b[10];
	n[11]=a[11]+b[11];
	n[12]=a[12]+b[12];
	n[13]=a[13]+b[13];
	n[14]=a[14]+b[14];
	n[15]=a[15]+b[15];
	n[16]=a[16]+b[16];
	n[17]=a[17]+b[17];
	n[18]=a[18]+b[18];
	n[19]=a[19]+b[19];
	n[20]=a[20]+b[20];
	n[21]=a[21]+b[21];
	n[22]=a[22]+b[22];
	n[23]=a[23]+b[23];
	n[24]=a[24]+b[24];
	n[25]=a[25]+b[25];
	n[26]=a[26]+b[26];
	n[27]=a[27]+b[27];
	n[28]=a[28]+b[28];
	n[29]=a[29]+b[29];
	n[30]=a[30]+b[30];
	n[31]=a[31]+b[31];
	n[32]=a[32]+b[32];
	n[33]=a[33]+b[33];
	n[34]=a[34]+b[34];
	n[35]=a[35]+b[35];
	n[36]=a[36]+b[36];
	n[37]=a[37]+b[37];
	n[38]=a[38]+b[38];
	n[39]=a[39]+b[39];
	n[40]=a[40]+b[40];
	n[41]=a[41]+b[41];
	n[42]=a[42]+b[42];
	n[43]=a[43]+b[43];
	n[44]=a[44]+b[44];
	n[45]=a[45]+b[45];
	n[46]=a[46]+b[46];
	n[47]=a[47]+b[47];
	n[48]=a[48]+b[48];
	n[49]=a[49]+b[49];
	n[50]=a[50]+b[50];
	n[51]=a[51]+b[51];
	n[52]=a[52]+b[52];
	n[53]=a[53]+b[53];
	n[54]=a[54]+b[54];
	n[55]=a[55]+b[55];
	n[56]=a[56]+b[56];
	n[57]=a[57]+b[57];
	n[58]=a[58]+b[58];
	n[59]=a[59]+b[59];
	n[60]=a[60]+b[60];
	n[61]=a[61]+b[61];
	n[62]=a[62]+b[62];
	n[63]=a[63]+b[63];
	n[64]=a[64]+b[64];
	n[65]=a[65]+b[65];
	n[66]=a[66]+b[66];
	n[67]=a[67]+b[67];
	n[68]=a[68]+b[68];
	n[0]+=(spint)2u;
	n[68]-=(spint)0x7e0u;
	carry=prop(n);
	n[0]-=(spint)2u&carry;
	n[68]+=((spint)0x7e0u)&carry;
	(void)prop(n);
}

//Modular subtraction - reduce less than 2p
static void modsub(const spint *a,const spint *b,spint *n) {
	spint carry;
	n[0]=a[0]-b[0];
	n[1]=a[1]-b[1];
	n[2]=a[2]-b[2];
	n[3]=a[3]-b[3];
	n[4]=a[4]-b[4];
	n[5]=a[5]-b[5];
	n[6]=a[6]-b[6];
	n[7]=a[7]-b[7];
	n[8]=a[8]-b[8];
	n[9]=a[9]-b[9];
	n[10]=a[10]-b[10];
	n[11]=a[11]-b[11];
	n[12]=a[12]-b[12];
	n[13]=a[13]-b[13];
	n[14]=a[14]-b[14];
	n[15]=a[15]-b[15];
	n[16]=a[16]-b[16];
	n[17]=a[17]-b[17];
	n[18]=a[18]-b[18];
	n[19]=a[19]-b[19];
	n[20]=a[20]-b[20];
	n[21]=a[21]-b[21];
	n[22]=a[22]-b[22];
	n[23]=a[23]-b[23];
	n[24]=a[24]-b[24];
	n[25]=a[25]-b[25];
	n[26]=a[26]-b[26];
	n[27]=a[27]-b[27];
	n[28]=a[28]-b[28];
	n[29]=a[29]-b[29];
	n[30]=a[30]-b[30];
	n[31]=a[31]-b[31];
	n[32]=a[32]-b[32];
	n[33]=a[33]-b[33];
	n[34]=a[34]-b[34];
	n[35]=a[35]-b[35];
	n[36]=a[36]-b[36];
	n[37]=a[37]-b[37];
	n[38]=a[38]-b[38];
	n[39]=a[39]-b[39];
	n[40]=a[40]-b[40];
	n[41]=a[41]-b[41];
	n[42]=a[42]-b[42];
	n[43]=a[43]-b[43];
	n[44]=a[44]-b[44];
	n[45]=a[45]-b[45];
	n[46]=a[46]-b[46];
	n[47]=a[47]-b[47];
	n[48]=a[48]-b[48];
	n[49]=a[49]-b[49];
	n[50]=a[50]-b[50];
	n[51]=a[51]-b[51];
	n[52]=a[52]-b[52];
	n[53]=a[53]-b[53];
	n[54]=a[54]-b[54];
	n[55]=a[55]-b[55];
	n[56]=a[56]-b[56];
	n[57]=a[57]-b[57];
	n[58]=a[58]-b[58];
	n[59]=a[59]-b[59];
	n[60]=a[60]-b[60];
	n[61]=a[61]-b[61];
	n[62]=a[62]-b[62];
	n[63]=a[63]-b[63];
	n[64]=a[64]-b[64];
	n[65]=a[65]-b[65];
	n[66]=a[66]-b[66];
	n[67]=a[67]-b[67];
	n[68]=a[68]-b[68];
	carry=prop(n);
	n[0]-=(spint)2u&carry;
	n[68]+=((spint)0x7e0u)&carry;
	(void)prop(n);
}

//Modular negation
static void modneg(const spint *b,spint *n) {
	spint carry;
	n[0]=(spint)0-b[0];
	n[1]=(spint)0-b[1];
	n[2]=(spint)0-b[2];
	n[3]=(spint)0-b[3];
	n[4]=(spint)0-b[4];
	n[5]=(spint)0-b[5];
	n[6]=(spint)0-b[6];
	n[7]=(spint)0-b[7];
	n[8]=(spint)0-b[8];
	n[9]=(spint)0-b[9];
	n[10]=(spint)0-b[10];
	n[11]=(spint)0-b[11];
	n[12]=(spint)0-b[12];
	n[13]=(spint)0-b[13];
	n[14]=(spint)0-b[14];
	n[15]=(spint)0-b[15];
	n[16]=(spint)0-b[16];
	n[17]=(spint)0-b[17];
	n[18]=(spint)0-b[18];
	n[19]=(spint)0-b[19];
	n[20]=(spint)0-b[20];
	n[21]=(spint)0-b[21];
	n[22]=(spint)0-b[22];
	n[23]=(spint)0-b[23];
	n[24]=(spint)0-b[24];
	n[25]=(spint)0-b[25];
	n[26]=(spint)0-b[26];
	n[27]=(spint)0-b[27];
	n[28]=(spint)0-b[28];
	n[29]=(spint)0-b[29];
	n[30]=(spint)0-b[30];
	n[31]=(spint)0-b[31];
	n[32]=(spint)0-b[32];
	n[33]=(spint)0-b[33];
	n[34]=(spint)0-b[34];
	n[35]=(spint)0-b[35];
	n[36]=(spint)0-b[36];
	n[37]=(spint)0-b[37];
	n[38]=(spint)0-b[38];
	n[39]=(spint)0-b[39];
	n[40]=(spint)0-b[40];
	n[41]=(spint)0-b[41];
	n[42]=(spint)0-b[42];
	n[43]=(spint)0-b[43];
	n[44]=(spint)0-b[44];
	n[45]=(spint)0-b[45];
	n[46]=(spint)0-b[46];
	n[47]=(spint)0-b[47];
	n[48]=(spint)0-b[48];
	n[49]=(spint)0-b[49];
	n[50]=(spint)0-b[50];
	n[51]=(spint)0-b[51];
	n[52]=(spint)0-b[52];
	n[53]=(spint)0-b[53];
	n[54]=(spint)0-b[54];
	n[55]=(spint)0-b[55];
	n[56]=(spint)0-b[56];
	n[57]=(spint)0-b[57];
	n[58]=(spint)0-b[58];
	n[59]=(spint)0-b[59];
	n[60]=(spint)0-b[60];
	n[61]=(spint)0-b[61];
	n[62]=(spint)0-b[62];
	n[63]=(spint)0-b[63];
	n[64]=(spint)0-b[64];
	n[65]=(spint)0-b[65];
	n[66]=(spint)0-b[66];
	n[67]=(spint)0-b[67];
	n[68]=(spint)0-b[68];
	carry=prop(n);
	n[0]-=(spint)2u&carry;
	n[68]+=((spint)0x7e0u)&carry;
	(void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 91716731709159196528552301346653469781
// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a,const spint *b,spint *c) {
	dpint t=0;
	spint p68=0x3f0u;
	spint q=((spint)1<<60u); // q is unsaturated radix 
	spint mask=(spint)(q-(spint)1);
	t+=(dpint)a[0]*b[0]; spint v0=((spint)t & mask); t>>=60;
	t+=(dpint)a[0]*b[1]; t+=(dpint)a[1]*b[0]; spint v1=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[2]; t+=(dpint)a[1]*b[1]; t+=(dpint)a[2]*b[0]; spint v2=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[3]; t+=(dpint)a[1]*b[2]; t+=(dpint)a[2]*b[1]; t+=(dpint)a[3]*b[0]; spint v3=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[4]; t+=(dpint)a[1]*b[3]; t+=(dpint)a[2]*b[2]; t+=(dpint)a[3]*b[1]; t+=(dpint)a[4]*b[0]; spint v4=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[5]; t+=(dpint)a[1]*b[4]; t+=(dpint)a[2]*b[3]; t+=(dpint)a[3]*b[2]; t+=(dpint)a[4]*b[1]; t+=(dpint)a[5]*b[0]; spint v5=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[6]; t+=(dpint)a[1]*b[5]; t+=(dpint)a[2]*b[4]; t+=(dpint)a[3]*b[3]; t+=(dpint)a[4]*b[2]; t+=(dpint)a[5]*b[1]; t+=(dpint)a[6]*b[0]; spint v6=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[7]; t+=(dpint)a[1]*b[6]; t+=(dpint)a[2]*b[5]; t+=(dpint)a[3]*b[4]; t+=(dpint)a[4]*b[3]; t+=(dpint)a[5]*b[2]; t+=(dpint)a[6]*b[1]; t+=(dpint)a[7]*b[0]; spint v7=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[8]; t+=(dpint)a[1]*b[7]; t+=(dpint)a[2]*b[6]; t+=(dpint)a[3]*b[5]; t+=(dpint)a[4]*b[4]; t+=(dpint)a[5]*b[3]; t+=(dpint)a[6]*b[2]; t+=(dpint)a[7]*b[1]; t+=(dpint)a[8]*b[0]; spint v8=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[9]; t+=(dpint)a[1]*b[8]; t+=(dpint)a[2]*b[7]; t+=(dpint)a[3]*b[6]; t+=(dpint)a[4]*b[5]; t+=(dpint)a[5]*b[4]; t+=(dpint)a[6]*b[3]; t+=(dpint)a[7]*b[2]; t+=(dpint)a[8]*b[1]; t+=(dpint)a[9]*b[0]; spint v9=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[10]; t+=(dpint)a[1]*b[9]; t+=(dpint)a[2]*b[8]; t+=(dpint)a[3]*b[7]; t+=(dpint)a[4]*b[6]; t+=(dpint)a[5]*b[5]; t+=(dpint)a[6]*b[4]; t+=(dpint)a[7]*b[3]; t+=(dpint)a[8]*b[2]; t+=(dpint)a[9]*b[1]; t+=(dpint)a[10]*b[0]; spint v10=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[11]; t+=(dpint)a[1]*b[10]; t+=(dpint)a[2]*b[9]; t+=(dpint)a[3]*b[8]; t+=(dpint)a[4]*b[7]; t+=(dpint)a[5]*b[6]; t+=(dpint)a[6]*b[5]; t+=(dpint)a[7]*b[4]; t+=(dpint)a[8]*b[3]; t+=(dpint)a[9]*b[2]; t+=(dpint)a[10]*b[1]; t+=(dpint)a[11]*b[0]; spint v11=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[12]; t+=(dpint)a[1]*b[11]; t+=(dpint)a[2]*b[10]; t+=(dpint)a[3]*b[9]; t+=(dpint)a[4]*b[8]; t+=(dpint)a[5]*b[7]; t+=(dpint)a[6]*b[6]; t+=(dpint)a[7]*b[5]; t+=(dpint)a[8]*b[4]; t+=(dpint)a[9]*b[3]; t+=(dpint)a[10]*b[2]; t+=(dpint)a[11]*b[1]; t+=(dpint)a[12]*b[0]; spint v12=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[13]; t+=(dpint)a[1]*b[12]; t+=(dpint)a[2]*b[11]; t+=(dpint)a[3]*b[10]; t+=(dpint)a[4]*b[9]; t+=(dpint)a[5]*b[8]; t+=(dpint)a[6]*b[7]; t+=(dpint)a[7]*b[6]; t+=(dpint)a[8]*b[5]; t+=(dpint)a[9]*b[4]; t+=(dpint)a[10]*b[3]; t+=(dpint)a[11]*b[2]; t+=(dpint)a[12]*b[1]; t+=(dpint)a[13]*b[0]; spint v13=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[14]; t+=(dpint)a[1]*b[13]; t+=(dpint)a[2]*b[12]; t+=(dpint)a[3]*b[11]; t+=(dpint)a[4]*b[10]; t+=(dpint)a[5]*b[9]; t+=(dpint)a[6]*b[8]; t+=(dpint)a[7]*b[7]; t+=(dpint)a[8]*b[6]; t+=(dpint)a[9]*b[5]; t+=(dpint)a[10]*b[4]; t+=(dpint)a[11]*b[3]; t+=(dpint)a[12]*b[2]; t+=(dpint)a[13]*b[1]; t+=(dpint)a[14]*b[0]; spint v14=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[15]; t+=(dpint)a[1]*b[14]; t+=(dpint)a[2]*b[13]; t+=(dpint)a[3]*b[12]; t+=(dpint)a[4]*b[11]; t+=(dpint)a[5]*b[10]; t+=(dpint)a[6]*b[9]; t+=(dpint)a[7]*b[8]; t+=(dpint)a[8]*b[7]; t+=(dpint)a[9]*b[6]; t+=(dpint)a[10]*b[5]; t+=(dpint)a[11]*b[4]; t+=(dpint)a[12]*b[3]; t+=(dpint)a[13]*b[2]; t+=(dpint)a[14]*b[1]; t+=(dpint)a[15]*b[0]; spint v15=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[16]; t+=(dpint)a[1]*b[15]; t+=(dpint)a[2]*b[14]; t+=(dpint)a[3]*b[13]; t+=(dpint)a[4]*b[12]; t+=(dpint)a[5]*b[11]; t+=(dpint)a[6]*b[10]; t+=(dpint)a[7]*b[9]; t+=(dpint)a[8]*b[8]; t+=(dpint)a[9]*b[7]; t+=(dpint)a[10]*b[6]; t+=(dpint)a[11]*b[5]; t+=(dpint)a[12]*b[4]; t+=(dpint)a[13]*b[3]; t+=(dpint)a[14]*b[2]; t+=(dpint)a[15]*b[1]; t+=(dpint)a[16]*b[0]; spint v16=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[17]; t+=(dpint)a[1]*b[16]; t+=(dpint)a[2]*b[15]; t+=(dpint)a[3]*b[14]; t+=(dpint)a[4]*b[13]; t+=(dpint)a[5]*b[12]; t+=(dpint)a[6]*b[11]; t+=(dpint)a[7]*b[10]; t+=(dpint)a[8]*b[9]; t+=(dpint)a[9]*b[8]; t+=(dpint)a[10]*b[7]; t+=(dpint)a[11]*b[6]; t+=(dpint)a[12]*b[5]; t+=(dpint)a[13]*b[4]; t+=(dpint)a[14]*b[3]; t+=(dpint)a[15]*b[2]; t+=(dpint)a[16]*b[1]; t+=(dpint)a[17]*b[0]; spint v17=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[18]; t+=(dpint)a[1]*b[17]; t+=(dpint)a[2]*b[16]; t+=(dpint)a[3]*b[15]; t+=(dpint)a[4]*b[14]; t+=(dpint)a[5]*b[13]; t+=(dpint)a[6]*b[12]; t+=(dpint)a[7]*b[11]; t+=(dpint)a[8]*b[10]; t+=(dpint)a[9]*b[9]; t+=(dpint)a[10]*b[8]; t+=(dpint)a[11]*b[7]; t+=(dpint)a[12]*b[6]; t+=(dpint)a[13]*b[5]; t+=(dpint)a[14]*b[4]; t+=(dpint)a[15]*b[3]; t+=(dpint)a[16]*b[2]; t+=(dpint)a[17]*b[1]; t+=(dpint)a[18]*b[0]; spint v18=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[19]; t+=(dpint)a[1]*b[18]; t+=(dpint)a[2]*b[17]; t+=(dpint)a[3]*b[16]; t+=(dpint)a[4]*b[15]; t+=(dpint)a[5]*b[14]; t+=(dpint)a[6]*b[13]; t+=(dpint)a[7]*b[12]; t+=(dpint)a[8]*b[11]; t+=(dpint)a[9]*b[10]; t+=(dpint)a[10]*b[9]; t+=(dpint)a[11]*b[8]; t+=(dpint)a[12]*b[7]; t+=(dpint)a[13]*b[6]; t+=(dpint)a[14]*b[5]; t+=(dpint)a[15]*b[4]; t+=(dpint)a[16]*b[3]; t+=(dpint)a[17]*b[2]; t+=(dpint)a[18]*b[1]; t+=(dpint)a[19]*b[0]; spint v19=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[20]; t+=(dpint)a[1]*b[19]; t+=(dpint)a[2]*b[18]; t+=(dpint)a[3]*b[17]; t+=(dpint)a[4]*b[16]; t+=(dpint)a[5]*b[15]; t+=(dpint)a[6]*b[14]; t+=(dpint)a[7]*b[13]; t+=(dpint)a[8]*b[12]; t+=(dpint)a[9]*b[11]; t+=(dpint)a[10]*b[10]; t+=(dpint)a[11]*b[9]; t+=(dpint)a[12]*b[8]; t+=(dpint)a[13]*b[7]; t+=(dpint)a[14]*b[6]; t+=(dpint)a[15]*b[5]; t+=(dpint)a[16]*b[4]; t+=(dpint)a[17]*b[3]; t+=(dpint)a[18]*b[2]; t+=(dpint)a[19]*b[1]; t+=(dpint)a[20]*b[0]; spint v20=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[21]; t+=(dpint)a[1]*b[20]; t+=(dpint)a[2]*b[19]; t+=(dpint)a[3]*b[18]; t+=(dpint)a[4]*b[17]; t+=(dpint)a[5]*b[16]; t+=(dpint)a[6]*b[15]; t+=(dpint)a[7]*b[14]; t+=(dpint)a[8]*b[13]; t+=(dpint)a[9]*b[12]; t+=(dpint)a[10]*b[11]; t+=(dpint)a[11]*b[10]; t+=(dpint)a[12]*b[9]; t+=(dpint)a[13]*b[8]; t+=(dpint)a[14]*b[7]; t+=(dpint)a[15]*b[6]; t+=(dpint)a[16]*b[5]; t+=(dpint)a[17]*b[4]; t+=(dpint)a[18]*b[3]; t+=(dpint)a[19]*b[2]; t+=(dpint)a[20]*b[1]; t+=(dpint)a[21]*b[0]; spint v21=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[22]; t+=(dpint)a[1]*b[21]; t+=(dpint)a[2]*b[20]; t+=(dpint)a[3]*b[19]; t+=(dpint)a[4]*b[18]; t+=(dpint)a[5]*b[17]; t+=(dpint)a[6]*b[16]; t+=(dpint)a[7]*b[15]; t+=(dpint)a[8]*b[14]; t+=(dpint)a[9]*b[13]; t+=(dpint)a[10]*b[12]; t+=(dpint)a[11]*b[11]; t+=(dpint)a[12]*b[10]; t+=(dpint)a[13]*b[9]; t+=(dpint)a[14]*b[8]; t+=(dpint)a[15]*b[7]; t+=(dpint)a[16]*b[6]; t+=(dpint)a[17]*b[5]; t+=(dpint)a[18]*b[4]; t+=(dpint)a[19]*b[3]; t+=(dpint)a[20]*b[2]; t+=(dpint)a[21]*b[1]; t+=(dpint)a[22]*b[0]; spint v22=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[23]; t+=(dpint)a[1]*b[22]; t+=(dpint)a[2]*b[21]; t+=(dpint)a[3]*b[20]; t+=(dpint)a[4]*b[19]; t+=(dpint)a[5]*b[18]; t+=(dpint)a[6]*b[17]; t+=(dpint)a[7]*b[16]; t+=(dpint)a[8]*b[15]; t+=(dpint)a[9]*b[14]; t+=(dpint)a[10]*b[13]; t+=(dpint)a[11]*b[12]; t+=(dpint)a[12]*b[11]; t+=(dpint)a[13]*b[10]; t+=(dpint)a[14]*b[9]; t+=(dpint)a[15]*b[8]; t+=(dpint)a[16]*b[7]; t+=(dpint)a[17]*b[6]; t+=(dpint)a[18]*b[5]; t+=(dpint)a[19]*b[4]; t+=(dpint)a[20]*b[3]; t+=(dpint)a[21]*b[2]; t+=(dpint)a[22]*b[1]; t+=(dpint)a[23]*b[0]; spint v23=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[24]; t+=(dpint)a[1]*b[23]; t+=(dpint)a[2]*b[22]; t+=(dpint)a[3]*b[21]; t+=(dpint)a[4]*b[20]; t+=(dpint)a[5]*b[19]; t+=(dpint)a[6]*b[18]; t+=(dpint)a[7]*b[17]; t+=(dpint)a[8]*b[16]; t+=(dpint)a[9]*b[15]; t+=(dpint)a[10]*b[14]; t+=(dpint)a[11]*b[13]; t+=(dpint)a[12]*b[12]; t+=(dpint)a[13]*b[11]; t+=(dpint)a[14]*b[10]; t+=(dpint)a[15]*b[9]; t+=(dpint)a[16]*b[8]; t+=(dpint)a[17]*b[7]; t+=(dpint)a[18]*b[6]; t+=(dpint)a[19]*b[5]; t+=(dpint)a[20]*b[4]; t+=(dpint)a[21]*b[3]; t+=(dpint)a[22]*b[2]; t+=(dpint)a[23]*b[1]; t+=(dpint)a[24]*b[0]; spint v24=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[25]; t+=(dpint)a[1]*b[24]; t+=(dpint)a[2]*b[23]; t+=(dpint)a[3]*b[22]; t+=(dpint)a[4]*b[21]; t+=(dpint)a[5]*b[20]; t+=(dpint)a[6]*b[19]; t+=(dpint)a[7]*b[18]; t+=(dpint)a[8]*b[17]; t+=(dpint)a[9]*b[16]; t+=(dpint)a[10]*b[15]; t+=(dpint)a[11]*b[14]; t+=(dpint)a[12]*b[13]; t+=(dpint)a[13]*b[12]; t+=(dpint)a[14]*b[11]; t+=(dpint)a[15]*b[10]; t+=(dpint)a[16]*b[9]; t+=(dpint)a[17]*b[8]; t+=(dpint)a[18]*b[7]; t+=(dpint)a[19]*b[6]; t+=(dpint)a[20]*b[5]; t+=(dpint)a[21]*b[4]; t+=(dpint)a[22]*b[3]; t+=(dpint)a[23]*b[2]; t+=(dpint)a[24]*b[1]; t+=(dpint)a[25]*b[0]; spint v25=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[26]; t+=(dpint)a[1]*b[25]; t+=(dpint)a[2]*b[24]; t+=(dpint)a[3]*b[23]; t+=(dpint)a[4]*b[22]; t+=(dpint)a[5]*b[21]; t+=(dpint)a[6]*b[20]; t+=(dpint)a[7]*b[19]; t+=(dpint)a[8]*b[18]; t+=(dpint)a[9]*b[17]; t+=(dpint)a[10]*b[16]; t+=(dpint)a[11]*b[15]; t+=(dpint)a[12]*b[14]; t+=(dpint)a[13]*b[13]; t+=(dpint)a[14]*b[12]; t+=(dpint)a[15]*b[11]; t+=(dpint)a[16]*b[10]; t+=(dpint)a[17]*b[9]; t+=(dpint)a[18]*b[8]; t+=(dpint)a[19]*b[7]; t+=(dpint)a[20]*b[6]; t+=(dpint)a[21]*b[5]; t+=(dpint)a[22]*b[4]; t+=(dpint)a[23]*b[3]; t+=(dpint)a[24]*b[2]; t+=(dpint)a[25]*b[1]; t+=(dpint)a[26]*b[0]; spint v26=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[27]; t+=(dpint)a[1]*b[26]; t+=(dpint)a[2]*b[25]; t+=(dpint)a[3]*b[24]; t+=(dpint)a[4]*b[23]; t+=(dpint)a[5]*b[22]; t+=(dpint)a[6]*b[21]; t+=(dpint)a[7]*b[20]; t+=(dpint)a[8]*b[19]; t+=(dpint)a[9]*b[18]; t+=(dpint)a[10]*b[17]; t+=(dpint)a[11]*b[16]; t+=(dpint)a[12]*b[15]; t+=(dpint)a[13]*b[14]; t+=(dpint)a[14]*b[13]; t+=(dpint)a[15]*b[12]; t+=(dpint)a[16]*b[11]; t+=(dpint)a[17]*b[10]; t+=(dpint)a[18]*b[9]; t+=(dpint)a[19]*b[8]; t+=(dpint)a[20]*b[7]; t+=(dpint)a[21]*b[6]; t+=(dpint)a[22]*b[5]; t+=(dpint)a[23]*b[4]; t+=(dpint)a[24]*b[3]; t+=(dpint)a[25]*b[2]; t+=(dpint)a[26]*b[1]; t+=(dpint)a[27]*b[0]; spint v27=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[28]; t+=(dpint)a[1]*b[27]; t+=(dpint)a[2]*b[26]; t+=(dpint)a[3]*b[25]; t+=(dpint)a[4]*b[24]; t+=(dpint)a[5]*b[23]; t+=(dpint)a[6]*b[22]; t+=(dpint)a[7]*b[21]; t+=(dpint)a[8]*b[20]; t+=(dpint)a[9]*b[19]; t+=(dpint)a[10]*b[18]; t+=(dpint)a[11]*b[17]; t+=(dpint)a[12]*b[16]; t+=(dpint)a[13]*b[15]; t+=(dpint)a[14]*b[14]; t+=(dpint)a[15]*b[13]; t+=(dpint)a[16]*b[12]; t+=(dpint)a[17]*b[11]; t+=(dpint)a[18]*b[10]; t+=(dpint)a[19]*b[9]; t+=(dpint)a[20]*b[8]; t+=(dpint)a[21]*b[7]; t+=(dpint)a[22]*b[6]; t+=(dpint)a[23]*b[5]; t+=(dpint)a[24]*b[4]; t+=(dpint)a[25]*b[3]; t+=(dpint)a[26]*b[2]; t+=(dpint)a[27]*b[1]; t+=(dpint)a[28]*b[0]; spint v28=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[29]; t+=(dpint)a[1]*b[28]; t+=(dpint)a[2]*b[27]; t+=(dpint)a[3]*b[26]; t+=(dpint)a[4]*b[25]; t+=(dpint)a[5]*b[24]; t+=(dpint)a[6]*b[23]; t+=(dpint)a[7]*b[22]; t+=(dpint)a[8]*b[21]; t+=(dpint)a[9]*b[20]; t+=(dpint)a[10]*b[19]; t+=(dpint)a[11]*b[18]; t+=(dpint)a[12]*b[17]; t+=(dpint)a[13]*b[16]; t+=(dpint)a[14]*b[15]; t+=(dpint)a[15]*b[14]; t+=(dpint)a[16]*b[13]; t+=(dpint)a[17]*b[12]; t+=(dpint)a[18]*b[11]; t+=(dpint)a[19]*b[10]; t+=(dpint)a[20]*b[9]; t+=(dpint)a[21]*b[8]; t+=(dpint)a[22]*b[7]; t+=(dpint)a[23]*b[6]; t+=(dpint)a[24]*b[5]; t+=(dpint)a[25]*b[4]; t+=(dpint)a[26]*b[3]; t+=(dpint)a[27]*b[2]; t+=(dpint)a[28]*b[1]; t+=(dpint)a[29]*b[0]; spint v29=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[30]; t+=(dpint)a[1]*b[29]; t+=(dpint)a[2]*b[28]; t+=(dpint)a[3]*b[27]; t+=(dpint)a[4]*b[26]; t+=(dpint)a[5]*b[25]; t+=(dpint)a[6]*b[24]; t+=(dpint)a[7]*b[23]; t+=(dpint)a[8]*b[22]; t+=(dpint)a[9]*b[21]; t+=(dpint)a[10]*b[20]; t+=(dpint)a[11]*b[19]; t+=(dpint)a[12]*b[18]; t+=(dpint)a[13]*b[17]; t+=(dpint)a[14]*b[16]; t+=(dpint)a[15]*b[15]; t+=(dpint)a[16]*b[14]; t+=(dpint)a[17]*b[13]; t+=(dpint)a[18]*b[12]; t+=(dpint)a[19]*b[11]; t+=(dpint)a[20]*b[10]; t+=(dpint)a[21]*b[9]; t+=(dpint)a[22]*b[8]; t+=(dpint)a[23]*b[7]; t+=(dpint)a[24]*b[6]; t+=(dpint)a[25]*b[5]; t+=(dpint)a[26]*b[4]; t+=(dpint)a[27]*b[3]; t+=(dpint)a[28]*b[2]; t+=(dpint)a[29]*b[1]; t+=(dpint)a[30]*b[0]; spint v30=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[31]; t+=(dpint)a[1]*b[30]; t+=(dpint)a[2]*b[29]; t+=(dpint)a[3]*b[28]; t+=(dpint)a[4]*b[27]; t+=(dpint)a[5]*b[26]; t+=(dpint)a[6]*b[25]; t+=(dpint)a[7]*b[24]; t+=(dpint)a[8]*b[23]; t+=(dpint)a[9]*b[22]; t+=(dpint)a[10]*b[21]; t+=(dpint)a[11]*b[20]; t+=(dpint)a[12]*b[19]; t+=(dpint)a[13]*b[18]; t+=(dpint)a[14]*b[17]; t+=(dpint)a[15]*b[16]; t+=(dpint)a[16]*b[15]; t+=(dpint)a[17]*b[14]; t+=(dpint)a[18]*b[13]; t+=(dpint)a[19]*b[12]; t+=(dpint)a[20]*b[11]; t+=(dpint)a[21]*b[10]; t+=(dpint)a[22]*b[9]; t+=(dpint)a[23]*b[8]; t+=(dpint)a[24]*b[7]; t+=(dpint)a[25]*b[6]; t+=(dpint)a[26]*b[5]; t+=(dpint)a[27]*b[4]; t+=(dpint)a[28]*b[3]; t+=(dpint)a[29]*b[2]; t+=(dpint)a[30]*b[1]; t+=(dpint)a[31]*b[0]; spint v31=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[32]; t+=(dpint)a[1]*b[31]; t+=(dpint)a[2]*b[30]; t+=(dpint)a[3]*b[29]; t+=(dpint)a[4]*b[28]; t+=(dpint)a[5]*b[27]; t+=(dpint)a[6]*b[26]; t+=(dpint)a[7]*b[25]; t+=(dpint)a[8]*b[24]; t+=(dpint)a[9]*b[23]; t+=(dpint)a[10]*b[22]; t+=(dpint)a[11]*b[21]; t+=(dpint)a[12]*b[20]; t+=(dpint)a[13]*b[19]; t+=(dpint)a[14]*b[18]; t+=(dpint)a[15]*b[17]; t+=(dpint)a[16]*b[16]; t+=(dpint)a[17]*b[15]; t+=(dpint)a[18]*b[14]; t+=(dpint)a[19]*b[13]; t+=(dpint)a[20]*b[12]; t+=(dpint)a[21]*b[11]; t+=(dpint)a[22]*b[10]; t+=(dpint)a[23]*b[9]; t+=(dpint)a[24]*b[8]; t+=(dpint)a[25]*b[7]; t+=(dpint)a[26]*b[6]; t+=(dpint)a[27]*b[5]; t+=(dpint)a[28]*b[4]; t+=(dpint)a[29]*b[3]; t+=(dpint)a[30]*b[2]; t+=(dpint)a[31]*b[1]; t+=(dpint)a[32]*b[0]; spint v32=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[33]; t+=(dpint)a[1]*b[32]; t+=(dpint)a[2]*b[31]; t+=(dpint)a[3]*b[30]; t+=(dpint)a[4]*b[29]; t+=(dpint)a[5]*b[28]; t+=(dpint)a[6]*b[27]; t+=(dpint)a[7]*b[26]; t+=(dpint)a[8]*b[25]; t+=(dpint)a[9]*b[24]; t+=(dpint)a[10]*b[23]; t+=(dpint)a[11]*b[22]; t+=(dpint)a[12]*b[21]; t+=(dpint)a[13]*b[20]; t+=(dpint)a[14]*b[19]; t+=(dpint)a[15]*b[18]; t+=(dpint)a[16]*b[17]; t+=(dpint)a[17]*b[16]; t+=(dpint)a[18]*b[15]; t+=(dpint)a[19]*b[14]; t+=(dpint)a[20]*b[13]; t+=(dpint)a[21]*b[12]; t+=(dpint)a[22]*b[11]; t+=(dpint)a[23]*b[10]; t+=(dpint)a[24]*b[9]; t+=(dpint)a[25]*b[8]; t+=(dpint)a[26]*b[7]; t+=(dpint)a[27]*b[6]; t+=(dpint)a[28]*b[5]; t+=(dpint)a[29]*b[4]; t+=(dpint)a[30]*b[3]; t+=(dpint)a[31]*b[2]; t+=(dpint)a[32]*b[1]; t+=(dpint)a[33]*b[0]; spint v33=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[34]; t+=(dpint)a[1]*b[33]; t+=(dpint)a[2]*b[32]; t+=(dpint)a[3]*b[31]; t+=(dpint)a[4]*b[30]; t+=(dpint)a[5]*b[29]; t+=(dpint)a[6]*b[28]; t+=(dpint)a[7]*b[27]; t+=(dpint)a[8]*b[26]; t+=(dpint)a[9]*b[25]; t+=(dpint)a[10]*b[24]; t+=(dpint)a[11]*b[23]; t+=(dpint)a[12]*b[22]; t+=(dpint)a[13]*b[21]; t+=(dpint)a[14]*b[20]; t+=(dpint)a[15]*b[19]; t+=(dpint)a[16]*b[18]; t+=(dpint)a[17]*b[17]; t+=(dpint)a[18]*b[16]; t+=(dpint)a[19]*b[15]; t+=(dpint)a[20]*b[14]; t+=(dpint)a[21]*b[13]; t+=(dpint)a[22]*b[12]; t+=(dpint)a[23]*b[11]; t+=(dpint)a[24]*b[10]; t+=(dpint)a[25]*b[9]; t+=(dpint)a[26]*b[8]; t+=(dpint)a[27]*b[7]; t+=(dpint)a[28]*b[6]; t+=(dpint)a[29]*b[5]; t+=(dpint)a[30]*b[4]; t+=(dpint)a[31]*b[3]; t+=(dpint)a[32]*b[2]; t+=(dpint)a[33]*b[1]; t+=(dpint)a[34]*b[0]; spint v34=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[35]; t+=(dpint)a[1]*b[34]; t+=(dpint)a[2]*b[33]; t+=(dpint)a[3]*b[32]; t+=(dpint)a[4]*b[31]; t+=(dpint)a[5]*b[30]; t+=(dpint)a[6]*b[29]; t+=(dpint)a[7]*b[28]; t+=(dpint)a[8]*b[27]; t+=(dpint)a[9]*b[26]; t+=(dpint)a[10]*b[25]; t+=(dpint)a[11]*b[24]; t+=(dpint)a[12]*b[23]; t+=(dpint)a[13]*b[22]; t+=(dpint)a[14]*b[21]; t+=(dpint)a[15]*b[20]; t+=(dpint)a[16]*b[19]; t+=(dpint)a[17]*b[18]; t+=(dpint)a[18]*b[17]; t+=(dpint)a[19]*b[16]; t+=(dpint)a[20]*b[15]; t+=(dpint)a[21]*b[14]; t+=(dpint)a[22]*b[13]; t+=(dpint)a[23]*b[12]; t+=(dpint)a[24]*b[11]; t+=(dpint)a[25]*b[10]; t+=(dpint)a[26]*b[9]; t+=(dpint)a[27]*b[8]; t+=(dpint)a[28]*b[7]; t+=(dpint)a[29]*b[6]; t+=(dpint)a[30]*b[5]; t+=(dpint)a[31]*b[4]; t+=(dpint)a[32]*b[3]; t+=(dpint)a[33]*b[2]; t+=(dpint)a[34]*b[1]; t+=(dpint)a[35]*b[0]; spint v35=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[36]; t+=(dpint)a[1]*b[35]; t+=(dpint)a[2]*b[34]; t+=(dpint)a[3]*b[33]; t+=(dpint)a[4]*b[32]; t+=(dpint)a[5]*b[31]; t+=(dpint)a[6]*b[30]; t+=(dpint)a[7]*b[29]; t+=(dpint)a[8]*b[28]; t+=(dpint)a[9]*b[27]; t+=(dpint)a[10]*b[26]; t+=(dpint)a[11]*b[25]; t+=(dpint)a[12]*b[24]; t+=(dpint)a[13]*b[23]; t+=(dpint)a[14]*b[22]; t+=(dpint)a[15]*b[21]; t+=(dpint)a[16]*b[20]; t+=(dpint)a[17]*b[19]; t+=(dpint)a[18]*b[18]; t+=(dpint)a[19]*b[17]; t+=(dpint)a[20]*b[16]; t+=(dpint)a[21]*b[15]; t+=(dpint)a[22]*b[14]; t+=(dpint)a[23]*b[13]; t+=(dpint)a[24]*b[12]; t+=(dpint)a[25]*b[11]; t+=(dpint)a[26]*b[10]; t+=(dpint)a[27]*b[9]; t+=(dpint)a[28]*b[8]; t+=(dpint)a[29]*b[7]; t+=(dpint)a[30]*b[6]; t+=(dpint)a[31]*b[5]; t+=(dpint)a[32]*b[4]; t+=(dpint)a[33]*b[3]; t+=(dpint)a[34]*b[2]; t+=(dpint)a[35]*b[1]; t+=(dpint)a[36]*b[0]; spint v36=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[37]; t+=(dpint)a[1]*b[36]; t+=(dpint)a[2]*b[35]; t+=(dpint)a[3]*b[34]; t+=(dpint)a[4]*b[33]; t+=(dpint)a[5]*b[32]; t+=(dpint)a[6]*b[31]; t+=(dpint)a[7]*b[30]; t+=(dpint)a[8]*b[29]; t+=(dpint)a[9]*b[28]; t+=(dpint)a[10]*b[27]; t+=(dpint)a[11]*b[26]; t+=(dpint)a[12]*b[25]; t+=(dpint)a[13]*b[24]; t+=(dpint)a[14]*b[23]; t+=(dpint)a[15]*b[22]; t+=(dpint)a[16]*b[21]; t+=(dpint)a[17]*b[20]; t+=(dpint)a[18]*b[19]; t+=(dpint)a[19]*b[18]; t+=(dpint)a[20]*b[17]; t+=(dpint)a[21]*b[16]; t+=(dpint)a[22]*b[15]; t+=(dpint)a[23]*b[14]; t+=(dpint)a[24]*b[13]; t+=(dpint)a[25]*b[12]; t+=(dpint)a[26]*b[11]; t+=(dpint)a[27]*b[10]; t+=(dpint)a[28]*b[9]; t+=(dpint)a[29]*b[8]; t+=(dpint)a[30]*b[7]; t+=(dpint)a[31]*b[6]; t+=(dpint)a[32]*b[5]; t+=(dpint)a[33]*b[4]; t+=(dpint)a[34]*b[3]; t+=(dpint)a[35]*b[2]; t+=(dpint)a[36]*b[1]; t+=(dpint)a[37]*b[0]; spint v37=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[38]; t+=(dpint)a[1]*b[37]; t+=(dpint)a[2]*b[36]; t+=(dpint)a[3]*b[35]; t+=(dpint)a[4]*b[34]; t+=(dpint)a[5]*b[33]; t+=(dpint)a[6]*b[32]; t+=(dpint)a[7]*b[31]; t+=(dpint)a[8]*b[30]; t+=(dpint)a[9]*b[29]; t+=(dpint)a[10]*b[28]; t+=(dpint)a[11]*b[27]; t+=(dpint)a[12]*b[26]; t+=(dpint)a[13]*b[25]; t+=(dpint)a[14]*b[24]; t+=(dpint)a[15]*b[23]; t+=(dpint)a[16]*b[22]; t+=(dpint)a[17]*b[21]; t+=(dpint)a[18]*b[20]; t+=(dpint)a[19]*b[19]; t+=(dpint)a[20]*b[18]; t+=(dpint)a[21]*b[17]; t+=(dpint)a[22]*b[16]; t+=(dpint)a[23]*b[15]; t+=(dpint)a[24]*b[14]; t+=(dpint)a[25]*b[13]; t+=(dpint)a[26]*b[12]; t+=(dpint)a[27]*b[11]; t+=(dpint)a[28]*b[10]; t+=(dpint)a[29]*b[9]; t+=(dpint)a[30]*b[8]; t+=(dpint)a[31]*b[7]; t+=(dpint)a[32]*b[6]; t+=(dpint)a[33]*b[5]; t+=(dpint)a[34]*b[4]; t+=(dpint)a[35]*b[3]; t+=(dpint)a[36]*b[2]; t+=(dpint)a[37]*b[1]; t+=(dpint)a[38]*b[0]; spint v38=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[39]; t+=(dpint)a[1]*b[38]; t+=(dpint)a[2]*b[37]; t+=(dpint)a[3]*b[36]; t+=(dpint)a[4]*b[35]; t+=(dpint)a[5]*b[34]; t+=(dpint)a[6]*b[33]; t+=(dpint)a[7]*b[32]; t+=(dpint)a[8]*b[31]; t+=(dpint)a[9]*b[30]; t+=(dpint)a[10]*b[29]; t+=(dpint)a[11]*b[28]; t+=(dpint)a[12]*b[27]; t+=(dpint)a[13]*b[26]; t+=(dpint)a[14]*b[25]; t+=(dpint)a[15]*b[24]; t+=(dpint)a[16]*b[23]; t+=(dpint)a[17]*b[22]; t+=(dpint)a[18]*b[21]; t+=(dpint)a[19]*b[20]; t+=(dpint)a[20]*b[19]; t+=(dpint)a[21]*b[18]; t+=(dpint)a[22]*b[17]; t+=(dpint)a[23]*b[16]; t+=(dpint)a[24]*b[15]; t+=(dpint)a[25]*b[14]; t+=(dpint)a[26]*b[13]; t+=(dpint)a[27]*b[12]; t+=(dpint)a[28]*b[11]; t+=(dpint)a[29]*b[10]; t+=(dpint)a[30]*b[9]; t+=(dpint)a[31]*b[8]; t+=(dpint)a[32]*b[7]; t+=(dpint)a[33]*b[6]; t+=(dpint)a[34]*b[5]; t+=(dpint)a[35]*b[4]; t+=(dpint)a[36]*b[3]; t+=(dpint)a[37]*b[2]; t+=(dpint)a[38]*b[1]; t+=(dpint)a[39]*b[0]; spint v39=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[40]; t+=(dpint)a[1]*b[39]; t+=(dpint)a[2]*b[38]; t+=(dpint)a[3]*b[37]; t+=(dpint)a[4]*b[36]; t+=(dpint)a[5]*b[35]; t+=(dpint)a[6]*b[34]; t+=(dpint)a[7]*b[33]; t+=(dpint)a[8]*b[32]; t+=(dpint)a[9]*b[31]; t+=(dpint)a[10]*b[30]; t+=(dpint)a[11]*b[29]; t+=(dpint)a[12]*b[28]; t+=(dpint)a[13]*b[27]; t+=(dpint)a[14]*b[26]; t+=(dpint)a[15]*b[25]; t+=(dpint)a[16]*b[24]; t+=(dpint)a[17]*b[23]; t+=(dpint)a[18]*b[22]; t+=(dpint)a[19]*b[21]; t+=(dpint)a[20]*b[20]; t+=(dpint)a[21]*b[19]; t+=(dpint)a[22]*b[18]; t+=(dpint)a[23]*b[17]; t+=(dpint)a[24]*b[16]; t+=(dpint)a[25]*b[15]; t+=(dpint)a[26]*b[14]; t+=(dpint)a[27]*b[13]; t+=(dpint)a[28]*b[12]; t+=(dpint)a[29]*b[11]; t+=(dpint)a[30]*b[10]; t+=(dpint)a[31]*b[9]; t+=(dpint)a[32]*b[8]; t+=(dpint)a[33]*b[7]; t+=(dpint)a[34]*b[6]; t+=(dpint)a[35]*b[5]; t+=(dpint)a[36]*b[4]; t+=(dpint)a[37]*b[3]; t+=(dpint)a[38]*b[2]; t+=(dpint)a[39]*b[1]; t+=(dpint)a[40]*b[0]; spint v40=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[41]; t+=(dpint)a[1]*b[40]; t+=(dpint)a[2]*b[39]; t+=(dpint)a[3]*b[38]; t+=(dpint)a[4]*b[37]; t+=(dpint)a[5]*b[36]; t+=(dpint)a[6]*b[35]; t+=(dpint)a[7]*b[34]; t+=(dpint)a[8]*b[33]; t+=(dpint)a[9]*b[32]; t+=(dpint)a[10]*b[31]; t+=(dpint)a[11]*b[30]; t+=(dpint)a[12]*b[29]; t+=(dpint)a[13]*b[28]; t+=(dpint)a[14]*b[27]; t+=(dpint)a[15]*b[26]; t+=(dpint)a[16]*b[25]; t+=(dpint)a[17]*b[24]; t+=(dpint)a[18]*b[23]; t+=(dpint)a[19]*b[22]; t+=(dpint)a[20]*b[21]; t+=(dpint)a[21]*b[20]; t+=(dpint)a[22]*b[19]; t+=(dpint)a[23]*b[18]; t+=(dpint)a[24]*b[17]; t+=(dpint)a[25]*b[16]; t+=(dpint)a[26]*b[15]; t+=(dpint)a[27]*b[14]; t+=(dpint)a[28]*b[13]; t+=(dpint)a[29]*b[12]; t+=(dpint)a[30]*b[11]; t+=(dpint)a[31]*b[10]; t+=(dpint)a[32]*b[9]; t+=(dpint)a[33]*b[8]; t+=(dpint)a[34]*b[7]; t+=(dpint)a[35]*b[6]; t+=(dpint)a[36]*b[5]; t+=(dpint)a[37]*b[4]; t+=(dpint)a[38]*b[3]; t+=(dpint)a[39]*b[2]; t+=(dpint)a[40]*b[1]; t+=(dpint)a[41]*b[0]; spint v41=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[42]; t+=(dpint)a[1]*b[41]; t+=(dpint)a[2]*b[40]; t+=(dpint)a[3]*b[39]; t+=(dpint)a[4]*b[38]; t+=(dpint)a[5]*b[37]; t+=(dpint)a[6]*b[36]; t+=(dpint)a[7]*b[35]; t+=(dpint)a[8]*b[34]; t+=(dpint)a[9]*b[33]; t+=(dpint)a[10]*b[32]; t+=(dpint)a[11]*b[31]; t+=(dpint)a[12]*b[30]; t+=(dpint)a[13]*b[29]; t+=(dpint)a[14]*b[28]; t+=(dpint)a[15]*b[27]; t+=(dpint)a[16]*b[26]; t+=(dpint)a[17]*b[25]; t+=(dpint)a[18]*b[24]; t+=(dpint)a[19]*b[23]; t+=(dpint)a[20]*b[22]; t+=(dpint)a[21]*b[21]; t+=(dpint)a[22]*b[20]; t+=(dpint)a[23]*b[19]; t+=(dpint)a[24]*b[18]; t+=(dpint)a[25]*b[17]; t+=(dpint)a[26]*b[16]; t+=(dpint)a[27]*b[15]; t+=(dpint)a[28]*b[14]; t+=(dpint)a[29]*b[13]; t+=(dpint)a[30]*b[12]; t+=(dpint)a[31]*b[11]; t+=(dpint)a[32]*b[10]; t+=(dpint)a[33]*b[9]; t+=(dpint)a[34]*b[8]; t+=(dpint)a[35]*b[7]; t+=(dpint)a[36]*b[6]; t+=(dpint)a[37]*b[5]; t+=(dpint)a[38]*b[4]; t+=(dpint)a[39]*b[3]; t+=(dpint)a[40]*b[2]; t+=(dpint)a[41]*b[1]; t+=(dpint)a[42]*b[0]; spint v42=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[43]; t+=(dpint)a[1]*b[42]; t+=(dpint)a[2]*b[41]; t+=(dpint)a[3]*b[40]; t+=(dpint)a[4]*b[39]; t+=(dpint)a[5]*b[38]; t+=(dpint)a[6]*b[37]; t+=(dpint)a[7]*b[36]; t+=(dpint)a[8]*b[35]; t+=(dpint)a[9]*b[34]; t+=(dpint)a[10]*b[33]; t+=(dpint)a[11]*b[32]; t+=(dpint)a[12]*b[31]; t+=(dpint)a[13]*b[30]; t+=(dpint)a[14]*b[29]; t+=(dpint)a[15]*b[28]; t+=(dpint)a[16]*b[27]; t+=(dpint)a[17]*b[26]; t+=(dpint)a[18]*b[25]; t+=(dpint)a[19]*b[24]; t+=(dpint)a[20]*b[23]; t+=(dpint)a[21]*b[22]; t+=(dpint)a[22]*b[21]; t+=(dpint)a[23]*b[20]; t+=(dpint)a[24]*b[19]; t+=(dpint)a[25]*b[18]; t+=(dpint)a[26]*b[17]; t+=(dpint)a[27]*b[16]; t+=(dpint)a[28]*b[15]; t+=(dpint)a[29]*b[14]; t+=(dpint)a[30]*b[13]; t+=(dpint)a[31]*b[12]; t+=(dpint)a[32]*b[11]; t+=(dpint)a[33]*b[10]; t+=(dpint)a[34]*b[9]; t+=(dpint)a[35]*b[8]; t+=(dpint)a[36]*b[7]; t+=(dpint)a[37]*b[6]; t+=(dpint)a[38]*b[5]; t+=(dpint)a[39]*b[4]; t+=(dpint)a[40]*b[3]; t+=(dpint)a[41]*b[2]; t+=(dpint)a[42]*b[1]; t+=(dpint)a[43]*b[0]; spint v43=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[44]; t+=(dpint)a[1]*b[43]; t+=(dpint)a[2]*b[42]; t+=(dpint)a[3]*b[41]; t+=(dpint)a[4]*b[40]; t+=(dpint)a[5]*b[39]; t+=(dpint)a[6]*b[38]; t+=(dpint)a[7]*b[37]; t+=(dpint)a[8]*b[36]; t+=(dpint)a[9]*b[35]; t+=(dpint)a[10]*b[34]; t+=(dpint)a[11]*b[33]; t+=(dpint)a[12]*b[32]; t+=(dpint)a[13]*b[31]; t+=(dpint)a[14]*b[30]; t+=(dpint)a[15]*b[29]; t+=(dpint)a[16]*b[28]; t+=(dpint)a[17]*b[27]; t+=(dpint)a[18]*b[26]; t+=(dpint)a[19]*b[25]; t+=(dpint)a[20]*b[24]; t+=(dpint)a[21]*b[23]; t+=(dpint)a[22]*b[22]; t+=(dpint)a[23]*b[21]; t+=(dpint)a[24]*b[20]; t+=(dpint)a[25]*b[19]; t+=(dpint)a[26]*b[18]; t+=(dpint)a[27]*b[17]; t+=(dpint)a[28]*b[16]; t+=(dpint)a[29]*b[15]; t+=(dpint)a[30]*b[14]; t+=(dpint)a[31]*b[13]; t+=(dpint)a[32]*b[12]; t+=(dpint)a[33]*b[11]; t+=(dpint)a[34]*b[10]; t+=(dpint)a[35]*b[9]; t+=(dpint)a[36]*b[8]; t+=(dpint)a[37]*b[7]; t+=(dpint)a[38]*b[6]; t+=(dpint)a[39]*b[5]; t+=(dpint)a[40]*b[4]; t+=(dpint)a[41]*b[3]; t+=(dpint)a[42]*b[2]; t+=(dpint)a[43]*b[1]; t+=(dpint)a[44]*b[0]; spint v44=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[45]; t+=(dpint)a[1]*b[44]; t+=(dpint)a[2]*b[43]; t+=(dpint)a[3]*b[42]; t+=(dpint)a[4]*b[41]; t+=(dpint)a[5]*b[40]; t+=(dpint)a[6]*b[39]; t+=(dpint)a[7]*b[38]; t+=(dpint)a[8]*b[37]; t+=(dpint)a[9]*b[36]; t+=(dpint)a[10]*b[35]; t+=(dpint)a[11]*b[34]; t+=(dpint)a[12]*b[33]; t+=(dpint)a[13]*b[32]; t+=(dpint)a[14]*b[31]; t+=(dpint)a[15]*b[30]; t+=(dpint)a[16]*b[29]; t+=(dpint)a[17]*b[28]; t+=(dpint)a[18]*b[27]; t+=(dpint)a[19]*b[26]; t+=(dpint)a[20]*b[25]; t+=(dpint)a[21]*b[24]; t+=(dpint)a[22]*b[23]; t+=(dpint)a[23]*b[22]; t+=(dpint)a[24]*b[21]; t+=(dpint)a[25]*b[20]; t+=(dpint)a[26]*b[19]; t+=(dpint)a[27]*b[18]; t+=(dpint)a[28]*b[17]; t+=(dpint)a[29]*b[16]; t+=(dpint)a[30]*b[15]; t+=(dpint)a[31]*b[14]; t+=(dpint)a[32]*b[13]; t+=(dpint)a[33]*b[12]; t+=(dpint)a[34]*b[11]; t+=(dpint)a[35]*b[10]; t+=(dpint)a[36]*b[9]; t+=(dpint)a[37]*b[8]; t+=(dpint)a[38]*b[7]; t+=(dpint)a[39]*b[6]; t+=(dpint)a[40]*b[5]; t+=(dpint)a[41]*b[4]; t+=(dpint)a[42]*b[3]; t+=(dpint)a[43]*b[2]; t+=(dpint)a[44]*b[1]; t+=(dpint)a[45]*b[0]; spint v45=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[46]; t+=(dpint)a[1]*b[45]; t+=(dpint)a[2]*b[44]; t+=(dpint)a[3]*b[43]; t+=(dpint)a[4]*b[42]; t+=(dpint)a[5]*b[41]; t+=(dpint)a[6]*b[40]; t+=(dpint)a[7]*b[39]; t+=(dpint)a[8]*b[38]; t+=(dpint)a[9]*b[37]; t+=(dpint)a[10]*b[36]; t+=(dpint)a[11]*b[35]; t+=(dpint)a[12]*b[34]; t+=(dpint)a[13]*b[33]; t+=(dpint)a[14]*b[32]; t+=(dpint)a[15]*b[31]; t+=(dpint)a[16]*b[30]; t+=(dpint)a[17]*b[29]; t+=(dpint)a[18]*b[28]; t+=(dpint)a[19]*b[27]; t+=(dpint)a[20]*b[26]; t+=(dpint)a[21]*b[25]; t+=(dpint)a[22]*b[24]; t+=(dpint)a[23]*b[23]; t+=(dpint)a[24]*b[22]; t+=(dpint)a[25]*b[21]; t+=(dpint)a[26]*b[20]; t+=(dpint)a[27]*b[19]; t+=(dpint)a[28]*b[18]; t+=(dpint)a[29]*b[17]; t+=(dpint)a[30]*b[16]; t+=(dpint)a[31]*b[15]; t+=(dpint)a[32]*b[14]; t+=(dpint)a[33]*b[13]; t+=(dpint)a[34]*b[12]; t+=(dpint)a[35]*b[11]; t+=(dpint)a[36]*b[10]; t+=(dpint)a[37]*b[9]; t+=(dpint)a[38]*b[8]; t+=(dpint)a[39]*b[7]; t+=(dpint)a[40]*b[6]; t+=(dpint)a[41]*b[5]; t+=(dpint)a[42]*b[4]; t+=(dpint)a[43]*b[3]; t+=(dpint)a[44]*b[2]; t+=(dpint)a[45]*b[1]; t+=(dpint)a[46]*b[0]; spint v46=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[47]; t+=(dpint)a[1]*b[46]; t+=(dpint)a[2]*b[45]; t+=(dpint)a[3]*b[44]; t+=(dpint)a[4]*b[43]; t+=(dpint)a[5]*b[42]; t+=(dpint)a[6]*b[41]; t+=(dpint)a[7]*b[40]; t+=(dpint)a[8]*b[39]; t+=(dpint)a[9]*b[38]; t+=(dpint)a[10]*b[37]; t+=(dpint)a[11]*b[36]; t+=(dpint)a[12]*b[35]; t+=(dpint)a[13]*b[34]; t+=(dpint)a[14]*b[33]; t+=(dpint)a[15]*b[32]; t+=(dpint)a[16]*b[31]; t+=(dpint)a[17]*b[30]; t+=(dpint)a[18]*b[29]; t+=(dpint)a[19]*b[28]; t+=(dpint)a[20]*b[27]; t+=(dpint)a[21]*b[26]; t+=(dpint)a[22]*b[25]; t+=(dpint)a[23]*b[24]; t+=(dpint)a[24]*b[23]; t+=(dpint)a[25]*b[22]; t+=(dpint)a[26]*b[21]; t+=(dpint)a[27]*b[20]; t+=(dpint)a[28]*b[19]; t+=(dpint)a[29]*b[18]; t+=(dpint)a[30]*b[17]; t+=(dpint)a[31]*b[16]; t+=(dpint)a[32]*b[15]; t+=(dpint)a[33]*b[14]; t+=(dpint)a[34]*b[13]; t+=(dpint)a[35]*b[12]; t+=(dpint)a[36]*b[11]; t+=(dpint)a[37]*b[10]; t+=(dpint)a[38]*b[9]; t+=(dpint)a[39]*b[8]; t+=(dpint)a[40]*b[7]; t+=(dpint)a[41]*b[6]; t+=(dpint)a[42]*b[5]; t+=(dpint)a[43]*b[4]; t+=(dpint)a[44]*b[3]; t+=(dpint)a[45]*b[2]; t+=(dpint)a[46]*b[1]; t+=(dpint)a[47]*b[0]; spint v47=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[48]; t+=(dpint)a[1]*b[47]; t+=(dpint)a[2]*b[46]; t+=(dpint)a[3]*b[45]; t+=(dpint)a[4]*b[44]; t+=(dpint)a[5]*b[43]; t+=(dpint)a[6]*b[42]; t+=(dpint)a[7]*b[41]; t+=(dpint)a[8]*b[40]; t+=(dpint)a[9]*b[39]; t+=(dpint)a[10]*b[38]; t+=(dpint)a[11]*b[37]; t+=(dpint)a[12]*b[36]; t+=(dpint)a[13]*b[35]; t+=(dpint)a[14]*b[34]; t+=(dpint)a[15]*b[33]; t+=(dpint)a[16]*b[32]; t+=(dpint)a[17]*b[31]; t+=(dpint)a[18]*b[30]; t+=(dpint)a[19]*b[29]; t+=(dpint)a[20]*b[28]; t+=(dpint)a[21]*b[27]; t+=(dpint)a[22]*b[26]; t+=(dpint)a[23]*b[25]; t+=(dpint)a[24]*b[24]; t+=(dpint)a[25]*b[23]; t+=(dpint)a[26]*b[22]; t+=(dpint)a[27]*b[21]; t+=(dpint)a[28]*b[20]; t+=(dpint)a[29]*b[19]; t+=(dpint)a[30]*b[18]; t+=(dpint)a[31]*b[17]; t+=(dpint)a[32]*b[16]; t+=(dpint)a[33]*b[15]; t+=(dpint)a[34]*b[14]; t+=(dpint)a[35]*b[13]; t+=(dpint)a[36]*b[12]; t+=(dpint)a[37]*b[11]; t+=(dpint)a[38]*b[10]; t+=(dpint)a[39]*b[9]; t+=(dpint)a[40]*b[8]; t+=(dpint)a[41]*b[7]; t+=(dpint)a[42]*b[6]; t+=(dpint)a[43]*b[5]; t+=(dpint)a[44]*b[4]; t+=(dpint)a[45]*b[3]; t+=(dpint)a[46]*b[2]; t+=(dpint)a[47]*b[1]; t+=(dpint)a[48]*b[0]; spint v48=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[49]; t+=(dpint)a[1]*b[48]; t+=(dpint)a[2]*b[47]; t+=(dpint)a[3]*b[46]; t+=(dpint)a[4]*b[45]; t+=(dpint)a[5]*b[44]; t+=(dpint)a[6]*b[43]; t+=(dpint)a[7]*b[42]; t+=(dpint)a[8]*b[41]; t+=(dpint)a[9]*b[40]; t+=(dpint)a[10]*b[39]; t+=(dpint)a[11]*b[38]; t+=(dpint)a[12]*b[37]; t+=(dpint)a[13]*b[36]; t+=(dpint)a[14]*b[35]; t+=(dpint)a[15]*b[34]; t+=(dpint)a[16]*b[33]; t+=(dpint)a[17]*b[32]; t+=(dpint)a[18]*b[31]; t+=(dpint)a[19]*b[30]; t+=(dpint)a[20]*b[29]; t+=(dpint)a[21]*b[28]; t+=(dpint)a[22]*b[27]; t+=(dpint)a[23]*b[26]; t+=(dpint)a[24]*b[25]; t+=(dpint)a[25]*b[24]; t+=(dpint)a[26]*b[23]; t+=(dpint)a[27]*b[22]; t+=(dpint)a[28]*b[21]; t+=(dpint)a[29]*b[20]; t+=(dpint)a[30]*b[19]; t+=(dpint)a[31]*b[18]; t+=(dpint)a[32]*b[17]; t+=(dpint)a[33]*b[16]; t+=(dpint)a[34]*b[15]; t+=(dpint)a[35]*b[14]; t+=(dpint)a[36]*b[13]; t+=(dpint)a[37]*b[12]; t+=(dpint)a[38]*b[11]; t+=(dpint)a[39]*b[10]; t+=(dpint)a[40]*b[9]; t+=(dpint)a[41]*b[8]; t+=(dpint)a[42]*b[7]; t+=(dpint)a[43]*b[6]; t+=(dpint)a[44]*b[5]; t+=(dpint)a[45]*b[4]; t+=(dpint)a[46]*b[3]; t+=(dpint)a[47]*b[2]; t+=(dpint)a[48]*b[1]; t+=(dpint)a[49]*b[0]; spint v49=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[50]; t+=(dpint)a[1]*b[49]; t+=(dpint)a[2]*b[48]; t+=(dpint)a[3]*b[47]; t+=(dpint)a[4]*b[46]; t+=(dpint)a[5]*b[45]; t+=(dpint)a[6]*b[44]; t+=(dpint)a[7]*b[43]; t+=(dpint)a[8]*b[42]; t+=(dpint)a[9]*b[41]; t+=(dpint)a[10]*b[40]; t+=(dpint)a[11]*b[39]; t+=(dpint)a[12]*b[38]; t+=(dpint)a[13]*b[37]; t+=(dpint)a[14]*b[36]; t+=(dpint)a[15]*b[35]; t+=(dpint)a[16]*b[34]; t+=(dpint)a[17]*b[33]; t+=(dpint)a[18]*b[32]; t+=(dpint)a[19]*b[31]; t+=(dpint)a[20]*b[30]; t+=(dpint)a[21]*b[29]; t+=(dpint)a[22]*b[28]; t+=(dpint)a[23]*b[27]; t+=(dpint)a[24]*b[26]; t+=(dpint)a[25]*b[25]; t+=(dpint)a[26]*b[24]; t+=(dpint)a[27]*b[23]; t+=(dpint)a[28]*b[22]; t+=(dpint)a[29]*b[21]; t+=(dpint)a[30]*b[20]; t+=(dpint)a[31]*b[19]; t+=(dpint)a[32]*b[18]; t+=(dpint)a[33]*b[17]; t+=(dpint)a[34]*b[16]; t+=(dpint)a[35]*b[15]; t+=(dpint)a[36]*b[14]; t+=(dpint)a[37]*b[13]; t+=(dpint)a[38]*b[12]; t+=(dpint)a[39]*b[11]; t+=(dpint)a[40]*b[10]; t+=(dpint)a[41]*b[9]; t+=(dpint)a[42]*b[8]; t+=(dpint)a[43]*b[7]; t+=(dpint)a[44]*b[6]; t+=(dpint)a[45]*b[5]; t+=(dpint)a[46]*b[4]; t+=(dpint)a[47]*b[3]; t+=(dpint)a[48]*b[2]; t+=(dpint)a[49]*b[1]; t+=(dpint)a[50]*b[0]; spint v50=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[51]; t+=(dpint)a[1]*b[50]; t+=(dpint)a[2]*b[49]; t+=(dpint)a[3]*b[48]; t+=(dpint)a[4]*b[47]; t+=(dpint)a[5]*b[46]; t+=(dpint)a[6]*b[45]; t+=(dpint)a[7]*b[44]; t+=(dpint)a[8]*b[43]; t+=(dpint)a[9]*b[42]; t+=(dpint)a[10]*b[41]; t+=(dpint)a[11]*b[40]; t+=(dpint)a[12]*b[39]; t+=(dpint)a[13]*b[38]; t+=(dpint)a[14]*b[37]; t+=(dpint)a[15]*b[36]; t+=(dpint)a[16]*b[35]; t+=(dpint)a[17]*b[34]; t+=(dpint)a[18]*b[33]; t+=(dpint)a[19]*b[32]; t+=(dpint)a[20]*b[31]; t+=(dpint)a[21]*b[30]; t+=(dpint)a[22]*b[29]; t+=(dpint)a[23]*b[28]; t+=(dpint)a[24]*b[27]; t+=(dpint)a[25]*b[26]; t+=(dpint)a[26]*b[25]; t+=(dpint)a[27]*b[24]; t+=(dpint)a[28]*b[23]; t+=(dpint)a[29]*b[22]; t+=(dpint)a[30]*b[21]; t+=(dpint)a[31]*b[20]; t+=(dpint)a[32]*b[19]; t+=(dpint)a[33]*b[18]; t+=(dpint)a[34]*b[17]; t+=(dpint)a[35]*b[16]; t+=(dpint)a[36]*b[15]; t+=(dpint)a[37]*b[14]; t+=(dpint)a[38]*b[13]; t+=(dpint)a[39]*b[12]; t+=(dpint)a[40]*b[11]; t+=(dpint)a[41]*b[10]; t+=(dpint)a[42]*b[9]; t+=(dpint)a[43]*b[8]; t+=(dpint)a[44]*b[7]; t+=(dpint)a[45]*b[6]; t+=(dpint)a[46]*b[5]; t+=(dpint)a[47]*b[4]; t+=(dpint)a[48]*b[3]; t+=(dpint)a[49]*b[2]; t+=(dpint)a[50]*b[1]; t+=(dpint)a[51]*b[0]; spint v51=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[52]; t+=(dpint)a[1]*b[51]; t+=(dpint)a[2]*b[50]; t+=(dpint)a[3]*b[49]; t+=(dpint)a[4]*b[48]; t+=(dpint)a[5]*b[47]; t+=(dpint)a[6]*b[46]; t+=(dpint)a[7]*b[45]; t+=(dpint)a[8]*b[44]; t+=(dpint)a[9]*b[43]; t+=(dpint)a[10]*b[42]; t+=(dpint)a[11]*b[41]; t+=(dpint)a[12]*b[40]; t+=(dpint)a[13]*b[39]; t+=(dpint)a[14]*b[38]; t+=(dpint)a[15]*b[37]; t+=(dpint)a[16]*b[36]; t+=(dpint)a[17]*b[35]; t+=(dpint)a[18]*b[34]; t+=(dpint)a[19]*b[33]; t+=(dpint)a[20]*b[32]; t+=(dpint)a[21]*b[31]; t+=(dpint)a[22]*b[30]; t+=(dpint)a[23]*b[29]; t+=(dpint)a[24]*b[28]; t+=(dpint)a[25]*b[27]; t+=(dpint)a[26]*b[26]; t+=(dpint)a[27]*b[25]; t+=(dpint)a[28]*b[24]; t+=(dpint)a[29]*b[23]; t+=(dpint)a[30]*b[22]; t+=(dpint)a[31]*b[21]; t+=(dpint)a[32]*b[20]; t+=(dpint)a[33]*b[19]; t+=(dpint)a[34]*b[18]; t+=(dpint)a[35]*b[17]; t+=(dpint)a[36]*b[16]; t+=(dpint)a[37]*b[15]; t+=(dpint)a[38]*b[14]; t+=(dpint)a[39]*b[13]; t+=(dpint)a[40]*b[12]; t+=(dpint)a[41]*b[11]; t+=(dpint)a[42]*b[10]; t+=(dpint)a[43]*b[9]; t+=(dpint)a[44]*b[8]; t+=(dpint)a[45]*b[7]; t+=(dpint)a[46]*b[6]; t+=(dpint)a[47]*b[5]; t+=(dpint)a[48]*b[4]; t+=(dpint)a[49]*b[3]; t+=(dpint)a[50]*b[2]; t+=(dpint)a[51]*b[1]; t+=(dpint)a[52]*b[0]; spint v52=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[53]; t+=(dpint)a[1]*b[52]; t+=(dpint)a[2]*b[51]; t+=(dpint)a[3]*b[50]; t+=(dpint)a[4]*b[49]; t+=(dpint)a[5]*b[48]; t+=(dpint)a[6]*b[47]; t+=(dpint)a[7]*b[46]; t+=(dpint)a[8]*b[45]; t+=(dpint)a[9]*b[44]; t+=(dpint)a[10]*b[43]; t+=(dpint)a[11]*b[42]; t+=(dpint)a[12]*b[41]; t+=(dpint)a[13]*b[40]; t+=(dpint)a[14]*b[39]; t+=(dpint)a[15]*b[38]; t+=(dpint)a[16]*b[37]; t+=(dpint)a[17]*b[36]; t+=(dpint)a[18]*b[35]; t+=(dpint)a[19]*b[34]; t+=(dpint)a[20]*b[33]; t+=(dpint)a[21]*b[32]; t+=(dpint)a[22]*b[31]; t+=(dpint)a[23]*b[30]; t+=(dpint)a[24]*b[29]; t+=(dpint)a[25]*b[28]; t+=(dpint)a[26]*b[27]; t+=(dpint)a[27]*b[26]; t+=(dpint)a[28]*b[25]; t+=(dpint)a[29]*b[24]; t+=(dpint)a[30]*b[23]; t+=(dpint)a[31]*b[22]; t+=(dpint)a[32]*b[21]; t+=(dpint)a[33]*b[20]; t+=(dpint)a[34]*b[19]; t+=(dpint)a[35]*b[18]; t+=(dpint)a[36]*b[17]; t+=(dpint)a[37]*b[16]; t+=(dpint)a[38]*b[15]; t+=(dpint)a[39]*b[14]; t+=(dpint)a[40]*b[13]; t+=(dpint)a[41]*b[12]; t+=(dpint)a[42]*b[11]; t+=(dpint)a[43]*b[10]; t+=(dpint)a[44]*b[9]; t+=(dpint)a[45]*b[8]; t+=(dpint)a[46]*b[7]; t+=(dpint)a[47]*b[6]; t+=(dpint)a[48]*b[5]; t+=(dpint)a[49]*b[4]; t+=(dpint)a[50]*b[3]; t+=(dpint)a[51]*b[2]; t+=(dpint)a[52]*b[1]; t+=(dpint)a[53]*b[0]; spint v53=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[54]; t+=(dpint)a[1]*b[53]; t+=(dpint)a[2]*b[52]; t+=(dpint)a[3]*b[51]; t+=(dpint)a[4]*b[50]; t+=(dpint)a[5]*b[49]; t+=(dpint)a[6]*b[48]; t+=(dpint)a[7]*b[47]; t+=(dpint)a[8]*b[46]; t+=(dpint)a[9]*b[45]; t+=(dpint)a[10]*b[44]; t+=(dpint)a[11]*b[43]; t+=(dpint)a[12]*b[42]; t+=(dpint)a[13]*b[41]; t+=(dpint)a[14]*b[40]; t+=(dpint)a[15]*b[39]; t+=(dpint)a[16]*b[38]; t+=(dpint)a[17]*b[37]; t+=(dpint)a[18]*b[36]; t+=(dpint)a[19]*b[35]; t+=(dpint)a[20]*b[34]; t+=(dpint)a[21]*b[33]; t+=(dpint)a[22]*b[32]; t+=(dpint)a[23]*b[31]; t+=(dpint)a[24]*b[30]; t+=(dpint)a[25]*b[29]; t+=(dpint)a[26]*b[28]; t+=(dpint)a[27]*b[27]; t+=(dpint)a[28]*b[26]; t+=(dpint)a[29]*b[25]; t+=(dpint)a[30]*b[24]; t+=(dpint)a[31]*b[23]; t+=(dpint)a[32]*b[22]; t+=(dpint)a[33]*b[21]; t+=(dpint)a[34]*b[20]; t+=(dpint)a[35]*b[19]; t+=(dpint)a[36]*b[18]; t+=(dpint)a[37]*b[17]; t+=(dpint)a[38]*b[16]; t+=(dpint)a[39]*b[15]; t+=(dpint)a[40]*b[14]; t+=(dpint)a[41]*b[13]; t+=(dpint)a[42]*b[12]; t+=(dpint)a[43]*b[11]; t+=(dpint)a[44]*b[10]; t+=(dpint)a[45]*b[9]; t+=(dpint)a[46]*b[8]; t+=(dpint)a[47]*b[7]; t+=(dpint)a[48]*b[6]; t+=(dpint)a[49]*b[5]; t+=(dpint)a[50]*b[4]; t+=(dpint)a[51]*b[3]; t+=(dpint)a[52]*b[2]; t+=(dpint)a[53]*b[1]; t+=(dpint)a[54]*b[0]; spint v54=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[55]; t+=(dpint)a[1]*b[54]; t+=(dpint)a[2]*b[53]; t+=(dpint)a[3]*b[52]; t+=(dpint)a[4]*b[51]; t+=(dpint)a[5]*b[50]; t+=(dpint)a[6]*b[49]; t+=(dpint)a[7]*b[48]; t+=(dpint)a[8]*b[47]; t+=(dpint)a[9]*b[46]; t+=(dpint)a[10]*b[45]; t+=(dpint)a[11]*b[44]; t+=(dpint)a[12]*b[43]; t+=(dpint)a[13]*b[42]; t+=(dpint)a[14]*b[41]; t+=(dpint)a[15]*b[40]; t+=(dpint)a[16]*b[39]; t+=(dpint)a[17]*b[38]; t+=(dpint)a[18]*b[37]; t+=(dpint)a[19]*b[36]; t+=(dpint)a[20]*b[35]; t+=(dpint)a[21]*b[34]; t+=(dpint)a[22]*b[33]; t+=(dpint)a[23]*b[32]; t+=(dpint)a[24]*b[31]; t+=(dpint)a[25]*b[30]; t+=(dpint)a[26]*b[29]; t+=(dpint)a[27]*b[28]; t+=(dpint)a[28]*b[27]; t+=(dpint)a[29]*b[26]; t+=(dpint)a[30]*b[25]; t+=(dpint)a[31]*b[24]; t+=(dpint)a[32]*b[23]; t+=(dpint)a[33]*b[22]; t+=(dpint)a[34]*b[21]; t+=(dpint)a[35]*b[20]; t+=(dpint)a[36]*b[19]; t+=(dpint)a[37]*b[18]; t+=(dpint)a[38]*b[17]; t+=(dpint)a[39]*b[16]; t+=(dpint)a[40]*b[15]; t+=(dpint)a[41]*b[14]; t+=(dpint)a[42]*b[13]; t+=(dpint)a[43]*b[12]; t+=(dpint)a[44]*b[11]; t+=(dpint)a[45]*b[10]; t+=(dpint)a[46]*b[9]; t+=(dpint)a[47]*b[8]; t+=(dpint)a[48]*b[7]; t+=(dpint)a[49]*b[6]; t+=(dpint)a[50]*b[5]; t+=(dpint)a[51]*b[4]; t+=(dpint)a[52]*b[3]; t+=(dpint)a[53]*b[2]; t+=(dpint)a[54]*b[1]; t+=(dpint)a[55]*b[0]; spint v55=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[56]; t+=(dpint)a[1]*b[55]; t+=(dpint)a[2]*b[54]; t+=(dpint)a[3]*b[53]; t+=(dpint)a[4]*b[52]; t+=(dpint)a[5]*b[51]; t+=(dpint)a[6]*b[50]; t+=(dpint)a[7]*b[49]; t+=(dpint)a[8]*b[48]; t+=(dpint)a[9]*b[47]; t+=(dpint)a[10]*b[46]; t+=(dpint)a[11]*b[45]; t+=(dpint)a[12]*b[44]; t+=(dpint)a[13]*b[43]; t+=(dpint)a[14]*b[42]; t+=(dpint)a[15]*b[41]; t+=(dpint)a[16]*b[40]; t+=(dpint)a[17]*b[39]; t+=(dpint)a[18]*b[38]; t+=(dpint)a[19]*b[37]; t+=(dpint)a[20]*b[36]; t+=(dpint)a[21]*b[35]; t+=(dpint)a[22]*b[34]; t+=(dpint)a[23]*b[33]; t+=(dpint)a[24]*b[32]; t+=(dpint)a[25]*b[31]; t+=(dpint)a[26]*b[30]; t+=(dpint)a[27]*b[29]; t+=(dpint)a[28]*b[28]; t+=(dpint)a[29]*b[27]; t+=(dpint)a[30]*b[26]; t+=(dpint)a[31]*b[25]; t+=(dpint)a[32]*b[24]; t+=(dpint)a[33]*b[23]; t+=(dpint)a[34]*b[22]; t+=(dpint)a[35]*b[21]; t+=(dpint)a[36]*b[20]; t+=(dpint)a[37]*b[19]; t+=(dpint)a[38]*b[18]; t+=(dpint)a[39]*b[17]; t+=(dpint)a[40]*b[16]; t+=(dpint)a[41]*b[15]; t+=(dpint)a[42]*b[14]; t+=(dpint)a[43]*b[13]; t+=(dpint)a[44]*b[12]; t+=(dpint)a[45]*b[11]; t+=(dpint)a[46]*b[10]; t+=(dpint)a[47]*b[9]; t+=(dpint)a[48]*b[8]; t+=(dpint)a[49]*b[7]; t+=(dpint)a[50]*b[6]; t+=(dpint)a[51]*b[5]; t+=(dpint)a[52]*b[4]; t+=(dpint)a[53]*b[3]; t+=(dpint)a[54]*b[2]; t+=(dpint)a[55]*b[1]; t+=(dpint)a[56]*b[0]; spint v56=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[57]; t+=(dpint)a[1]*b[56]; t+=(dpint)a[2]*b[55]; t+=(dpint)a[3]*b[54]; t+=(dpint)a[4]*b[53]; t+=(dpint)a[5]*b[52]; t+=(dpint)a[6]*b[51]; t+=(dpint)a[7]*b[50]; t+=(dpint)a[8]*b[49]; t+=(dpint)a[9]*b[48]; t+=(dpint)a[10]*b[47]; t+=(dpint)a[11]*b[46]; t+=(dpint)a[12]*b[45]; t+=(dpint)a[13]*b[44]; t+=(dpint)a[14]*b[43]; t+=(dpint)a[15]*b[42]; t+=(dpint)a[16]*b[41]; t+=(dpint)a[17]*b[40]; t+=(dpint)a[18]*b[39]; t+=(dpint)a[19]*b[38]; t+=(dpint)a[20]*b[37]; t+=(dpint)a[21]*b[36]; t+=(dpint)a[22]*b[35]; t+=(dpint)a[23]*b[34]; t+=(dpint)a[24]*b[33]; t+=(dpint)a[25]*b[32]; t+=(dpint)a[26]*b[31]; t+=(dpint)a[27]*b[30]; t+=(dpint)a[28]*b[29]; t+=(dpint)a[29]*b[28]; t+=(dpint)a[30]*b[27]; t+=(dpint)a[31]*b[26]; t+=(dpint)a[32]*b[25]; t+=(dpint)a[33]*b[24]; t+=(dpint)a[34]*b[23]; t+=(dpint)a[35]*b[22]; t+=(dpint)a[36]*b[21]; t+=(dpint)a[37]*b[20]; t+=(dpint)a[38]*b[19]; t+=(dpint)a[39]*b[18]; t+=(dpint)a[40]*b[17]; t+=(dpint)a[41]*b[16]; t+=(dpint)a[42]*b[15]; t+=(dpint)a[43]*b[14]; t+=(dpint)a[44]*b[13]; t+=(dpint)a[45]*b[12]; t+=(dpint)a[46]*b[11]; t+=(dpint)a[47]*b[10]; t+=(dpint)a[48]*b[9]; t+=(dpint)a[49]*b[8]; t+=(dpint)a[50]*b[7]; t+=(dpint)a[51]*b[6]; t+=(dpint)a[52]*b[5]; t+=(dpint)a[53]*b[4]; t+=(dpint)a[54]*b[3]; t+=(dpint)a[55]*b[2]; t+=(dpint)a[56]*b[1]; t+=(dpint)a[57]*b[0]; spint v57=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[58]; t+=(dpint)a[1]*b[57]; t+=(dpint)a[2]*b[56]; t+=(dpint)a[3]*b[55]; t+=(dpint)a[4]*b[54]; t+=(dpint)a[5]*b[53]; t+=(dpint)a[6]*b[52]; t+=(dpint)a[7]*b[51]; t+=(dpint)a[8]*b[50]; t+=(dpint)a[9]*b[49]; t+=(dpint)a[10]*b[48]; t+=(dpint)a[11]*b[47]; t+=(dpint)a[12]*b[46]; t+=(dpint)a[13]*b[45]; t+=(dpint)a[14]*b[44]; t+=(dpint)a[15]*b[43]; t+=(dpint)a[16]*b[42]; t+=(dpint)a[17]*b[41]; t+=(dpint)a[18]*b[40]; t+=(dpint)a[19]*b[39]; t+=(dpint)a[20]*b[38]; t+=(dpint)a[21]*b[37]; t+=(dpint)a[22]*b[36]; t+=(dpint)a[23]*b[35]; t+=(dpint)a[24]*b[34]; t+=(dpint)a[25]*b[33]; t+=(dpint)a[26]*b[32]; t+=(dpint)a[27]*b[31]; t+=(dpint)a[28]*b[30]; t+=(dpint)a[29]*b[29]; t+=(dpint)a[30]*b[28]; t+=(dpint)a[31]*b[27]; t+=(dpint)a[32]*b[26]; t+=(dpint)a[33]*b[25]; t+=(dpint)a[34]*b[24]; t+=(dpint)a[35]*b[23]; t+=(dpint)a[36]*b[22]; t+=(dpint)a[37]*b[21]; t+=(dpint)a[38]*b[20]; t+=(dpint)a[39]*b[19]; t+=(dpint)a[40]*b[18]; t+=(dpint)a[41]*b[17]; t+=(dpint)a[42]*b[16]; t+=(dpint)a[43]*b[15]; t+=(dpint)a[44]*b[14]; t+=(dpint)a[45]*b[13]; t+=(dpint)a[46]*b[12]; t+=(dpint)a[47]*b[11]; t+=(dpint)a[48]*b[10]; t+=(dpint)a[49]*b[9]; t+=(dpint)a[50]*b[8]; t+=(dpint)a[51]*b[7]; t+=(dpint)a[52]*b[6]; t+=(dpint)a[53]*b[5]; t+=(dpint)a[54]*b[4]; t+=(dpint)a[55]*b[3]; t+=(dpint)a[56]*b[2]; t+=(dpint)a[57]*b[1]; t+=(dpint)a[58]*b[0]; spint v58=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[59]; t+=(dpint)a[1]*b[58]; t+=(dpint)a[2]*b[57]; t+=(dpint)a[3]*b[56]; t+=(dpint)a[4]*b[55]; t+=(dpint)a[5]*b[54]; t+=(dpint)a[6]*b[53]; t+=(dpint)a[7]*b[52]; t+=(dpint)a[8]*b[51]; t+=(dpint)a[9]*b[50]; t+=(dpint)a[10]*b[49]; t+=(dpint)a[11]*b[48]; t+=(dpint)a[12]*b[47]; t+=(dpint)a[13]*b[46]; t+=(dpint)a[14]*b[45]; t+=(dpint)a[15]*b[44]; t+=(dpint)a[16]*b[43]; t+=(dpint)a[17]*b[42]; t+=(dpint)a[18]*b[41]; t+=(dpint)a[19]*b[40]; t+=(dpint)a[20]*b[39]; t+=(dpint)a[21]*b[38]; t+=(dpint)a[22]*b[37]; t+=(dpint)a[23]*b[36]; t+=(dpint)a[24]*b[35]; t+=(dpint)a[25]*b[34]; t+=(dpint)a[26]*b[33]; t+=(dpint)a[27]*b[32]; t+=(dpint)a[28]*b[31]; t+=(dpint)a[29]*b[30]; t+=(dpint)a[30]*b[29]; t+=(dpint)a[31]*b[28]; t+=(dpint)a[32]*b[27]; t+=(dpint)a[33]*b[26]; t+=(dpint)a[34]*b[25]; t+=(dpint)a[35]*b[24]; t+=(dpint)a[36]*b[23]; t+=(dpint)a[37]*b[22]; t+=(dpint)a[38]*b[21]; t+=(dpint)a[39]*b[20]; t+=(dpint)a[40]*b[19]; t+=(dpint)a[41]*b[18]; t+=(dpint)a[42]*b[17]; t+=(dpint)a[43]*b[16]; t+=(dpint)a[44]*b[15]; t+=(dpint)a[45]*b[14]; t+=(dpint)a[46]*b[13]; t+=(dpint)a[47]*b[12]; t+=(dpint)a[48]*b[11]; t+=(dpint)a[49]*b[10]; t+=(dpint)a[50]*b[9]; t+=(dpint)a[51]*b[8]; t+=(dpint)a[52]*b[7]; t+=(dpint)a[53]*b[6]; t+=(dpint)a[54]*b[5]; t+=(dpint)a[55]*b[4]; t+=(dpint)a[56]*b[3]; t+=(dpint)a[57]*b[2]; t+=(dpint)a[58]*b[1]; t+=(dpint)a[59]*b[0]; spint v59=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[60]; t+=(dpint)a[1]*b[59]; t+=(dpint)a[2]*b[58]; t+=(dpint)a[3]*b[57]; t+=(dpint)a[4]*b[56]; t+=(dpint)a[5]*b[55]; t+=(dpint)a[6]*b[54]; t+=(dpint)a[7]*b[53]; t+=(dpint)a[8]*b[52]; t+=(dpint)a[9]*b[51]; t+=(dpint)a[10]*b[50]; t+=(dpint)a[11]*b[49]; t+=(dpint)a[12]*b[48]; t+=(dpint)a[13]*b[47]; t+=(dpint)a[14]*b[46]; t+=(dpint)a[15]*b[45]; t+=(dpint)a[16]*b[44]; t+=(dpint)a[17]*b[43]; t+=(dpint)a[18]*b[42]; t+=(dpint)a[19]*b[41]; t+=(dpint)a[20]*b[40]; t+=(dpint)a[21]*b[39]; t+=(dpint)a[22]*b[38]; t+=(dpint)a[23]*b[37]; t+=(dpint)a[24]*b[36]; t+=(dpint)a[25]*b[35]; t+=(dpint)a[26]*b[34]; t+=(dpint)a[27]*b[33]; t+=(dpint)a[28]*b[32]; t+=(dpint)a[29]*b[31]; t+=(dpint)a[30]*b[30]; t+=(dpint)a[31]*b[29]; t+=(dpint)a[32]*b[28]; t+=(dpint)a[33]*b[27]; t+=(dpint)a[34]*b[26]; t+=(dpint)a[35]*b[25]; t+=(dpint)a[36]*b[24]; t+=(dpint)a[37]*b[23]; t+=(dpint)a[38]*b[22]; t+=(dpint)a[39]*b[21]; t+=(dpint)a[40]*b[20]; t+=(dpint)a[41]*b[19]; t+=(dpint)a[42]*b[18]; t+=(dpint)a[43]*b[17]; t+=(dpint)a[44]*b[16]; t+=(dpint)a[45]*b[15]; t+=(dpint)a[46]*b[14]; t+=(dpint)a[47]*b[13]; t+=(dpint)a[48]*b[12]; t+=(dpint)a[49]*b[11]; t+=(dpint)a[50]*b[10]; t+=(dpint)a[51]*b[9]; t+=(dpint)a[52]*b[8]; t+=(dpint)a[53]*b[7]; t+=(dpint)a[54]*b[6]; t+=(dpint)a[55]*b[5]; t+=(dpint)a[56]*b[4]; t+=(dpint)a[57]*b[3]; t+=(dpint)a[58]*b[2]; t+=(dpint)a[59]*b[1]; t+=(dpint)a[60]*b[0]; spint v60=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[61]; t+=(dpint)a[1]*b[60]; t+=(dpint)a[2]*b[59]; t+=(dpint)a[3]*b[58]; t+=(dpint)a[4]*b[57]; t+=(dpint)a[5]*b[56]; t+=(dpint)a[6]*b[55]; t+=(dpint)a[7]*b[54]; t+=(dpint)a[8]*b[53]; t+=(dpint)a[9]*b[52]; t+=(dpint)a[10]*b[51]; t+=(dpint)a[11]*b[50]; t+=(dpint)a[12]*b[49]; t+=(dpint)a[13]*b[48]; t+=(dpint)a[14]*b[47]; t+=(dpint)a[15]*b[46]; t+=(dpint)a[16]*b[45]; t+=(dpint)a[17]*b[44]; t+=(dpint)a[18]*b[43]; t+=(dpint)a[19]*b[42]; t+=(dpint)a[20]*b[41]; t+=(dpint)a[21]*b[40]; t+=(dpint)a[22]*b[39]; t+=(dpint)a[23]*b[38]; t+=(dpint)a[24]*b[37]; t+=(dpint)a[25]*b[36]; t+=(dpint)a[26]*b[35]; t+=(dpint)a[27]*b[34]; t+=(dpint)a[28]*b[33]; t+=(dpint)a[29]*b[32]; t+=(dpint)a[30]*b[31]; t+=(dpint)a[31]*b[30]; t+=(dpint)a[32]*b[29]; t+=(dpint)a[33]*b[28]; t+=(dpint)a[34]*b[27]; t+=(dpint)a[35]*b[26]; t+=(dpint)a[36]*b[25]; t+=(dpint)a[37]*b[24]; t+=(dpint)a[38]*b[23]; t+=(dpint)a[39]*b[22]; t+=(dpint)a[40]*b[21]; t+=(dpint)a[41]*b[20]; t+=(dpint)a[42]*b[19]; t+=(dpint)a[43]*b[18]; t+=(dpint)a[44]*b[17]; t+=(dpint)a[45]*b[16]; t+=(dpint)a[46]*b[15]; t+=(dpint)a[47]*b[14]; t+=(dpint)a[48]*b[13]; t+=(dpint)a[49]*b[12]; t+=(dpint)a[50]*b[11]; t+=(dpint)a[51]*b[10]; t+=(dpint)a[52]*b[9]; t+=(dpint)a[53]*b[8]; t+=(dpint)a[54]*b[7]; t+=(dpint)a[55]*b[6]; t+=(dpint)a[56]*b[5]; t+=(dpint)a[57]*b[4]; t+=(dpint)a[58]*b[3]; t+=(dpint)a[59]*b[2]; t+=(dpint)a[60]*b[1]; t+=(dpint)a[61]*b[0]; spint v61=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[62]; t+=(dpint)a[1]*b[61]; t+=(dpint)a[2]*b[60]; t+=(dpint)a[3]*b[59]; t+=(dpint)a[4]*b[58]; t+=(dpint)a[5]*b[57]; t+=(dpint)a[6]*b[56]; t+=(dpint)a[7]*b[55]; t+=(dpint)a[8]*b[54]; t+=(dpint)a[9]*b[53]; t+=(dpint)a[10]*b[52]; t+=(dpint)a[11]*b[51]; t+=(dpint)a[12]*b[50]; t+=(dpint)a[13]*b[49]; t+=(dpint)a[14]*b[48]; t+=(dpint)a[15]*b[47]; t+=(dpint)a[16]*b[46]; t+=(dpint)a[17]*b[45]; t+=(dpint)a[18]*b[44]; t+=(dpint)a[19]*b[43]; t+=(dpint)a[20]*b[42]; t+=(dpint)a[21]*b[41]; t+=(dpint)a[22]*b[40]; t+=(dpint)a[23]*b[39]; t+=(dpint)a[24]*b[38]; t+=(dpint)a[25]*b[37]; t+=(dpint)a[26]*b[36]; t+=(dpint)a[27]*b[35]; t+=(dpint)a[28]*b[34]; t+=(dpint)a[29]*b[33]; t+=(dpint)a[30]*b[32]; t+=(dpint)a[31]*b[31]; t+=(dpint)a[32]*b[30]; t+=(dpint)a[33]*b[29]; t+=(dpint)a[34]*b[28]; t+=(dpint)a[35]*b[27]; t+=(dpint)a[36]*b[26]; t+=(dpint)a[37]*b[25]; t+=(dpint)a[38]*b[24]; t+=(dpint)a[39]*b[23]; t+=(dpint)a[40]*b[22]; t+=(dpint)a[41]*b[21]; t+=(dpint)a[42]*b[20]; t+=(dpint)a[43]*b[19]; t+=(dpint)a[44]*b[18]; t+=(dpint)a[45]*b[17]; t+=(dpint)a[46]*b[16]; t+=(dpint)a[47]*b[15]; t+=(dpint)a[48]*b[14]; t+=(dpint)a[49]*b[13]; t+=(dpint)a[50]*b[12]; t+=(dpint)a[51]*b[11]; t+=(dpint)a[52]*b[10]; t+=(dpint)a[53]*b[9]; t+=(dpint)a[54]*b[8]; t+=(dpint)a[55]*b[7]; t+=(dpint)a[56]*b[6]; t+=(dpint)a[57]*b[5]; t+=(dpint)a[58]*b[4]; t+=(dpint)a[59]*b[3]; t+=(dpint)a[60]*b[2]; t+=(dpint)a[61]*b[1]; t+=(dpint)a[62]*b[0]; spint v62=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[63]; t+=(dpint)a[1]*b[62]; t+=(dpint)a[2]*b[61]; t+=(dpint)a[3]*b[60]; t+=(dpint)a[4]*b[59]; t+=(dpint)a[5]*b[58]; t+=(dpint)a[6]*b[57]; t+=(dpint)a[7]*b[56]; t+=(dpint)a[8]*b[55]; t+=(dpint)a[9]*b[54]; t+=(dpint)a[10]*b[53]; t+=(dpint)a[11]*b[52]; t+=(dpint)a[12]*b[51]; t+=(dpint)a[13]*b[50]; t+=(dpint)a[14]*b[49]; t+=(dpint)a[15]*b[48]; t+=(dpint)a[16]*b[47]; t+=(dpint)a[17]*b[46]; t+=(dpint)a[18]*b[45]; t+=(dpint)a[19]*b[44]; t+=(dpint)a[20]*b[43]; t+=(dpint)a[21]*b[42]; t+=(dpint)a[22]*b[41]; t+=(dpint)a[23]*b[40]; t+=(dpint)a[24]*b[39]; t+=(dpint)a[25]*b[38]; t+=(dpint)a[26]*b[37]; t+=(dpint)a[27]*b[36]; t+=(dpint)a[28]*b[35]; t+=(dpint)a[29]*b[34]; t+=(dpint)a[30]*b[33]; t+=(dpint)a[31]*b[32]; t+=(dpint)a[32]*b[31]; t+=(dpint)a[33]*b[30]; t+=(dpint)a[34]*b[29]; t+=(dpint)a[35]*b[28]; t+=(dpint)a[36]*b[27]; t+=(dpint)a[37]*b[26]; t+=(dpint)a[38]*b[25]; t+=(dpint)a[39]*b[24]; t+=(dpint)a[40]*b[23]; t+=(dpint)a[41]*b[22]; t+=(dpint)a[42]*b[21]; t+=(dpint)a[43]*b[20]; t+=(dpint)a[44]*b[19]; t+=(dpint)a[45]*b[18]; t+=(dpint)a[46]*b[17]; t+=(dpint)a[47]*b[16]; t+=(dpint)a[48]*b[15]; t+=(dpint)a[49]*b[14]; t+=(dpint)a[50]*b[13]; t+=(dpint)a[51]*b[12]; t+=(dpint)a[52]*b[11]; t+=(dpint)a[53]*b[10]; t+=(dpint)a[54]*b[9]; t+=(dpint)a[55]*b[8]; t+=(dpint)a[56]*b[7]; t+=(dpint)a[57]*b[6]; t+=(dpint)a[58]*b[5]; t+=(dpint)a[59]*b[4]; t+=(dpint)a[60]*b[3]; t+=(dpint)a[61]*b[2]; t+=(dpint)a[62]*b[1]; t+=(dpint)a[63]*b[0]; spint v63=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[64]; t+=(dpint)a[1]*b[63]; t+=(dpint)a[2]*b[62]; t+=(dpint)a[3]*b[61]; t+=(dpint)a[4]*b[60]; t+=(dpint)a[5]*b[59]; t+=(dpint)a[6]*b[58]; t+=(dpint)a[7]*b[57]; t+=(dpint)a[8]*b[56]; t+=(dpint)a[9]*b[55]; t+=(dpint)a[10]*b[54]; t+=(dpint)a[11]*b[53]; t+=(dpint)a[12]*b[52]; t+=(dpint)a[13]*b[51]; t+=(dpint)a[14]*b[50]; t+=(dpint)a[15]*b[49]; t+=(dpint)a[16]*b[48]; t+=(dpint)a[17]*b[47]; t+=(dpint)a[18]*b[46]; t+=(dpint)a[19]*b[45]; t+=(dpint)a[20]*b[44]; t+=(dpint)a[21]*b[43]; t+=(dpint)a[22]*b[42]; t+=(dpint)a[23]*b[41]; t+=(dpint)a[24]*b[40]; t+=(dpint)a[25]*b[39]; t+=(dpint)a[26]*b[38]; t+=(dpint)a[27]*b[37]; t+=(dpint)a[28]*b[36]; t+=(dpint)a[29]*b[35]; t+=(dpint)a[30]*b[34]; t+=(dpint)a[31]*b[33]; t+=(dpint)a[32]*b[32]; t+=(dpint)a[33]*b[31]; t+=(dpint)a[34]*b[30]; t+=(dpint)a[35]*b[29]; t+=(dpint)a[36]*b[28]; t+=(dpint)a[37]*b[27]; t+=(dpint)a[38]*b[26]; t+=(dpint)a[39]*b[25]; t+=(dpint)a[40]*b[24]; t+=(dpint)a[41]*b[23]; t+=(dpint)a[42]*b[22]; t+=(dpint)a[43]*b[21]; t+=(dpint)a[44]*b[20]; t+=(dpint)a[45]*b[19]; t+=(dpint)a[46]*b[18]; t+=(dpint)a[47]*b[17]; t+=(dpint)a[48]*b[16]; t+=(dpint)a[49]*b[15]; t+=(dpint)a[50]*b[14]; t+=(dpint)a[51]*b[13]; t+=(dpint)a[52]*b[12]; t+=(dpint)a[53]*b[11]; t+=(dpint)a[54]*b[10]; t+=(dpint)a[55]*b[9]; t+=(dpint)a[56]*b[8]; t+=(dpint)a[57]*b[7]; t+=(dpint)a[58]*b[6]; t+=(dpint)a[59]*b[5]; t+=(dpint)a[60]*b[4]; t+=(dpint)a[61]*b[3]; t+=(dpint)a[62]*b[2]; t+=(dpint)a[63]*b[1]; t+=(dpint)a[64]*b[0]; spint v64=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[65]; t+=(dpint)a[1]*b[64]; t+=(dpint)a[2]*b[63]; t+=(dpint)a[3]*b[62]; t+=(dpint)a[4]*b[61]; t+=(dpint)a[5]*b[60]; t+=(dpint)a[6]*b[59]; t+=(dpint)a[7]*b[58]; t+=(dpint)a[8]*b[57]; t+=(dpint)a[9]*b[56]; t+=(dpint)a[10]*b[55]; t+=(dpint)a[11]*b[54]; t+=(dpint)a[12]*b[53]; t+=(dpint)a[13]*b[52]; t+=(dpint)a[14]*b[51]; t+=(dpint)a[15]*b[50]; t+=(dpint)a[16]*b[49]; t+=(dpint)a[17]*b[48]; t+=(dpint)a[18]*b[47]; t+=(dpint)a[19]*b[46]; t+=(dpint)a[20]*b[45]; t+=(dpint)a[21]*b[44]; t+=(dpint)a[22]*b[43]; t+=(dpint)a[23]*b[42]; t+=(dpint)a[24]*b[41]; t+=(dpint)a[25]*b[40]; t+=(dpint)a[26]*b[39]; t+=(dpint)a[27]*b[38]; t+=(dpint)a[28]*b[37]; t+=(dpint)a[29]*b[36]; t+=(dpint)a[30]*b[35]; t+=(dpint)a[31]*b[34]; t+=(dpint)a[32]*b[33]; t+=(dpint)a[33]*b[32]; t+=(dpint)a[34]*b[31]; t+=(dpint)a[35]*b[30]; t+=(dpint)a[36]*b[29]; t+=(dpint)a[37]*b[28]; t+=(dpint)a[38]*b[27]; t+=(dpint)a[39]*b[26]; t+=(dpint)a[40]*b[25]; t+=(dpint)a[41]*b[24]; t+=(dpint)a[42]*b[23]; t+=(dpint)a[43]*b[22]; t+=(dpint)a[44]*b[21]; t+=(dpint)a[45]*b[20]; t+=(dpint)a[46]*b[19]; t+=(dpint)a[47]*b[18]; t+=(dpint)a[48]*b[17]; t+=(dpint)a[49]*b[16]; t+=(dpint)a[50]*b[15]; t+=(dpint)a[51]*b[14]; t+=(dpint)a[52]*b[13]; t+=(dpint)a[53]*b[12]; t+=(dpint)a[54]*b[11]; t+=(dpint)a[55]*b[10]; t+=(dpint)a[56]*b[9]; t+=(dpint)a[57]*b[8]; t+=(dpint)a[58]*b[7]; t+=(dpint)a[59]*b[6]; t+=(dpint)a[60]*b[5]; t+=(dpint)a[61]*b[4]; t+=(dpint)a[62]*b[3]; t+=(dpint)a[63]*b[2]; t+=(dpint)a[64]*b[1]; t+=(dpint)a[65]*b[0]; spint v65=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[66]; t+=(dpint)a[1]*b[65]; t+=(dpint)a[2]*b[64]; t+=(dpint)a[3]*b[63]; t+=(dpint)a[4]*b[62]; t+=(dpint)a[5]*b[61]; t+=(dpint)a[6]*b[60]; t+=(dpint)a[7]*b[59]; t+=(dpint)a[8]*b[58]; t+=(dpint)a[9]*b[57]; t+=(dpint)a[10]*b[56]; t+=(dpint)a[11]*b[55]; t+=(dpint)a[12]*b[54]; t+=(dpint)a[13]*b[53]; t+=(dpint)a[14]*b[52]; t+=(dpint)a[15]*b[51]; t+=(dpint)a[16]*b[50]; t+=(dpint)a[17]*b[49]; t+=(dpint)a[18]*b[48]; t+=(dpint)a[19]*b[47]; t+=(dpint)a[20]*b[46]; t+=(dpint)a[21]*b[45]; t+=(dpint)a[22]*b[44]; t+=(dpint)a[23]*b[43]; t+=(dpint)a[24]*b[42]; t+=(dpint)a[25]*b[41]; t+=(dpint)a[26]*b[40]; t+=(dpint)a[27]*b[39]; t+=(dpint)a[28]*b[38]; t+=(dpint)a[29]*b[37]; t+=(dpint)a[30]*b[36]; t+=(dpint)a[31]*b[35]; t+=(dpint)a[32]*b[34]; t+=(dpint)a[33]*b[33]; t+=(dpint)a[34]*b[32]; t+=(dpint)a[35]*b[31]; t+=(dpint)a[36]*b[30]; t+=(dpint)a[37]*b[29]; t+=(dpint)a[38]*b[28]; t+=(dpint)a[39]*b[27]; t+=(dpint)a[40]*b[26]; t+=(dpint)a[41]*b[25]; t+=(dpint)a[42]*b[24]; t+=(dpint)a[43]*b[23]; t+=(dpint)a[44]*b[22]; t+=(dpint)a[45]*b[21]; t+=(dpint)a[46]*b[20]; t+=(dpint)a[47]*b[19]; t+=(dpint)a[48]*b[18]; t+=(dpint)a[49]*b[17]; t+=(dpint)a[50]*b[16]; t+=(dpint)a[51]*b[15]; t+=(dpint)a[52]*b[14]; t+=(dpint)a[53]*b[13]; t+=(dpint)a[54]*b[12]; t+=(dpint)a[55]*b[11]; t+=(dpint)a[56]*b[10]; t+=(dpint)a[57]*b[9]; t+=(dpint)a[58]*b[8]; t+=(dpint)a[59]*b[7]; t+=(dpint)a[60]*b[6]; t+=(dpint)a[61]*b[5]; t+=(dpint)a[62]*b[4]; t+=(dpint)a[63]*b[3]; t+=(dpint)a[64]*b[2]; t+=(dpint)a[65]*b[1]; t+=(dpint)a[66]*b[0]; spint v66=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[67]; t+=(dpint)a[1]*b[66]; t+=(dpint)a[2]*b[65]; t+=(dpint)a[3]*b[64]; t+=(dpint)a[4]*b[63]; t+=(dpint)a[5]*b[62]; t+=(dpint)a[6]*b[61]; t+=(dpint)a[7]*b[60]; t+=(dpint)a[8]*b[59]; t+=(dpint)a[9]*b[58]; t+=(dpint)a[10]*b[57]; t+=(dpint)a[11]*b[56]; t+=(dpint)a[12]*b[55]; t+=(dpint)a[13]*b[54]; t+=(dpint)a[14]*b[53]; t+=(dpint)a[15]*b[52]; t+=(dpint)a[16]*b[51]; t+=(dpint)a[17]*b[50]; t+=(dpint)a[18]*b[49]; t+=(dpint)a[19]*b[48]; t+=(dpint)a[20]*b[47]; t+=(dpint)a[21]*b[46]; t+=(dpint)a[22]*b[45]; t+=(dpint)a[23]*b[44]; t+=(dpint)a[24]*b[43]; t+=(dpint)a[25]*b[42]; t+=(dpint)a[26]*b[41]; t+=(dpint)a[27]*b[40]; t+=(dpint)a[28]*b[39]; t+=(dpint)a[29]*b[38]; t+=(dpint)a[30]*b[37]; t+=(dpint)a[31]*b[36]; t+=(dpint)a[32]*b[35]; t+=(dpint)a[33]*b[34]; t+=(dpint)a[34]*b[33]; t+=(dpint)a[35]*b[32]; t+=(dpint)a[36]*b[31]; t+=(dpint)a[37]*b[30]; t+=(dpint)a[38]*b[29]; t+=(dpint)a[39]*b[28]; t+=(dpint)a[40]*b[27]; t+=(dpint)a[41]*b[26]; t+=(dpint)a[42]*b[25]; t+=(dpint)a[43]*b[24]; t+=(dpint)a[44]*b[23]; t+=(dpint)a[45]*b[22]; t+=(dpint)a[46]*b[21]; t+=(dpint)a[47]*b[20]; t+=(dpint)a[48]*b[19]; t+=(dpint)a[49]*b[18]; t+=(dpint)a[50]*b[17]; t+=(dpint)a[51]*b[16]; t+=(dpint)a[52]*b[15]; t+=(dpint)a[53]*b[14]; t+=(dpint)a[54]*b[13]; t+=(dpint)a[55]*b[12]; t+=(dpint)a[56]*b[11]; t+=(dpint)a[57]*b[10]; t+=(dpint)a[58]*b[9]; t+=(dpint)a[59]*b[8]; t+=(dpint)a[60]*b[7]; t+=(dpint)a[61]*b[6]; t+=(dpint)a[62]*b[5]; t+=(dpint)a[63]*b[4]; t+=(dpint)a[64]*b[3]; t+=(dpint)a[65]*b[2]; t+=(dpint)a[66]*b[1]; t+=(dpint)a[67]*b[0]; spint v67=((spint)t & mask);  t>>=60;
	t+=(dpint)a[0]*b[68]; t+=(dpint)a[1]*b[67]; t+=(dpint)a[2]*b[66]; t+=(dpint)a[3]*b[65]; t+=(dpint)a[4]*b[64]; t+=(dpint)a[5]*b[63]; t+=(dpint)a[6]*b[62]; t+=(dpint)a[7]*b[61]; t+=(dpint)a[8]*b[60]; t+=(dpint)a[9]*b[59]; t+=(dpint)a[10]*b[58]; t+=(dpint)a[11]*b[57]; t+=(dpint)a[12]*b[56]; t+=(dpint)a[13]*b[55]; t+=(dpint)a[14]*b[54]; t+=(dpint)a[15]*b[53]; t+=(dpint)a[16]*b[52]; t+=(dpint)a[17]*b[51]; t+=(dpint)a[18]*b[50]; t+=(dpint)a[19]*b[49]; t+=(dpint)a[20]*b[48]; t+=(dpint)a[21]*b[47]; t+=(dpint)a[22]*b[46]; t+=(dpint)a[23]*b[45]; t+=(dpint)a[24]*b[44]; t+=(dpint)a[25]*b[43]; t+=(dpint)a[26]*b[42]; t+=(dpint)a[27]*b[41]; t+=(dpint)a[28]*b[40]; t+=(dpint)a[29]*b[39]; t+=(dpint)a[30]*b[38]; t+=(dpint)a[31]*b[37]; t+=(dpint)a[32]*b[36]; t+=(dpint)a[33]*b[35]; t+=(dpint)a[34]*b[34]; t+=(dpint)a[35]*b[33]; t+=(dpint)a[36]*b[32]; t+=(dpint)a[37]*b[31]; t+=(dpint)a[38]*b[30]; t+=(dpint)a[39]*b[29]; t+=(dpint)a[40]*b[28]; t+=(dpint)a[41]*b[27]; t+=(dpint)a[42]*b[26]; t+=(dpint)a[43]*b[25]; t+=(dpint)a[44]*b[24]; t+=(dpint)a[45]*b[23]; t+=(dpint)a[46]*b[22]; t+=(dpint)a[47]*b[21]; t+=(dpint)a[48]*b[20]; t+=(dpint)a[49]*b[19]; t+=(dpint)a[50]*b[18]; t+=(dpint)a[51]*b[17]; t+=(dpint)a[52]*b[16]; t+=(dpint)a[53]*b[15]; t+=(dpint)a[54]*b[14]; t+=(dpint)a[55]*b[13]; t+=(dpint)a[56]*b[12]; t+=(dpint)a[57]*b[11]; t+=(dpint)a[58]*b[10]; t+=(dpint)a[59]*b[9]; t+=(dpint)a[60]*b[8]; t+=(dpint)a[61]*b[7]; t+=(dpint)a[62]*b[6]; t+=(dpint)a[63]*b[5]; t+=(dpint)a[64]*b[4]; t+=(dpint)a[65]*b[3]; t+=(dpint)a[66]*b[2]; t+=(dpint)a[67]*b[1]; t+=(dpint)a[68]*b[0]; t+=(dpint)v0*(dpint)p68;  spint v68=((spint)t & mask);  t>>=60;
	t+=(dpint)a[1]*b[68]; t+=(dpint)a[2]*b[67]; t+=(dpint)a[3]*b[66]; t+=(dpint)a[4]*b[65]; t+=(dpint)a[5]*b[64]; t+=(dpint)a[6]*b[63]; t+=(dpint)a[7]*b[62]; t+=(dpint)a[8]*b[61]; t+=(dpint)a[9]*b[60]; t+=(dpint)a[10]*b[59]; t+=(dpint)a[11]*b[58]; t+=(dpint)a[12]*b[57]; t+=(dpint)a[13]*b[56]; t+=(dpint)a[14]*b[55]; t+=(dpint)a[15]*b[54]; t+=(dpint)a[16]*b[53]; t+=(dpint)a[17]*b[52]; t+=(dpint)a[18]*b[51]; t+=(dpint)a[19]*b[50]; t+=(dpint)a[20]*b[49]; t+=(dpint)a[21]*b[48]; t+=(dpint)a[22]*b[47]; t+=(dpint)a[23]*b[46]; t+=(dpint)a[24]*b[45]; t+=(dpint)a[25]*b[44]; t+=(dpint)a[26]*b[43]; t+=(dpint)a[27]*b[42]; t+=(dpint)a[28]*b[41]; t+=(dpint)a[29]*b[40]; t+=(dpint)a[30]*b[39]; t+=(dpint)a[31]*b[38]; t+=(dpint)a[32]*b[37]; t+=(dpint)a[33]*b[36]; t+=(dpint)a[34]*b[35]; t+=(dpint)a[35]*b[34]; t+=(dpint)a[36]*b[33]; t+=(dpint)a[37]*b[32]; t+=(dpint)a[38]*b[31]; t+=(dpint)a[39]*b[30]; t+=(dpint)a[40]*b[29]; t+=(dpint)a[41]*b[28]; t+=(dpint)a[42]*b[27]; t+=(dpint)a[43]*b[26]; t+=(dpint)a[44]*b[25]; t+=(dpint)a[45]*b[24]; t+=(dpint)a[46]*b[23]; t+=(dpint)a[47]*b[22]; t+=(dpint)a[48]*b[21]; t+=(dpint)a[49]*b[20]; t+=(dpint)a[50]*b[19]; t+=(dpint)a[51]*b[18]; t+=(dpint)a[52]*b[17]; t+=(dpint)a[53]*b[16]; t+=(dpint)a[54]*b[15]; t+=(dpint)a[55]*b[14]; t+=(dpint)a[56]*b[13]; t+=(dpint)a[57]*b[12]; t+=(dpint)a[58]*b[11]; t+=(dpint)a[59]*b[10]; t+=(dpint)a[60]*b[9]; t+=(dpint)a[61]*b[8]; t+=(dpint)a[62]*b[7]; t+=(dpint)a[63]*b[6]; t+=(dpint)a[64]*b[5]; t+=(dpint)a[65]*b[4]; t+=(dpint)a[66]*b[3]; t+=(dpint)a[67]*b[2]; t+=(dpint)a[68]*b[1]; t+=(dpint)v1*(dpint)p68;  c[0]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[2]*b[68]; t+=(dpint)a[3]*b[67]; t+=(dpint)a[4]*b[66]; t+=(dpint)a[5]*b[65]; t+=(dpint)a[6]*b[64]; t+=(dpint)a[7]*b[63]; t+=(dpint)a[8]*b[62]; t+=(dpint)a[9]*b[61]; t+=(dpint)a[10]*b[60]; t+=(dpint)a[11]*b[59]; t+=(dpint)a[12]*b[58]; t+=(dpint)a[13]*b[57]; t+=(dpint)a[14]*b[56]; t+=(dpint)a[15]*b[55]; t+=(dpint)a[16]*b[54]; t+=(dpint)a[17]*b[53]; t+=(dpint)a[18]*b[52]; t+=(dpint)a[19]*b[51]; t+=(dpint)a[20]*b[50]; t+=(dpint)a[21]*b[49]; t+=(dpint)a[22]*b[48]; t+=(dpint)a[23]*b[47]; t+=(dpint)a[24]*b[46]; t+=(dpint)a[25]*b[45]; t+=(dpint)a[26]*b[44]; t+=(dpint)a[27]*b[43]; t+=(dpint)a[28]*b[42]; t+=(dpint)a[29]*b[41]; t+=(dpint)a[30]*b[40]; t+=(dpint)a[31]*b[39]; t+=(dpint)a[32]*b[38]; t+=(dpint)a[33]*b[37]; t+=(dpint)a[34]*b[36]; t+=(dpint)a[35]*b[35]; t+=(dpint)a[36]*b[34]; t+=(dpint)a[37]*b[33]; t+=(dpint)a[38]*b[32]; t+=(dpint)a[39]*b[31]; t+=(dpint)a[40]*b[30]; t+=(dpint)a[41]*b[29]; t+=(dpint)a[42]*b[28]; t+=(dpint)a[43]*b[27]; t+=(dpint)a[44]*b[26]; t+=(dpint)a[45]*b[25]; t+=(dpint)a[46]*b[24]; t+=(dpint)a[47]*b[23]; t+=(dpint)a[48]*b[22]; t+=(dpint)a[49]*b[21]; t+=(dpint)a[50]*b[20]; t+=(dpint)a[51]*b[19]; t+=(dpint)a[52]*b[18]; t+=(dpint)a[53]*b[17]; t+=(dpint)a[54]*b[16]; t+=(dpint)a[55]*b[15]; t+=(dpint)a[56]*b[14]; t+=(dpint)a[57]*b[13]; t+=(dpint)a[58]*b[12]; t+=(dpint)a[59]*b[11]; t+=(dpint)a[60]*b[10]; t+=(dpint)a[61]*b[9]; t+=(dpint)a[62]*b[8]; t+=(dpint)a[63]*b[7]; t+=(dpint)a[64]*b[6]; t+=(dpint)a[65]*b[5]; t+=(dpint)a[66]*b[4]; t+=(dpint)a[67]*b[3]; t+=(dpint)a[68]*b[2]; t+=(dpint)v2*(dpint)p68;  c[1]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[3]*b[68]; t+=(dpint)a[4]*b[67]; t+=(dpint)a[5]*b[66]; t+=(dpint)a[6]*b[65]; t+=(dpint)a[7]*b[64]; t+=(dpint)a[8]*b[63]; t+=(dpint)a[9]*b[62]; t+=(dpint)a[10]*b[61]; t+=(dpint)a[11]*b[60]; t+=(dpint)a[12]*b[59]; t+=(dpint)a[13]*b[58]; t+=(dpint)a[14]*b[57]; t+=(dpint)a[15]*b[56]; t+=(dpint)a[16]*b[55]; t+=(dpint)a[17]*b[54]; t+=(dpint)a[18]*b[53]; t+=(dpint)a[19]*b[52]; t+=(dpint)a[20]*b[51]; t+=(dpint)a[21]*b[50]; t+=(dpint)a[22]*b[49]; t+=(dpint)a[23]*b[48]; t+=(dpint)a[24]*b[47]; t+=(dpint)a[25]*b[46]; t+=(dpint)a[26]*b[45]; t+=(dpint)a[27]*b[44]; t+=(dpint)a[28]*b[43]; t+=(dpint)a[29]*b[42]; t+=(dpint)a[30]*b[41]; t+=(dpint)a[31]*b[40]; t+=(dpint)a[32]*b[39]; t+=(dpint)a[33]*b[38]; t+=(dpint)a[34]*b[37]; t+=(dpint)a[35]*b[36]; t+=(dpint)a[36]*b[35]; t+=(dpint)a[37]*b[34]; t+=(dpint)a[38]*b[33]; t+=(dpint)a[39]*b[32]; t+=(dpint)a[40]*b[31]; t+=(dpint)a[41]*b[30]; t+=(dpint)a[42]*b[29]; t+=(dpint)a[43]*b[28]; t+=(dpint)a[44]*b[27]; t+=(dpint)a[45]*b[26]; t+=(dpint)a[46]*b[25]; t+=(dpint)a[47]*b[24]; t+=(dpint)a[48]*b[23]; t+=(dpint)a[49]*b[22]; t+=(dpint)a[50]*b[21]; t+=(dpint)a[51]*b[20]; t+=(dpint)a[52]*b[19]; t+=(dpint)a[53]*b[18]; t+=(dpint)a[54]*b[17]; t+=(dpint)a[55]*b[16]; t+=(dpint)a[56]*b[15]; t+=(dpint)a[57]*b[14]; t+=(dpint)a[58]*b[13]; t+=(dpint)a[59]*b[12]; t+=(dpint)a[60]*b[11]; t+=(dpint)a[61]*b[10]; t+=(dpint)a[62]*b[9]; t+=(dpint)a[63]*b[8]; t+=(dpint)a[64]*b[7]; t+=(dpint)a[65]*b[6]; t+=(dpint)a[66]*b[5]; t+=(dpint)a[67]*b[4]; t+=(dpint)a[68]*b[3]; t+=(dpint)v3*(dpint)p68;  c[2]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[4]*b[68]; t+=(dpint)a[5]*b[67]; t+=(dpint)a[6]*b[66]; t+=(dpint)a[7]*b[65]; t+=(dpint)a[8]*b[64]; t+=(dpint)a[9]*b[63]; t+=(dpint)a[10]*b[62]; t+=(dpint)a[11]*b[61]; t+=(dpint)a[12]*b[60]; t+=(dpint)a[13]*b[59]; t+=(dpint)a[14]*b[58]; t+=(dpint)a[15]*b[57]; t+=(dpint)a[16]*b[56]; t+=(dpint)a[17]*b[55]; t+=(dpint)a[18]*b[54]; t+=(dpint)a[19]*b[53]; t+=(dpint)a[20]*b[52]; t+=(dpint)a[21]*b[51]; t+=(dpint)a[22]*b[50]; t+=(dpint)a[23]*b[49]; t+=(dpint)a[24]*b[48]; t+=(dpint)a[25]*b[47]; t+=(dpint)a[26]*b[46]; t+=(dpint)a[27]*b[45]; t+=(dpint)a[28]*b[44]; t+=(dpint)a[29]*b[43]; t+=(dpint)a[30]*b[42]; t+=(dpint)a[31]*b[41]; t+=(dpint)a[32]*b[40]; t+=(dpint)a[33]*b[39]; t+=(dpint)a[34]*b[38]; t+=(dpint)a[35]*b[37]; t+=(dpint)a[36]*b[36]; t+=(dpint)a[37]*b[35]; t+=(dpint)a[38]*b[34]; t+=(dpint)a[39]*b[33]; t+=(dpint)a[40]*b[32]; t+=(dpint)a[41]*b[31]; t+=(dpint)a[42]*b[30]; t+=(dpint)a[43]*b[29]; t+=(dpint)a[44]*b[28]; t+=(dpint)a[45]*b[27]; t+=(dpint)a[46]*b[26]; t+=(dpint)a[47]*b[25]; t+=(dpint)a[48]*b[24]; t+=(dpint)a[49]*b[23]; t+=(dpint)a[50]*b[22]; t+=(dpint)a[51]*b[21]; t+=(dpint)a[52]*b[20]; t+=(dpint)a[53]*b[19]; t+=(dpint)a[54]*b[18]; t+=(dpint)a[55]*b[17]; t+=(dpint)a[56]*b[16]; t+=(dpint)a[57]*b[15]; t+=(dpint)a[58]*b[14]; t+=(dpint)a[59]*b[13]; t+=(dpint)a[60]*b[12]; t+=(dpint)a[61]*b[11]; t+=(dpint)a[62]*b[10]; t+=(dpint)a[63]*b[9]; t+=(dpint)a[64]*b[8]; t+=(dpint)a[65]*b[7]; t+=(dpint)a[66]*b[6]; t+=(dpint)a[67]*b[5]; t+=(dpint)a[68]*b[4]; t+=(dpint)v4*(dpint)p68;  c[3]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[5]*b[68]; t+=(dpint)a[6]*b[67]; t+=(dpint)a[7]*b[66]; t+=(dpint)a[8]*b[65]; t+=(dpint)a[9]*b[64]; t+=(dpint)a[10]*b[63]; t+=(dpint)a[11]*b[62]; t+=(dpint)a[12]*b[61]; t+=(dpint)a[13]*b[60]; t+=(dpint)a[14]*b[59]; t+=(dpint)a[15]*b[58]; t+=(dpint)a[16]*b[57]; t+=(dpint)a[17]*b[56]; t+=(dpint)a[18]*b[55]; t+=(dpint)a[19]*b[54]; t+=(dpint)a[20]*b[53]; t+=(dpint)a[21]*b[52]; t+=(dpint)a[22]*b[51]; t+=(dpint)a[23]*b[50]; t+=(dpint)a[24]*b[49]; t+=(dpint)a[25]*b[48]; t+=(dpint)a[26]*b[47]; t+=(dpint)a[27]*b[46]; t+=(dpint)a[28]*b[45]; t+=(dpint)a[29]*b[44]; t+=(dpint)a[30]*b[43]; t+=(dpint)a[31]*b[42]; t+=(dpint)a[32]*b[41]; t+=(dpint)a[33]*b[40]; t+=(dpint)a[34]*b[39]; t+=(dpint)a[35]*b[38]; t+=(dpint)a[36]*b[37]; t+=(dpint)a[37]*b[36]; t+=(dpint)a[38]*b[35]; t+=(dpint)a[39]*b[34]; t+=(dpint)a[40]*b[33]; t+=(dpint)a[41]*b[32]; t+=(dpint)a[42]*b[31]; t+=(dpint)a[43]*b[30]; t+=(dpint)a[44]*b[29]; t+=(dpint)a[45]*b[28]; t+=(dpint)a[46]*b[27]; t+=(dpint)a[47]*b[26]; t+=(dpint)a[48]*b[25]; t+=(dpint)a[49]*b[24]; t+=(dpint)a[50]*b[23]; t+=(dpint)a[51]*b[22]; t+=(dpint)a[52]*b[21]; t+=(dpint)a[53]*b[20]; t+=(dpint)a[54]*b[19]; t+=(dpint)a[55]*b[18]; t+=(dpint)a[56]*b[17]; t+=(dpint)a[57]*b[16]; t+=(dpint)a[58]*b[15]; t+=(dpint)a[59]*b[14]; t+=(dpint)a[60]*b[13]; t+=(dpint)a[61]*b[12]; t+=(dpint)a[62]*b[11]; t+=(dpint)a[63]*b[10]; t+=(dpint)a[64]*b[9]; t+=(dpint)a[65]*b[8]; t+=(dpint)a[66]*b[7]; t+=(dpint)a[67]*b[6]; t+=(dpint)a[68]*b[5]; t+=(dpint)v5*(dpint)p68;  c[4]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[6]*b[68]; t+=(dpint)a[7]*b[67]; t+=(dpint)a[8]*b[66]; t+=(dpint)a[9]*b[65]; t+=(dpint)a[10]*b[64]; t+=(dpint)a[11]*b[63]; t+=(dpint)a[12]*b[62]; t+=(dpint)a[13]*b[61]; t+=(dpint)a[14]*b[60]; t+=(dpint)a[15]*b[59]; t+=(dpint)a[16]*b[58]; t+=(dpint)a[17]*b[57]; t+=(dpint)a[18]*b[56]; t+=(dpint)a[19]*b[55]; t+=(dpint)a[20]*b[54]; t+=(dpint)a[21]*b[53]; t+=(dpint)a[22]*b[52]; t+=(dpint)a[23]*b[51]; t+=(dpint)a[24]*b[50]; t+=(dpint)a[25]*b[49]; t+=(dpint)a[26]*b[48]; t+=(dpint)a[27]*b[47]; t+=(dpint)a[28]*b[46]; t+=(dpint)a[29]*b[45]; t+=(dpint)a[30]*b[44]; t+=(dpint)a[31]*b[43]; t+=(dpint)a[32]*b[42]; t+=(dpint)a[33]*b[41]; t+=(dpint)a[34]*b[40]; t+=(dpint)a[35]*b[39]; t+=(dpint)a[36]*b[38]; t+=(dpint)a[37]*b[37]; t+=(dpint)a[38]*b[36]; t+=(dpint)a[39]*b[35]; t+=(dpint)a[40]*b[34]; t+=(dpint)a[41]*b[33]; t+=(dpint)a[42]*b[32]; t+=(dpint)a[43]*b[31]; t+=(dpint)a[44]*b[30]; t+=(dpint)a[45]*b[29]; t+=(dpint)a[46]*b[28]; t+=(dpint)a[47]*b[27]; t+=(dpint)a[48]*b[26]; t+=(dpint)a[49]*b[25]; t+=(dpint)a[50]*b[24]; t+=(dpint)a[51]*b[23]; t+=(dpint)a[52]*b[22]; t+=(dpint)a[53]*b[21]; t+=(dpint)a[54]*b[20]; t+=(dpint)a[55]*b[19]; t+=(dpint)a[56]*b[18]; t+=(dpint)a[57]*b[17]; t+=(dpint)a[58]*b[16]; t+=(dpint)a[59]*b[15]; t+=(dpint)a[60]*b[14]; t+=(dpint)a[61]*b[13]; t+=(dpint)a[62]*b[12]; t+=(dpint)a[63]*b[11]; t+=(dpint)a[64]*b[10]; t+=(dpint)a[65]*b[9]; t+=(dpint)a[66]*b[8]; t+=(dpint)a[67]*b[7]; t+=(dpint)a[68]*b[6]; t+=(dpint)v6*(dpint)p68;  c[5]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[7]*b[68]; t+=(dpint)a[8]*b[67]; t+=(dpint)a[9]*b[66]; t+=(dpint)a[10]*b[65]; t+=(dpint)a[11]*b[64]; t+=(dpint)a[12]*b[63]; t+=(dpint)a[13]*b[62]; t+=(dpint)a[14]*b[61]; t+=(dpint)a[15]*b[60]; t+=(dpint)a[16]*b[59]; t+=(dpint)a[17]*b[58]; t+=(dpint)a[18]*b[57]; t+=(dpint)a[19]*b[56]; t+=(dpint)a[20]*b[55]; t+=(dpint)a[21]*b[54]; t+=(dpint)a[22]*b[53]; t+=(dpint)a[23]*b[52]; t+=(dpint)a[24]*b[51]; t+=(dpint)a[25]*b[50]; t+=(dpint)a[26]*b[49]; t+=(dpint)a[27]*b[48]; t+=(dpint)a[28]*b[47]; t+=(dpint)a[29]*b[46]; t+=(dpint)a[30]*b[45]; t+=(dpint)a[31]*b[44]; t+=(dpint)a[32]*b[43]; t+=(dpint)a[33]*b[42]; t+=(dpint)a[34]*b[41]; t+=(dpint)a[35]*b[40]; t+=(dpint)a[36]*b[39]; t+=(dpint)a[37]*b[38]; t+=(dpint)a[38]*b[37]; t+=(dpint)a[39]*b[36]; t+=(dpint)a[40]*b[35]; t+=(dpint)a[41]*b[34]; t+=(dpint)a[42]*b[33]; t+=(dpint)a[43]*b[32]; t+=(dpint)a[44]*b[31]; t+=(dpint)a[45]*b[30]; t+=(dpint)a[46]*b[29]; t+=(dpint)a[47]*b[28]; t+=(dpint)a[48]*b[27]; t+=(dpint)a[49]*b[26]; t+=(dpint)a[50]*b[25]; t+=(dpint)a[51]*b[24]; t+=(dpint)a[52]*b[23]; t+=(dpint)a[53]*b[22]; t+=(dpint)a[54]*b[21]; t+=(dpint)a[55]*b[20]; t+=(dpint)a[56]*b[19]; t+=(dpint)a[57]*b[18]; t+=(dpint)a[58]*b[17]; t+=(dpint)a[59]*b[16]; t+=(dpint)a[60]*b[15]; t+=(dpint)a[61]*b[14]; t+=(dpint)a[62]*b[13]; t+=(dpint)a[63]*b[12]; t+=(dpint)a[64]*b[11]; t+=(dpint)a[65]*b[10]; t+=(dpint)a[66]*b[9]; t+=(dpint)a[67]*b[8]; t+=(dpint)a[68]*b[7]; t+=(dpint)v7*(dpint)p68;  c[6]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[8]*b[68]; t+=(dpint)a[9]*b[67]; t+=(dpint)a[10]*b[66]; t+=(dpint)a[11]*b[65]; t+=(dpint)a[12]*b[64]; t+=(dpint)a[13]*b[63]; t+=(dpint)a[14]*b[62]; t+=(dpint)a[15]*b[61]; t+=(dpint)a[16]*b[60]; t+=(dpint)a[17]*b[59]; t+=(dpint)a[18]*b[58]; t+=(dpint)a[19]*b[57]; t+=(dpint)a[20]*b[56]; t+=(dpint)a[21]*b[55]; t+=(dpint)a[22]*b[54]; t+=(dpint)a[23]*b[53]; t+=(dpint)a[24]*b[52]; t+=(dpint)a[25]*b[51]; t+=(dpint)a[26]*b[50]; t+=(dpint)a[27]*b[49]; t+=(dpint)a[28]*b[48]; t+=(dpint)a[29]*b[47]; t+=(dpint)a[30]*b[46]; t+=(dpint)a[31]*b[45]; t+=(dpint)a[32]*b[44]; t+=(dpint)a[33]*b[43]; t+=(dpint)a[34]*b[42]; t+=(dpint)a[35]*b[41]; t+=(dpint)a[36]*b[40]; t+=(dpint)a[37]*b[39]; t+=(dpint)a[38]*b[38]; t+=(dpint)a[39]*b[37]; t+=(dpint)a[40]*b[36]; t+=(dpint)a[41]*b[35]; t+=(dpint)a[42]*b[34]; t+=(dpint)a[43]*b[33]; t+=(dpint)a[44]*b[32]; t+=(dpint)a[45]*b[31]; t+=(dpint)a[46]*b[30]; t+=(dpint)a[47]*b[29]; t+=(dpint)a[48]*b[28]; t+=(dpint)a[49]*b[27]; t+=(dpint)a[50]*b[26]; t+=(dpint)a[51]*b[25]; t+=(dpint)a[52]*b[24]; t+=(dpint)a[53]*b[23]; t+=(dpint)a[54]*b[22]; t+=(dpint)a[55]*b[21]; t+=(dpint)a[56]*b[20]; t+=(dpint)a[57]*b[19]; t+=(dpint)a[58]*b[18]; t+=(dpint)a[59]*b[17]; t+=(dpint)a[60]*b[16]; t+=(dpint)a[61]*b[15]; t+=(dpint)a[62]*b[14]; t+=(dpint)a[63]*b[13]; t+=(dpint)a[64]*b[12]; t+=(dpint)a[65]*b[11]; t+=(dpint)a[66]*b[10]; t+=(dpint)a[67]*b[9]; t+=(dpint)a[68]*b[8]; t+=(dpint)v8*(dpint)p68;  c[7]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[9]*b[68]; t+=(dpint)a[10]*b[67]; t+=(dpint)a[11]*b[66]; t+=(dpint)a[12]*b[65]; t+=(dpint)a[13]*b[64]; t+=(dpint)a[14]*b[63]; t+=(dpint)a[15]*b[62]; t+=(dpint)a[16]*b[61]; t+=(dpint)a[17]*b[60]; t+=(dpint)a[18]*b[59]; t+=(dpint)a[19]*b[58]; t+=(dpint)a[20]*b[57]; t+=(dpint)a[21]*b[56]; t+=(dpint)a[22]*b[55]; t+=(dpint)a[23]*b[54]; t+=(dpint)a[24]*b[53]; t+=(dpint)a[25]*b[52]; t+=(dpint)a[26]*b[51]; t+=(dpint)a[27]*b[50]; t+=(dpint)a[28]*b[49]; t+=(dpint)a[29]*b[48]; t+=(dpint)a[30]*b[47]; t+=(dpint)a[31]*b[46]; t+=(dpint)a[32]*b[45]; t+=(dpint)a[33]*b[44]; t+=(dpint)a[34]*b[43]; t+=(dpint)a[35]*b[42]; t+=(dpint)a[36]*b[41]; t+=(dpint)a[37]*b[40]; t+=(dpint)a[38]*b[39]; t+=(dpint)a[39]*b[38]; t+=(dpint)a[40]*b[37]; t+=(dpint)a[41]*b[36]; t+=(dpint)a[42]*b[35]; t+=(dpint)a[43]*b[34]; t+=(dpint)a[44]*b[33]; t+=(dpint)a[45]*b[32]; t+=(dpint)a[46]*b[31]; t+=(dpint)a[47]*b[30]; t+=(dpint)a[48]*b[29]; t+=(dpint)a[49]*b[28]; t+=(dpint)a[50]*b[27]; t+=(dpint)a[51]*b[26]; t+=(dpint)a[52]*b[25]; t+=(dpint)a[53]*b[24]; t+=(dpint)a[54]*b[23]; t+=(dpint)a[55]*b[22]; t+=(dpint)a[56]*b[21]; t+=(dpint)a[57]*b[20]; t+=(dpint)a[58]*b[19]; t+=(dpint)a[59]*b[18]; t+=(dpint)a[60]*b[17]; t+=(dpint)a[61]*b[16]; t+=(dpint)a[62]*b[15]; t+=(dpint)a[63]*b[14]; t+=(dpint)a[64]*b[13]; t+=(dpint)a[65]*b[12]; t+=(dpint)a[66]*b[11]; t+=(dpint)a[67]*b[10]; t+=(dpint)a[68]*b[9]; t+=(dpint)v9*(dpint)p68;  c[8]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[10]*b[68]; t+=(dpint)a[11]*b[67]; t+=(dpint)a[12]*b[66]; t+=(dpint)a[13]*b[65]; t+=(dpint)a[14]*b[64]; t+=(dpint)a[15]*b[63]; t+=(dpint)a[16]*b[62]; t+=(dpint)a[17]*b[61]; t+=(dpint)a[18]*b[60]; t+=(dpint)a[19]*b[59]; t+=(dpint)a[20]*b[58]; t+=(dpint)a[21]*b[57]; t+=(dpint)a[22]*b[56]; t+=(dpint)a[23]*b[55]; t+=(dpint)a[24]*b[54]; t+=(dpint)a[25]*b[53]; t+=(dpint)a[26]*b[52]; t+=(dpint)a[27]*b[51]; t+=(dpint)a[28]*b[50]; t+=(dpint)a[29]*b[49]; t+=(dpint)a[30]*b[48]; t+=(dpint)a[31]*b[47]; t+=(dpint)a[32]*b[46]; t+=(dpint)a[33]*b[45]; t+=(dpint)a[34]*b[44]; t+=(dpint)a[35]*b[43]; t+=(dpint)a[36]*b[42]; t+=(dpint)a[37]*b[41]; t+=(dpint)a[38]*b[40]; t+=(dpint)a[39]*b[39]; t+=(dpint)a[40]*b[38]; t+=(dpint)a[41]*b[37]; t+=(dpint)a[42]*b[36]; t+=(dpint)a[43]*b[35]; t+=(dpint)a[44]*b[34]; t+=(dpint)a[45]*b[33]; t+=(dpint)a[46]*b[32]; t+=(dpint)a[47]*b[31]; t+=(dpint)a[48]*b[30]; t+=(dpint)a[49]*b[29]; t+=(dpint)a[50]*b[28]; t+=(dpint)a[51]*b[27]; t+=(dpint)a[52]*b[26]; t+=(dpint)a[53]*b[25]; t+=(dpint)a[54]*b[24]; t+=(dpint)a[55]*b[23]; t+=(dpint)a[56]*b[22]; t+=(dpint)a[57]*b[21]; t+=(dpint)a[58]*b[20]; t+=(dpint)a[59]*b[19]; t+=(dpint)a[60]*b[18]; t+=(dpint)a[61]*b[17]; t+=(dpint)a[62]*b[16]; t+=(dpint)a[63]*b[15]; t+=(dpint)a[64]*b[14]; t+=(dpint)a[65]*b[13]; t+=(dpint)a[66]*b[12]; t+=(dpint)a[67]*b[11]; t+=(dpint)a[68]*b[10]; t+=(dpint)v10*(dpint)p68;  c[9]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[11]*b[68]; t+=(dpint)a[12]*b[67]; t+=(dpint)a[13]*b[66]; t+=(dpint)a[14]*b[65]; t+=(dpint)a[15]*b[64]; t+=(dpint)a[16]*b[63]; t+=(dpint)a[17]*b[62]; t+=(dpint)a[18]*b[61]; t+=(dpint)a[19]*b[60]; t+=(dpint)a[20]*b[59]; t+=(dpint)a[21]*b[58]; t+=(dpint)a[22]*b[57]; t+=(dpint)a[23]*b[56]; t+=(dpint)a[24]*b[55]; t+=(dpint)a[25]*b[54]; t+=(dpint)a[26]*b[53]; t+=(dpint)a[27]*b[52]; t+=(dpint)a[28]*b[51]; t+=(dpint)a[29]*b[50]; t+=(dpint)a[30]*b[49]; t+=(dpint)a[31]*b[48]; t+=(dpint)a[32]*b[47]; t+=(dpint)a[33]*b[46]; t+=(dpint)a[34]*b[45]; t+=(dpint)a[35]*b[44]; t+=(dpint)a[36]*b[43]; t+=(dpint)a[37]*b[42]; t+=(dpint)a[38]*b[41]; t+=(dpint)a[39]*b[40]; t+=(dpint)a[40]*b[39]; t+=(dpint)a[41]*b[38]; t+=(dpint)a[42]*b[37]; t+=(dpint)a[43]*b[36]; t+=(dpint)a[44]*b[35]; t+=(dpint)a[45]*b[34]; t+=(dpint)a[46]*b[33]; t+=(dpint)a[47]*b[32]; t+=(dpint)a[48]*b[31]; t+=(dpint)a[49]*b[30]; t+=(dpint)a[50]*b[29]; t+=(dpint)a[51]*b[28]; t+=(dpint)a[52]*b[27]; t+=(dpint)a[53]*b[26]; t+=(dpint)a[54]*b[25]; t+=(dpint)a[55]*b[24]; t+=(dpint)a[56]*b[23]; t+=(dpint)a[57]*b[22]; t+=(dpint)a[58]*b[21]; t+=(dpint)a[59]*b[20]; t+=(dpint)a[60]*b[19]; t+=(dpint)a[61]*b[18]; t+=(dpint)a[62]*b[17]; t+=(dpint)a[63]*b[16]; t+=(dpint)a[64]*b[15]; t+=(dpint)a[65]*b[14]; t+=(dpint)a[66]*b[13]; t+=(dpint)a[67]*b[12]; t+=(dpint)a[68]*b[11]; t+=(dpint)v11*(dpint)p68;  c[10]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[12]*b[68]; t+=(dpint)a[13]*b[67]; t+=(dpint)a[14]*b[66]; t+=(dpint)a[15]*b[65]; t+=(dpint)a[16]*b[64]; t+=(dpint)a[17]*b[63]; t+=(dpint)a[18]*b[62]; t+=(dpint)a[19]*b[61]; t+=(dpint)a[20]*b[60]; t+=(dpint)a[21]*b[59]; t+=(dpint)a[22]*b[58]; t+=(dpint)a[23]*b[57]; t+=(dpint)a[24]*b[56]; t+=(dpint)a[25]*b[55]; t+=(dpint)a[26]*b[54]; t+=(dpint)a[27]*b[53]; t+=(dpint)a[28]*b[52]; t+=(dpint)a[29]*b[51]; t+=(dpint)a[30]*b[50]; t+=(dpint)a[31]*b[49]; t+=(dpint)a[32]*b[48]; t+=(dpint)a[33]*b[47]; t+=(dpint)a[34]*b[46]; t+=(dpint)a[35]*b[45]; t+=(dpint)a[36]*b[44]; t+=(dpint)a[37]*b[43]; t+=(dpint)a[38]*b[42]; t+=(dpint)a[39]*b[41]; t+=(dpint)a[40]*b[40]; t+=(dpint)a[41]*b[39]; t+=(dpint)a[42]*b[38]; t+=(dpint)a[43]*b[37]; t+=(dpint)a[44]*b[36]; t+=(dpint)a[45]*b[35]; t+=(dpint)a[46]*b[34]; t+=(dpint)a[47]*b[33]; t+=(dpint)a[48]*b[32]; t+=(dpint)a[49]*b[31]; t+=(dpint)a[50]*b[30]; t+=(dpint)a[51]*b[29]; t+=(dpint)a[52]*b[28]; t+=(dpint)a[53]*b[27]; t+=(dpint)a[54]*b[26]; t+=(dpint)a[55]*b[25]; t+=(dpint)a[56]*b[24]; t+=(dpint)a[57]*b[23]; t+=(dpint)a[58]*b[22]; t+=(dpint)a[59]*b[21]; t+=(dpint)a[60]*b[20]; t+=(dpint)a[61]*b[19]; t+=(dpint)a[62]*b[18]; t+=(dpint)a[63]*b[17]; t+=(dpint)a[64]*b[16]; t+=(dpint)a[65]*b[15]; t+=(dpint)a[66]*b[14]; t+=(dpint)a[67]*b[13]; t+=(dpint)a[68]*b[12]; t+=(dpint)v12*(dpint)p68;  c[11]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[13]*b[68]; t+=(dpint)a[14]*b[67]; t+=(dpint)a[15]*b[66]; t+=(dpint)a[16]*b[65]; t+=(dpint)a[17]*b[64]; t+=(dpint)a[18]*b[63]; t+=(dpint)a[19]*b[62]; t+=(dpint)a[20]*b[61]; t+=(dpint)a[21]*b[60]; t+=(dpint)a[22]*b[59]; t+=(dpint)a[23]*b[58]; t+=(dpint)a[24]*b[57]; t+=(dpint)a[25]*b[56]; t+=(dpint)a[26]*b[55]; t+=(dpint)a[27]*b[54]; t+=(dpint)a[28]*b[53]; t+=(dpint)a[29]*b[52]; t+=(dpint)a[30]*b[51]; t+=(dpint)a[31]*b[50]; t+=(dpint)a[32]*b[49]; t+=(dpint)a[33]*b[48]; t+=(dpint)a[34]*b[47]; t+=(dpint)a[35]*b[46]; t+=(dpint)a[36]*b[45]; t+=(dpint)a[37]*b[44]; t+=(dpint)a[38]*b[43]; t+=(dpint)a[39]*b[42]; t+=(dpint)a[40]*b[41]; t+=(dpint)a[41]*b[40]; t+=(dpint)a[42]*b[39]; t+=(dpint)a[43]*b[38]; t+=(dpint)a[44]*b[37]; t+=(dpint)a[45]*b[36]; t+=(dpint)a[46]*b[35]; t+=(dpint)a[47]*b[34]; t+=(dpint)a[48]*b[33]; t+=(dpint)a[49]*b[32]; t+=(dpint)a[50]*b[31]; t+=(dpint)a[51]*b[30]; t+=(dpint)a[52]*b[29]; t+=(dpint)a[53]*b[28]; t+=(dpint)a[54]*b[27]; t+=(dpint)a[55]*b[26]; t+=(dpint)a[56]*b[25]; t+=(dpint)a[57]*b[24]; t+=(dpint)a[58]*b[23]; t+=(dpint)a[59]*b[22]; t+=(dpint)a[60]*b[21]; t+=(dpint)a[61]*b[20]; t+=(dpint)a[62]*b[19]; t+=(dpint)a[63]*b[18]; t+=(dpint)a[64]*b[17]; t+=(dpint)a[65]*b[16]; t+=(dpint)a[66]*b[15]; t+=(dpint)a[67]*b[14]; t+=(dpint)a[68]*b[13]; t+=(dpint)v13*(dpint)p68;  c[12]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[14]*b[68]; t+=(dpint)a[15]*b[67]; t+=(dpint)a[16]*b[66]; t+=(dpint)a[17]*b[65]; t+=(dpint)a[18]*b[64]; t+=(dpint)a[19]*b[63]; t+=(dpint)a[20]*b[62]; t+=(dpint)a[21]*b[61]; t+=(dpint)a[22]*b[60]; t+=(dpint)a[23]*b[59]; t+=(dpint)a[24]*b[58]; t+=(dpint)a[25]*b[57]; t+=(dpint)a[26]*b[56]; t+=(dpint)a[27]*b[55]; t+=(dpint)a[28]*b[54]; t+=(dpint)a[29]*b[53]; t+=(dpint)a[30]*b[52]; t+=(dpint)a[31]*b[51]; t+=(dpint)a[32]*b[50]; t+=(dpint)a[33]*b[49]; t+=(dpint)a[34]*b[48]; t+=(dpint)a[35]*b[47]; t+=(dpint)a[36]*b[46]; t+=(dpint)a[37]*b[45]; t+=(dpint)a[38]*b[44]; t+=(dpint)a[39]*b[43]; t+=(dpint)a[40]*b[42]; t+=(dpint)a[41]*b[41]; t+=(dpint)a[42]*b[40]; t+=(dpint)a[43]*b[39]; t+=(dpint)a[44]*b[38]; t+=(dpint)a[45]*b[37]; t+=(dpint)a[46]*b[36]; t+=(dpint)a[47]*b[35]; t+=(dpint)a[48]*b[34]; t+=(dpint)a[49]*b[33]; t+=(dpint)a[50]*b[32]; t+=(dpint)a[51]*b[31]; t+=(dpint)a[52]*b[30]; t+=(dpint)a[53]*b[29]; t+=(dpint)a[54]*b[28]; t+=(dpint)a[55]*b[27]; t+=(dpint)a[56]*b[26]; t+=(dpint)a[57]*b[25]; t+=(dpint)a[58]*b[24]; t+=(dpint)a[59]*b[23]; t+=(dpint)a[60]*b[22]; t+=(dpint)a[61]*b[21]; t+=(dpint)a[62]*b[20]; t+=(dpint)a[63]*b[19]; t+=(dpint)a[64]*b[18]; t+=(dpint)a[65]*b[17]; t+=(dpint)a[66]*b[16]; t+=(dpint)a[67]*b[15]; t+=(dpint)a[68]*b[14]; t+=(dpint)v14*(dpint)p68;  c[13]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[15]*b[68]; t+=(dpint)a[16]*b[67]; t+=(dpint)a[17]*b[66]; t+=(dpint)a[18]*b[65]; t+=(dpint)a[19]*b[64]; t+=(dpint)a[20]*b[63]; t+=(dpint)a[21]*b[62]; t+=(dpint)a[22]*b[61]; t+=(dpint)a[23]*b[60]; t+=(dpint)a[24]*b[59]; t+=(dpint)a[25]*b[58]; t+=(dpint)a[26]*b[57]; t+=(dpint)a[27]*b[56]; t+=(dpint)a[28]*b[55]; t+=(dpint)a[29]*b[54]; t+=(dpint)a[30]*b[53]; t+=(dpint)a[31]*b[52]; t+=(dpint)a[32]*b[51]; t+=(dpint)a[33]*b[50]; t+=(dpint)a[34]*b[49]; t+=(dpint)a[35]*b[48]; t+=(dpint)a[36]*b[47]; t+=(dpint)a[37]*b[46]; t+=(dpint)a[38]*b[45]; t+=(dpint)a[39]*b[44]; t+=(dpint)a[40]*b[43]; t+=(dpint)a[41]*b[42]; t+=(dpint)a[42]*b[41]; t+=(dpint)a[43]*b[40]; t+=(dpint)a[44]*b[39]; t+=(dpint)a[45]*b[38]; t+=(dpint)a[46]*b[37]; t+=(dpint)a[47]*b[36]; t+=(dpint)a[48]*b[35]; t+=(dpint)a[49]*b[34]; t+=(dpint)a[50]*b[33]; t+=(dpint)a[51]*b[32]; t+=(dpint)a[52]*b[31]; t+=(dpint)a[53]*b[30]; t+=(dpint)a[54]*b[29]; t+=(dpint)a[55]*b[28]; t+=(dpint)a[56]*b[27]; t+=(dpint)a[57]*b[26]; t+=(dpint)a[58]*b[25]; t+=(dpint)a[59]*b[24]; t+=(dpint)a[60]*b[23]; t+=(dpint)a[61]*b[22]; t+=(dpint)a[62]*b[21]; t+=(dpint)a[63]*b[20]; t+=(dpint)a[64]*b[19]; t+=(dpint)a[65]*b[18]; t+=(dpint)a[66]*b[17]; t+=(dpint)a[67]*b[16]; t+=(dpint)a[68]*b[15]; t+=(dpint)v15*(dpint)p68;  c[14]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[16]*b[68]; t+=(dpint)a[17]*b[67]; t+=(dpint)a[18]*b[66]; t+=(dpint)a[19]*b[65]; t+=(dpint)a[20]*b[64]; t+=(dpint)a[21]*b[63]; t+=(dpint)a[22]*b[62]; t+=(dpint)a[23]*b[61]; t+=(dpint)a[24]*b[60]; t+=(dpint)a[25]*b[59]; t+=(dpint)a[26]*b[58]; t+=(dpint)a[27]*b[57]; t+=(dpint)a[28]*b[56]; t+=(dpint)a[29]*b[55]; t+=(dpint)a[30]*b[54]; t+=(dpint)a[31]*b[53]; t+=(dpint)a[32]*b[52]; t+=(dpint)a[33]*b[51]; t+=(dpint)a[34]*b[50]; t+=(dpint)a[35]*b[49]; t+=(dpint)a[36]*b[48]; t+=(dpint)a[37]*b[47]; t+=(dpint)a[38]*b[46]; t+=(dpint)a[39]*b[45]; t+=(dpint)a[40]*b[44]; t+=(dpint)a[41]*b[43]; t+=(dpint)a[42]*b[42]; t+=(dpint)a[43]*b[41]; t+=(dpint)a[44]*b[40]; t+=(dpint)a[45]*b[39]; t+=(dpint)a[46]*b[38]; t+=(dpint)a[47]*b[37]; t+=(dpint)a[48]*b[36]; t+=(dpint)a[49]*b[35]; t+=(dpint)a[50]*b[34]; t+=(dpint)a[51]*b[33]; t+=(dpint)a[52]*b[32]; t+=(dpint)a[53]*b[31]; t+=(dpint)a[54]*b[30]; t+=(dpint)a[55]*b[29]; t+=(dpint)a[56]*b[28]; t+=(dpint)a[57]*b[27]; t+=(dpint)a[58]*b[26]; t+=(dpint)a[59]*b[25]; t+=(dpint)a[60]*b[24]; t+=(dpint)a[61]*b[23]; t+=(dpint)a[62]*b[22]; t+=(dpint)a[63]*b[21]; t+=(dpint)a[64]*b[20]; t+=(dpint)a[65]*b[19]; t+=(dpint)a[66]*b[18]; t+=(dpint)a[67]*b[17]; t+=(dpint)a[68]*b[16]; t+=(dpint)v16*(dpint)p68;  c[15]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[17]*b[68]; t+=(dpint)a[18]*b[67]; t+=(dpint)a[19]*b[66]; t+=(dpint)a[20]*b[65]; t+=(dpint)a[21]*b[64]; t+=(dpint)a[22]*b[63]; t+=(dpint)a[23]*b[62]; t+=(dpint)a[24]*b[61]; t+=(dpint)a[25]*b[60]; t+=(dpint)a[26]*b[59]; t+=(dpint)a[27]*b[58]; t+=(dpint)a[28]*b[57]; t+=(dpint)a[29]*b[56]; t+=(dpint)a[30]*b[55]; t+=(dpint)a[31]*b[54]; t+=(dpint)a[32]*b[53]; t+=(dpint)a[33]*b[52]; t+=(dpint)a[34]*b[51]; t+=(dpint)a[35]*b[50]; t+=(dpint)a[36]*b[49]; t+=(dpint)a[37]*b[48]; t+=(dpint)a[38]*b[47]; t+=(dpint)a[39]*b[46]; t+=(dpint)a[40]*b[45]; t+=(dpint)a[41]*b[44]; t+=(dpint)a[42]*b[43]; t+=(dpint)a[43]*b[42]; t+=(dpint)a[44]*b[41]; t+=(dpint)a[45]*b[40]; t+=(dpint)a[46]*b[39]; t+=(dpint)a[47]*b[38]; t+=(dpint)a[48]*b[37]; t+=(dpint)a[49]*b[36]; t+=(dpint)a[50]*b[35]; t+=(dpint)a[51]*b[34]; t+=(dpint)a[52]*b[33]; t+=(dpint)a[53]*b[32]; t+=(dpint)a[54]*b[31]; t+=(dpint)a[55]*b[30]; t+=(dpint)a[56]*b[29]; t+=(dpint)a[57]*b[28]; t+=(dpint)a[58]*b[27]; t+=(dpint)a[59]*b[26]; t+=(dpint)a[60]*b[25]; t+=(dpint)a[61]*b[24]; t+=(dpint)a[62]*b[23]; t+=(dpint)a[63]*b[22]; t+=(dpint)a[64]*b[21]; t+=(dpint)a[65]*b[20]; t+=(dpint)a[66]*b[19]; t+=(dpint)a[67]*b[18]; t+=(dpint)a[68]*b[17]; t+=(dpint)v17*(dpint)p68;  c[16]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[18]*b[68]; t+=(dpint)a[19]*b[67]; t+=(dpint)a[20]*b[66]; t+=(dpint)a[21]*b[65]; t+=(dpint)a[22]*b[64]; t+=(dpint)a[23]*b[63]; t+=(dpint)a[24]*b[62]; t+=(dpint)a[25]*b[61]; t+=(dpint)a[26]*b[60]; t+=(dpint)a[27]*b[59]; t+=(dpint)a[28]*b[58]; t+=(dpint)a[29]*b[57]; t+=(dpint)a[30]*b[56]; t+=(dpint)a[31]*b[55]; t+=(dpint)a[32]*b[54]; t+=(dpint)a[33]*b[53]; t+=(dpint)a[34]*b[52]; t+=(dpint)a[35]*b[51]; t+=(dpint)a[36]*b[50]; t+=(dpint)a[37]*b[49]; t+=(dpint)a[38]*b[48]; t+=(dpint)a[39]*b[47]; t+=(dpint)a[40]*b[46]; t+=(dpint)a[41]*b[45]; t+=(dpint)a[42]*b[44]; t+=(dpint)a[43]*b[43]; t+=(dpint)a[44]*b[42]; t+=(dpint)a[45]*b[41]; t+=(dpint)a[46]*b[40]; t+=(dpint)a[47]*b[39]; t+=(dpint)a[48]*b[38]; t+=(dpint)a[49]*b[37]; t+=(dpint)a[50]*b[36]; t+=(dpint)a[51]*b[35]; t+=(dpint)a[52]*b[34]; t+=(dpint)a[53]*b[33]; t+=(dpint)a[54]*b[32]; t+=(dpint)a[55]*b[31]; t+=(dpint)a[56]*b[30]; t+=(dpint)a[57]*b[29]; t+=(dpint)a[58]*b[28]; t+=(dpint)a[59]*b[27]; t+=(dpint)a[60]*b[26]; t+=(dpint)a[61]*b[25]; t+=(dpint)a[62]*b[24]; t+=(dpint)a[63]*b[23]; t+=(dpint)a[64]*b[22]; t+=(dpint)a[65]*b[21]; t+=(dpint)a[66]*b[20]; t+=(dpint)a[67]*b[19]; t+=(dpint)a[68]*b[18]; t+=(dpint)v18*(dpint)p68;  c[17]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[19]*b[68]; t+=(dpint)a[20]*b[67]; t+=(dpint)a[21]*b[66]; t+=(dpint)a[22]*b[65]; t+=(dpint)a[23]*b[64]; t+=(dpint)a[24]*b[63]; t+=(dpint)a[25]*b[62]; t+=(dpint)a[26]*b[61]; t+=(dpint)a[27]*b[60]; t+=(dpint)a[28]*b[59]; t+=(dpint)a[29]*b[58]; t+=(dpint)a[30]*b[57]; t+=(dpint)a[31]*b[56]; t+=(dpint)a[32]*b[55]; t+=(dpint)a[33]*b[54]; t+=(dpint)a[34]*b[53]; t+=(dpint)a[35]*b[52]; t+=(dpint)a[36]*b[51]; t+=(dpint)a[37]*b[50]; t+=(dpint)a[38]*b[49]; t+=(dpint)a[39]*b[48]; t+=(dpint)a[40]*b[47]; t+=(dpint)a[41]*b[46]; t+=(dpint)a[42]*b[45]; t+=(dpint)a[43]*b[44]; t+=(dpint)a[44]*b[43]; t+=(dpint)a[45]*b[42]; t+=(dpint)a[46]*b[41]; t+=(dpint)a[47]*b[40]; t+=(dpint)a[48]*b[39]; t+=(dpint)a[49]*b[38]; t+=(dpint)a[50]*b[37]; t+=(dpint)a[51]*b[36]; t+=(dpint)a[52]*b[35]; t+=(dpint)a[53]*b[34]; t+=(dpint)a[54]*b[33]; t+=(dpint)a[55]*b[32]; t+=(dpint)a[56]*b[31]; t+=(dpint)a[57]*b[30]; t+=(dpint)a[58]*b[29]; t+=(dpint)a[59]*b[28]; t+=(dpint)a[60]*b[27]; t+=(dpint)a[61]*b[26]; t+=(dpint)a[62]*b[25]; t+=(dpint)a[63]*b[24]; t+=(dpint)a[64]*b[23]; t+=(dpint)a[65]*b[22]; t+=(dpint)a[66]*b[21]; t+=(dpint)a[67]*b[20]; t+=(dpint)a[68]*b[19]; t+=(dpint)v19*(dpint)p68;  c[18]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[20]*b[68]; t+=(dpint)a[21]*b[67]; t+=(dpint)a[22]*b[66]; t+=(dpint)a[23]*b[65]; t+=(dpint)a[24]*b[64]; t+=(dpint)a[25]*b[63]; t+=(dpint)a[26]*b[62]; t+=(dpint)a[27]*b[61]; t+=(dpint)a[28]*b[60]; t+=(dpint)a[29]*b[59]; t+=(dpint)a[30]*b[58]; t+=(dpint)a[31]*b[57]; t+=(dpint)a[32]*b[56]; t+=(dpint)a[33]*b[55]; t+=(dpint)a[34]*b[54]; t+=(dpint)a[35]*b[53]; t+=(dpint)a[36]*b[52]; t+=(dpint)a[37]*b[51]; t+=(dpint)a[38]*b[50]; t+=(dpint)a[39]*b[49]; t+=(dpint)a[40]*b[48]; t+=(dpint)a[41]*b[47]; t+=(dpint)a[42]*b[46]; t+=(dpint)a[43]*b[45]; t+=(dpint)a[44]*b[44]; t+=(dpint)a[45]*b[43]; t+=(dpint)a[46]*b[42]; t+=(dpint)a[47]*b[41]; t+=(dpint)a[48]*b[40]; t+=(dpint)a[49]*b[39]; t+=(dpint)a[50]*b[38]; t+=(dpint)a[51]*b[37]; t+=(dpint)a[52]*b[36]; t+=(dpint)a[53]*b[35]; t+=(dpint)a[54]*b[34]; t+=(dpint)a[55]*b[33]; t+=(dpint)a[56]*b[32]; t+=(dpint)a[57]*b[31]; t+=(dpint)a[58]*b[30]; t+=(dpint)a[59]*b[29]; t+=(dpint)a[60]*b[28]; t+=(dpint)a[61]*b[27]; t+=(dpint)a[62]*b[26]; t+=(dpint)a[63]*b[25]; t+=(dpint)a[64]*b[24]; t+=(dpint)a[65]*b[23]; t+=(dpint)a[66]*b[22]; t+=(dpint)a[67]*b[21]; t+=(dpint)a[68]*b[20]; t+=(dpint)v20*(dpint)p68;  c[19]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[21]*b[68]; t+=(dpint)a[22]*b[67]; t+=(dpint)a[23]*b[66]; t+=(dpint)a[24]*b[65]; t+=(dpint)a[25]*b[64]; t+=(dpint)a[26]*b[63]; t+=(dpint)a[27]*b[62]; t+=(dpint)a[28]*b[61]; t+=(dpint)a[29]*b[60]; t+=(dpint)a[30]*b[59]; t+=(dpint)a[31]*b[58]; t+=(dpint)a[32]*b[57]; t+=(dpint)a[33]*b[56]; t+=(dpint)a[34]*b[55]; t+=(dpint)a[35]*b[54]; t+=(dpint)a[36]*b[53]; t+=(dpint)a[37]*b[52]; t+=(dpint)a[38]*b[51]; t+=(dpint)a[39]*b[50]; t+=(dpint)a[40]*b[49]; t+=(dpint)a[41]*b[48]; t+=(dpint)a[42]*b[47]; t+=(dpint)a[43]*b[46]; t+=(dpint)a[44]*b[45]; t+=(dpint)a[45]*b[44]; t+=(dpint)a[46]*b[43]; t+=(dpint)a[47]*b[42]; t+=(dpint)a[48]*b[41]; t+=(dpint)a[49]*b[40]; t+=(dpint)a[50]*b[39]; t+=(dpint)a[51]*b[38]; t+=(dpint)a[52]*b[37]; t+=(dpint)a[53]*b[36]; t+=(dpint)a[54]*b[35]; t+=(dpint)a[55]*b[34]; t+=(dpint)a[56]*b[33]; t+=(dpint)a[57]*b[32]; t+=(dpint)a[58]*b[31]; t+=(dpint)a[59]*b[30]; t+=(dpint)a[60]*b[29]; t+=(dpint)a[61]*b[28]; t+=(dpint)a[62]*b[27]; t+=(dpint)a[63]*b[26]; t+=(dpint)a[64]*b[25]; t+=(dpint)a[65]*b[24]; t+=(dpint)a[66]*b[23]; t+=(dpint)a[67]*b[22]; t+=(dpint)a[68]*b[21]; t+=(dpint)v21*(dpint)p68;  c[20]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[22]*b[68]; t+=(dpint)a[23]*b[67]; t+=(dpint)a[24]*b[66]; t+=(dpint)a[25]*b[65]; t+=(dpint)a[26]*b[64]; t+=(dpint)a[27]*b[63]; t+=(dpint)a[28]*b[62]; t+=(dpint)a[29]*b[61]; t+=(dpint)a[30]*b[60]; t+=(dpint)a[31]*b[59]; t+=(dpint)a[32]*b[58]; t+=(dpint)a[33]*b[57]; t+=(dpint)a[34]*b[56]; t+=(dpint)a[35]*b[55]; t+=(dpint)a[36]*b[54]; t+=(dpint)a[37]*b[53]; t+=(dpint)a[38]*b[52]; t+=(dpint)a[39]*b[51]; t+=(dpint)a[40]*b[50]; t+=(dpint)a[41]*b[49]; t+=(dpint)a[42]*b[48]; t+=(dpint)a[43]*b[47]; t+=(dpint)a[44]*b[46]; t+=(dpint)a[45]*b[45]; t+=(dpint)a[46]*b[44]; t+=(dpint)a[47]*b[43]; t+=(dpint)a[48]*b[42]; t+=(dpint)a[49]*b[41]; t+=(dpint)a[50]*b[40]; t+=(dpint)a[51]*b[39]; t+=(dpint)a[52]*b[38]; t+=(dpint)a[53]*b[37]; t+=(dpint)a[54]*b[36]; t+=(dpint)a[55]*b[35]; t+=(dpint)a[56]*b[34]; t+=(dpint)a[57]*b[33]; t+=(dpint)a[58]*b[32]; t+=(dpint)a[59]*b[31]; t+=(dpint)a[60]*b[30]; t+=(dpint)a[61]*b[29]; t+=(dpint)a[62]*b[28]; t+=(dpint)a[63]*b[27]; t+=(dpint)a[64]*b[26]; t+=(dpint)a[65]*b[25]; t+=(dpint)a[66]*b[24]; t+=(dpint)a[67]*b[23]; t+=(dpint)a[68]*b[22]; t+=(dpint)v22*(dpint)p68;  c[21]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[23]*b[68]; t+=(dpint)a[24]*b[67]; t+=(dpint)a[25]*b[66]; t+=(dpint)a[26]*b[65]; t+=(dpint)a[27]*b[64]; t+=(dpint)a[28]*b[63]; t+=(dpint)a[29]*b[62]; t+=(dpint)a[30]*b[61]; t+=(dpint)a[31]*b[60]; t+=(dpint)a[32]*b[59]; t+=(dpint)a[33]*b[58]; t+=(dpint)a[34]*b[57]; t+=(dpint)a[35]*b[56]; t+=(dpint)a[36]*b[55]; t+=(dpint)a[37]*b[54]; t+=(dpint)a[38]*b[53]; t+=(dpint)a[39]*b[52]; t+=(dpint)a[40]*b[51]; t+=(dpint)a[41]*b[50]; t+=(dpint)a[42]*b[49]; t+=(dpint)a[43]*b[48]; t+=(dpint)a[44]*b[47]; t+=(dpint)a[45]*b[46]; t+=(dpint)a[46]*b[45]; t+=(dpint)a[47]*b[44]; t+=(dpint)a[48]*b[43]; t+=(dpint)a[49]*b[42]; t+=(dpint)a[50]*b[41]; t+=(dpint)a[51]*b[40]; t+=(dpint)a[52]*b[39]; t+=(dpint)a[53]*b[38]; t+=(dpint)a[54]*b[37]; t+=(dpint)a[55]*b[36]; t+=(dpint)a[56]*b[35]; t+=(dpint)a[57]*b[34]; t+=(dpint)a[58]*b[33]; t+=(dpint)a[59]*b[32]; t+=(dpint)a[60]*b[31]; t+=(dpint)a[61]*b[30]; t+=(dpint)a[62]*b[29]; t+=(dpint)a[63]*b[28]; t+=(dpint)a[64]*b[27]; t+=(dpint)a[65]*b[26]; t+=(dpint)a[66]*b[25]; t+=(dpint)a[67]*b[24]; t+=(dpint)a[68]*b[23]; t+=(dpint)v23*(dpint)p68;  c[22]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[24]*b[68]; t+=(dpint)a[25]*b[67]; t+=(dpint)a[26]*b[66]; t+=(dpint)a[27]*b[65]; t+=(dpint)a[28]*b[64]; t+=(dpint)a[29]*b[63]; t+=(dpint)a[30]*b[62]; t+=(dpint)a[31]*b[61]; t+=(dpint)a[32]*b[60]; t+=(dpint)a[33]*b[59]; t+=(dpint)a[34]*b[58]; t+=(dpint)a[35]*b[57]; t+=(dpint)a[36]*b[56]; t+=(dpint)a[37]*b[55]; t+=(dpint)a[38]*b[54]; t+=(dpint)a[39]*b[53]; t+=(dpint)a[40]*b[52]; t+=(dpint)a[41]*b[51]; t+=(dpint)a[42]*b[50]; t+=(dpint)a[43]*b[49]; t+=(dpint)a[44]*b[48]; t+=(dpint)a[45]*b[47]; t+=(dpint)a[46]*b[46]; t+=(dpint)a[47]*b[45]; t+=(dpint)a[48]*b[44]; t+=(dpint)a[49]*b[43]; t+=(dpint)a[50]*b[42]; t+=(dpint)a[51]*b[41]; t+=(dpint)a[52]*b[40]; t+=(dpint)a[53]*b[39]; t+=(dpint)a[54]*b[38]; t+=(dpint)a[55]*b[37]; t+=(dpint)a[56]*b[36]; t+=(dpint)a[57]*b[35]; t+=(dpint)a[58]*b[34]; t+=(dpint)a[59]*b[33]; t+=(dpint)a[60]*b[32]; t+=(dpint)a[61]*b[31]; t+=(dpint)a[62]*b[30]; t+=(dpint)a[63]*b[29]; t+=(dpint)a[64]*b[28]; t+=(dpint)a[65]*b[27]; t+=(dpint)a[66]*b[26]; t+=(dpint)a[67]*b[25]; t+=(dpint)a[68]*b[24]; t+=(dpint)v24*(dpint)p68;  c[23]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[25]*b[68]; t+=(dpint)a[26]*b[67]; t+=(dpint)a[27]*b[66]; t+=(dpint)a[28]*b[65]; t+=(dpint)a[29]*b[64]; t+=(dpint)a[30]*b[63]; t+=(dpint)a[31]*b[62]; t+=(dpint)a[32]*b[61]; t+=(dpint)a[33]*b[60]; t+=(dpint)a[34]*b[59]; t+=(dpint)a[35]*b[58]; t+=(dpint)a[36]*b[57]; t+=(dpint)a[37]*b[56]; t+=(dpint)a[38]*b[55]; t+=(dpint)a[39]*b[54]; t+=(dpint)a[40]*b[53]; t+=(dpint)a[41]*b[52]; t+=(dpint)a[42]*b[51]; t+=(dpint)a[43]*b[50]; t+=(dpint)a[44]*b[49]; t+=(dpint)a[45]*b[48]; t+=(dpint)a[46]*b[47]; t+=(dpint)a[47]*b[46]; t+=(dpint)a[48]*b[45]; t+=(dpint)a[49]*b[44]; t+=(dpint)a[50]*b[43]; t+=(dpint)a[51]*b[42]; t+=(dpint)a[52]*b[41]; t+=(dpint)a[53]*b[40]; t+=(dpint)a[54]*b[39]; t+=(dpint)a[55]*b[38]; t+=(dpint)a[56]*b[37]; t+=(dpint)a[57]*b[36]; t+=(dpint)a[58]*b[35]; t+=(dpint)a[59]*b[34]; t+=(dpint)a[60]*b[33]; t+=(dpint)a[61]*b[32]; t+=(dpint)a[62]*b[31]; t+=(dpint)a[63]*b[30]; t+=(dpint)a[64]*b[29]; t+=(dpint)a[65]*b[28]; t+=(dpint)a[66]*b[27]; t+=(dpint)a[67]*b[26]; t+=(dpint)a[68]*b[25]; t+=(dpint)v25*(dpint)p68;  c[24]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[26]*b[68]; t+=(dpint)a[27]*b[67]; t+=(dpint)a[28]*b[66]; t+=(dpint)a[29]*b[65]; t+=(dpint)a[30]*b[64]; t+=(dpint)a[31]*b[63]; t+=(dpint)a[32]*b[62]; t+=(dpint)a[33]*b[61]; t+=(dpint)a[34]*b[60]; t+=(dpint)a[35]*b[59]; t+=(dpint)a[36]*b[58]; t+=(dpint)a[37]*b[57]; t+=(dpint)a[38]*b[56]; t+=(dpint)a[39]*b[55]; t+=(dpint)a[40]*b[54]; t+=(dpint)a[41]*b[53]; t+=(dpint)a[42]*b[52]; t+=(dpint)a[43]*b[51]; t+=(dpint)a[44]*b[50]; t+=(dpint)a[45]*b[49]; t+=(dpint)a[46]*b[48]; t+=(dpint)a[47]*b[47]; t+=(dpint)a[48]*b[46]; t+=(dpint)a[49]*b[45]; t+=(dpint)a[50]*b[44]; t+=(dpint)a[51]*b[43]; t+=(dpint)a[52]*b[42]; t+=(dpint)a[53]*b[41]; t+=(dpint)a[54]*b[40]; t+=(dpint)a[55]*b[39]; t+=(dpint)a[56]*b[38]; t+=(dpint)a[57]*b[37]; t+=(dpint)a[58]*b[36]; t+=(dpint)a[59]*b[35]; t+=(dpint)a[60]*b[34]; t+=(dpint)a[61]*b[33]; t+=(dpint)a[62]*b[32]; t+=(dpint)a[63]*b[31]; t+=(dpint)a[64]*b[30]; t+=(dpint)a[65]*b[29]; t+=(dpint)a[66]*b[28]; t+=(dpint)a[67]*b[27]; t+=(dpint)a[68]*b[26]; t+=(dpint)v26*(dpint)p68;  c[25]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[27]*b[68]; t+=(dpint)a[28]*b[67]; t+=(dpint)a[29]*b[66]; t+=(dpint)a[30]*b[65]; t+=(dpint)a[31]*b[64]; t+=(dpint)a[32]*b[63]; t+=(dpint)a[33]*b[62]; t+=(dpint)a[34]*b[61]; t+=(dpint)a[35]*b[60]; t+=(dpint)a[36]*b[59]; t+=(dpint)a[37]*b[58]; t+=(dpint)a[38]*b[57]; t+=(dpint)a[39]*b[56]; t+=(dpint)a[40]*b[55]; t+=(dpint)a[41]*b[54]; t+=(dpint)a[42]*b[53]; t+=(dpint)a[43]*b[52]; t+=(dpint)a[44]*b[51]; t+=(dpint)a[45]*b[50]; t+=(dpint)a[46]*b[49]; t+=(dpint)a[47]*b[48]; t+=(dpint)a[48]*b[47]; t+=(dpint)a[49]*b[46]; t+=(dpint)a[50]*b[45]; t+=(dpint)a[51]*b[44]; t+=(dpint)a[52]*b[43]; t+=(dpint)a[53]*b[42]; t+=(dpint)a[54]*b[41]; t+=(dpint)a[55]*b[40]; t+=(dpint)a[56]*b[39]; t+=(dpint)a[57]*b[38]; t+=(dpint)a[58]*b[37]; t+=(dpint)a[59]*b[36]; t+=(dpint)a[60]*b[35]; t+=(dpint)a[61]*b[34]; t+=(dpint)a[62]*b[33]; t+=(dpint)a[63]*b[32]; t+=(dpint)a[64]*b[31]; t+=(dpint)a[65]*b[30]; t+=(dpint)a[66]*b[29]; t+=(dpint)a[67]*b[28]; t+=(dpint)a[68]*b[27]; t+=(dpint)v27*(dpint)p68;  c[26]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[28]*b[68]; t+=(dpint)a[29]*b[67]; t+=(dpint)a[30]*b[66]; t+=(dpint)a[31]*b[65]; t+=(dpint)a[32]*b[64]; t+=(dpint)a[33]*b[63]; t+=(dpint)a[34]*b[62]; t+=(dpint)a[35]*b[61]; t+=(dpint)a[36]*b[60]; t+=(dpint)a[37]*b[59]; t+=(dpint)a[38]*b[58]; t+=(dpint)a[39]*b[57]; t+=(dpint)a[40]*b[56]; t+=(dpint)a[41]*b[55]; t+=(dpint)a[42]*b[54]; t+=(dpint)a[43]*b[53]; t+=(dpint)a[44]*b[52]; t+=(dpint)a[45]*b[51]; t+=(dpint)a[46]*b[50]; t+=(dpint)a[47]*b[49]; t+=(dpint)a[48]*b[48]; t+=(dpint)a[49]*b[47]; t+=(dpint)a[50]*b[46]; t+=(dpint)a[51]*b[45]; t+=(dpint)a[52]*b[44]; t+=(dpint)a[53]*b[43]; t+=(dpint)a[54]*b[42]; t+=(dpint)a[55]*b[41]; t+=(dpint)a[56]*b[40]; t+=(dpint)a[57]*b[39]; t+=(dpint)a[58]*b[38]; t+=(dpint)a[59]*b[37]; t+=(dpint)a[60]*b[36]; t+=(dpint)a[61]*b[35]; t+=(dpint)a[62]*b[34]; t+=(dpint)a[63]*b[33]; t+=(dpint)a[64]*b[32]; t+=(dpint)a[65]*b[31]; t+=(dpint)a[66]*b[30]; t+=(dpint)a[67]*b[29]; t+=(dpint)a[68]*b[28]; t+=(dpint)v28*(dpint)p68;  c[27]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[29]*b[68]; t+=(dpint)a[30]*b[67]; t+=(dpint)a[31]*b[66]; t+=(dpint)a[32]*b[65]; t+=(dpint)a[33]*b[64]; t+=(dpint)a[34]*b[63]; t+=(dpint)a[35]*b[62]; t+=(dpint)a[36]*b[61]; t+=(dpint)a[37]*b[60]; t+=(dpint)a[38]*b[59]; t+=(dpint)a[39]*b[58]; t+=(dpint)a[40]*b[57]; t+=(dpint)a[41]*b[56]; t+=(dpint)a[42]*b[55]; t+=(dpint)a[43]*b[54]; t+=(dpint)a[44]*b[53]; t+=(dpint)a[45]*b[52]; t+=(dpint)a[46]*b[51]; t+=(dpint)a[47]*b[50]; t+=(dpint)a[48]*b[49]; t+=(dpint)a[49]*b[48]; t+=(dpint)a[50]*b[47]; t+=(dpint)a[51]*b[46]; t+=(dpint)a[52]*b[45]; t+=(dpint)a[53]*b[44]; t+=(dpint)a[54]*b[43]; t+=(dpint)a[55]*b[42]; t+=(dpint)a[56]*b[41]; t+=(dpint)a[57]*b[40]; t+=(dpint)a[58]*b[39]; t+=(dpint)a[59]*b[38]; t+=(dpint)a[60]*b[37]; t+=(dpint)a[61]*b[36]; t+=(dpint)a[62]*b[35]; t+=(dpint)a[63]*b[34]; t+=(dpint)a[64]*b[33]; t+=(dpint)a[65]*b[32]; t+=(dpint)a[66]*b[31]; t+=(dpint)a[67]*b[30]; t+=(dpint)a[68]*b[29]; t+=(dpint)v29*(dpint)p68;  c[28]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[30]*b[68]; t+=(dpint)a[31]*b[67]; t+=(dpint)a[32]*b[66]; t+=(dpint)a[33]*b[65]; t+=(dpint)a[34]*b[64]; t+=(dpint)a[35]*b[63]; t+=(dpint)a[36]*b[62]; t+=(dpint)a[37]*b[61]; t+=(dpint)a[38]*b[60]; t+=(dpint)a[39]*b[59]; t+=(dpint)a[40]*b[58]; t+=(dpint)a[41]*b[57]; t+=(dpint)a[42]*b[56]; t+=(dpint)a[43]*b[55]; t+=(dpint)a[44]*b[54]; t+=(dpint)a[45]*b[53]; t+=(dpint)a[46]*b[52]; t+=(dpint)a[47]*b[51]; t+=(dpint)a[48]*b[50]; t+=(dpint)a[49]*b[49]; t+=(dpint)a[50]*b[48]; t+=(dpint)a[51]*b[47]; t+=(dpint)a[52]*b[46]; t+=(dpint)a[53]*b[45]; t+=(dpint)a[54]*b[44]; t+=(dpint)a[55]*b[43]; t+=(dpint)a[56]*b[42]; t+=(dpint)a[57]*b[41]; t+=(dpint)a[58]*b[40]; t+=(dpint)a[59]*b[39]; t+=(dpint)a[60]*b[38]; t+=(dpint)a[61]*b[37]; t+=(dpint)a[62]*b[36]; t+=(dpint)a[63]*b[35]; t+=(dpint)a[64]*b[34]; t+=(dpint)a[65]*b[33]; t+=(dpint)a[66]*b[32]; t+=(dpint)a[67]*b[31]; t+=(dpint)a[68]*b[30]; t+=(dpint)v30*(dpint)p68;  c[29]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[31]*b[68]; t+=(dpint)a[32]*b[67]; t+=(dpint)a[33]*b[66]; t+=(dpint)a[34]*b[65]; t+=(dpint)a[35]*b[64]; t+=(dpint)a[36]*b[63]; t+=(dpint)a[37]*b[62]; t+=(dpint)a[38]*b[61]; t+=(dpint)a[39]*b[60]; t+=(dpint)a[40]*b[59]; t+=(dpint)a[41]*b[58]; t+=(dpint)a[42]*b[57]; t+=(dpint)a[43]*b[56]; t+=(dpint)a[44]*b[55]; t+=(dpint)a[45]*b[54]; t+=(dpint)a[46]*b[53]; t+=(dpint)a[47]*b[52]; t+=(dpint)a[48]*b[51]; t+=(dpint)a[49]*b[50]; t+=(dpint)a[50]*b[49]; t+=(dpint)a[51]*b[48]; t+=(dpint)a[52]*b[47]; t+=(dpint)a[53]*b[46]; t+=(dpint)a[54]*b[45]; t+=(dpint)a[55]*b[44]; t+=(dpint)a[56]*b[43]; t+=(dpint)a[57]*b[42]; t+=(dpint)a[58]*b[41]; t+=(dpint)a[59]*b[40]; t+=(dpint)a[60]*b[39]; t+=(dpint)a[61]*b[38]; t+=(dpint)a[62]*b[37]; t+=(dpint)a[63]*b[36]; t+=(dpint)a[64]*b[35]; t+=(dpint)a[65]*b[34]; t+=(dpint)a[66]*b[33]; t+=(dpint)a[67]*b[32]; t+=(dpint)a[68]*b[31]; t+=(dpint)v31*(dpint)p68;  c[30]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[32]*b[68]; t+=(dpint)a[33]*b[67]; t+=(dpint)a[34]*b[66]; t+=(dpint)a[35]*b[65]; t+=(dpint)a[36]*b[64]; t+=(dpint)a[37]*b[63]; t+=(dpint)a[38]*b[62]; t+=(dpint)a[39]*b[61]; t+=(dpint)a[40]*b[60]; t+=(dpint)a[41]*b[59]; t+=(dpint)a[42]*b[58]; t+=(dpint)a[43]*b[57]; t+=(dpint)a[44]*b[56]; t+=(dpint)a[45]*b[55]; t+=(dpint)a[46]*b[54]; t+=(dpint)a[47]*b[53]; t+=(dpint)a[48]*b[52]; t+=(dpint)a[49]*b[51]; t+=(dpint)a[50]*b[50]; t+=(dpint)a[51]*b[49]; t+=(dpint)a[52]*b[48]; t+=(dpint)a[53]*b[47]; t+=(dpint)a[54]*b[46]; t+=(dpint)a[55]*b[45]; t+=(dpint)a[56]*b[44]; t+=(dpint)a[57]*b[43]; t+=(dpint)a[58]*b[42]; t+=(dpint)a[59]*b[41]; t+=(dpint)a[60]*b[40]; t+=(dpint)a[61]*b[39]; t+=(dpint)a[62]*b[38]; t+=(dpint)a[63]*b[37]; t+=(dpint)a[64]*b[36]; t+=(dpint)a[65]*b[35]; t+=(dpint)a[66]*b[34]; t+=(dpint)a[67]*b[33]; t+=(dpint)a[68]*b[32]; t+=(dpint)v32*(dpint)p68;  c[31]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[33]*b[68]; t+=(dpint)a[34]*b[67]; t+=(dpint)a[35]*b[66]; t+=(dpint)a[36]*b[65]; t+=(dpint)a[37]*b[64]; t+=(dpint)a[38]*b[63]; t+=(dpint)a[39]*b[62]; t+=(dpint)a[40]*b[61]; t+=(dpint)a[41]*b[60]; t+=(dpint)a[42]*b[59]; t+=(dpint)a[43]*b[58]; t+=(dpint)a[44]*b[57]; t+=(dpint)a[45]*b[56]; t+=(dpint)a[46]*b[55]; t+=(dpint)a[47]*b[54]; t+=(dpint)a[48]*b[53]; t+=(dpint)a[49]*b[52]; t+=(dpint)a[50]*b[51]; t+=(dpint)a[51]*b[50]; t+=(dpint)a[52]*b[49]; t+=(dpint)a[53]*b[48]; t+=(dpint)a[54]*b[47]; t+=(dpint)a[55]*b[46]; t+=(dpint)a[56]*b[45]; t+=(dpint)a[57]*b[44]; t+=(dpint)a[58]*b[43]; t+=(dpint)a[59]*b[42]; t+=(dpint)a[60]*b[41]; t+=(dpint)a[61]*b[40]; t+=(dpint)a[62]*b[39]; t+=(dpint)a[63]*b[38]; t+=(dpint)a[64]*b[37]; t+=(dpint)a[65]*b[36]; t+=(dpint)a[66]*b[35]; t+=(dpint)a[67]*b[34]; t+=(dpint)a[68]*b[33]; t+=(dpint)v33*(dpint)p68;  c[32]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[34]*b[68]; t+=(dpint)a[35]*b[67]; t+=(dpint)a[36]*b[66]; t+=(dpint)a[37]*b[65]; t+=(dpint)a[38]*b[64]; t+=(dpint)a[39]*b[63]; t+=(dpint)a[40]*b[62]; t+=(dpint)a[41]*b[61]; t+=(dpint)a[42]*b[60]; t+=(dpint)a[43]*b[59]; t+=(dpint)a[44]*b[58]; t+=(dpint)a[45]*b[57]; t+=(dpint)a[46]*b[56]; t+=(dpint)a[47]*b[55]; t+=(dpint)a[48]*b[54]; t+=(dpint)a[49]*b[53]; t+=(dpint)a[50]*b[52]; t+=(dpint)a[51]*b[51]; t+=(dpint)a[52]*b[50]; t+=(dpint)a[53]*b[49]; t+=(dpint)a[54]*b[48]; t+=(dpint)a[55]*b[47]; t+=(dpint)a[56]*b[46]; t+=(dpint)a[57]*b[45]; t+=(dpint)a[58]*b[44]; t+=(dpint)a[59]*b[43]; t+=(dpint)a[60]*b[42]; t+=(dpint)a[61]*b[41]; t+=(dpint)a[62]*b[40]; t+=(dpint)a[63]*b[39]; t+=(dpint)a[64]*b[38]; t+=(dpint)a[65]*b[37]; t+=(dpint)a[66]*b[36]; t+=(dpint)a[67]*b[35]; t+=(dpint)a[68]*b[34]; t+=(dpint)v34*(dpint)p68;  c[33]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[35]*b[68]; t+=(dpint)a[36]*b[67]; t+=(dpint)a[37]*b[66]; t+=(dpint)a[38]*b[65]; t+=(dpint)a[39]*b[64]; t+=(dpint)a[40]*b[63]; t+=(dpint)a[41]*b[62]; t+=(dpint)a[42]*b[61]; t+=(dpint)a[43]*b[60]; t+=(dpint)a[44]*b[59]; t+=(dpint)a[45]*b[58]; t+=(dpint)a[46]*b[57]; t+=(dpint)a[47]*b[56]; t+=(dpint)a[48]*b[55]; t+=(dpint)a[49]*b[54]; t+=(dpint)a[50]*b[53]; t+=(dpint)a[51]*b[52]; t+=(dpint)a[52]*b[51]; t+=(dpint)a[53]*b[50]; t+=(dpint)a[54]*b[49]; t+=(dpint)a[55]*b[48]; t+=(dpint)a[56]*b[47]; t+=(dpint)a[57]*b[46]; t+=(dpint)a[58]*b[45]; t+=(dpint)a[59]*b[44]; t+=(dpint)a[60]*b[43]; t+=(dpint)a[61]*b[42]; t+=(dpint)a[62]*b[41]; t+=(dpint)a[63]*b[40]; t+=(dpint)a[64]*b[39]; t+=(dpint)a[65]*b[38]; t+=(dpint)a[66]*b[37]; t+=(dpint)a[67]*b[36]; t+=(dpint)a[68]*b[35]; t+=(dpint)v35*(dpint)p68;  c[34]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[36]*b[68]; t+=(dpint)a[37]*b[67]; t+=(dpint)a[38]*b[66]; t+=(dpint)a[39]*b[65]; t+=(dpint)a[40]*b[64]; t+=(dpint)a[41]*b[63]; t+=(dpint)a[42]*b[62]; t+=(dpint)a[43]*b[61]; t+=(dpint)a[44]*b[60]; t+=(dpint)a[45]*b[59]; t+=(dpint)a[46]*b[58]; t+=(dpint)a[47]*b[57]; t+=(dpint)a[48]*b[56]; t+=(dpint)a[49]*b[55]; t+=(dpint)a[50]*b[54]; t+=(dpint)a[51]*b[53]; t+=(dpint)a[52]*b[52]; t+=(dpint)a[53]*b[51]; t+=(dpint)a[54]*b[50]; t+=(dpint)a[55]*b[49]; t+=(dpint)a[56]*b[48]; t+=(dpint)a[57]*b[47]; t+=(dpint)a[58]*b[46]; t+=(dpint)a[59]*b[45]; t+=(dpint)a[60]*b[44]; t+=(dpint)a[61]*b[43]; t+=(dpint)a[62]*b[42]; t+=(dpint)a[63]*b[41]; t+=(dpint)a[64]*b[40]; t+=(dpint)a[65]*b[39]; t+=(dpint)a[66]*b[38]; t+=(dpint)a[67]*b[37]; t+=(dpint)a[68]*b[36]; t+=(dpint)v36*(dpint)p68;  c[35]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[37]*b[68]; t+=(dpint)a[38]*b[67]; t+=(dpint)a[39]*b[66]; t+=(dpint)a[40]*b[65]; t+=(dpint)a[41]*b[64]; t+=(dpint)a[42]*b[63]; t+=(dpint)a[43]*b[62]; t+=(dpint)a[44]*b[61]; t+=(dpint)a[45]*b[60]; t+=(dpint)a[46]*b[59]; t+=(dpint)a[47]*b[58]; t+=(dpint)a[48]*b[57]; t+=(dpint)a[49]*b[56]; t+=(dpint)a[50]*b[55]; t+=(dpint)a[51]*b[54]; t+=(dpint)a[52]*b[53]; t+=(dpint)a[53]*b[52]; t+=(dpint)a[54]*b[51]; t+=(dpint)a[55]*b[50]; t+=(dpint)a[56]*b[49]; t+=(dpint)a[57]*b[48]; t+=(dpint)a[58]*b[47]; t+=(dpint)a[59]*b[46]; t+=(dpint)a[60]*b[45]; t+=(dpint)a[61]*b[44]; t+=(dpint)a[62]*b[43]; t+=(dpint)a[63]*b[42]; t+=(dpint)a[64]*b[41]; t+=(dpint)a[65]*b[40]; t+=(dpint)a[66]*b[39]; t+=(dpint)a[67]*b[38]; t+=(dpint)a[68]*b[37]; t+=(dpint)v37*(dpint)p68;  c[36]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[38]*b[68]; t+=(dpint)a[39]*b[67]; t+=(dpint)a[40]*b[66]; t+=(dpint)a[41]*b[65]; t+=(dpint)a[42]*b[64]; t+=(dpint)a[43]*b[63]; t+=(dpint)a[44]*b[62]; t+=(dpint)a[45]*b[61]; t+=(dpint)a[46]*b[60]; t+=(dpint)a[47]*b[59]; t+=(dpint)a[48]*b[58]; t+=(dpint)a[49]*b[57]; t+=(dpint)a[50]*b[56]; t+=(dpint)a[51]*b[55]; t+=(dpint)a[52]*b[54]; t+=(dpint)a[53]*b[53]; t+=(dpint)a[54]*b[52]; t+=(dpint)a[55]*b[51]; t+=(dpint)a[56]*b[50]; t+=(dpint)a[57]*b[49]; t+=(dpint)a[58]*b[48]; t+=(dpint)a[59]*b[47]; t+=(dpint)a[60]*b[46]; t+=(dpint)a[61]*b[45]; t+=(dpint)a[62]*b[44]; t+=(dpint)a[63]*b[43]; t+=(dpint)a[64]*b[42]; t+=(dpint)a[65]*b[41]; t+=(dpint)a[66]*b[40]; t+=(dpint)a[67]*b[39]; t+=(dpint)a[68]*b[38]; t+=(dpint)v38*(dpint)p68;  c[37]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[39]*b[68]; t+=(dpint)a[40]*b[67]; t+=(dpint)a[41]*b[66]; t+=(dpint)a[42]*b[65]; t+=(dpint)a[43]*b[64]; t+=(dpint)a[44]*b[63]; t+=(dpint)a[45]*b[62]; t+=(dpint)a[46]*b[61]; t+=(dpint)a[47]*b[60]; t+=(dpint)a[48]*b[59]; t+=(dpint)a[49]*b[58]; t+=(dpint)a[50]*b[57]; t+=(dpint)a[51]*b[56]; t+=(dpint)a[52]*b[55]; t+=(dpint)a[53]*b[54]; t+=(dpint)a[54]*b[53]; t+=(dpint)a[55]*b[52]; t+=(dpint)a[56]*b[51]; t+=(dpint)a[57]*b[50]; t+=(dpint)a[58]*b[49]; t+=(dpint)a[59]*b[48]; t+=(dpint)a[60]*b[47]; t+=(dpint)a[61]*b[46]; t+=(dpint)a[62]*b[45]; t+=(dpint)a[63]*b[44]; t+=(dpint)a[64]*b[43]; t+=(dpint)a[65]*b[42]; t+=(dpint)a[66]*b[41]; t+=(dpint)a[67]*b[40]; t+=(dpint)a[68]*b[39]; t+=(dpint)v39*(dpint)p68;  c[38]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[40]*b[68]; t+=(dpint)a[41]*b[67]; t+=(dpint)a[42]*b[66]; t+=(dpint)a[43]*b[65]; t+=(dpint)a[44]*b[64]; t+=(dpint)a[45]*b[63]; t+=(dpint)a[46]*b[62]; t+=(dpint)a[47]*b[61]; t+=(dpint)a[48]*b[60]; t+=(dpint)a[49]*b[59]; t+=(dpint)a[50]*b[58]; t+=(dpint)a[51]*b[57]; t+=(dpint)a[52]*b[56]; t+=(dpint)a[53]*b[55]; t+=(dpint)a[54]*b[54]; t+=(dpint)a[55]*b[53]; t+=(dpint)a[56]*b[52]; t+=(dpint)a[57]*b[51]; t+=(dpint)a[58]*b[50]; t+=(dpint)a[59]*b[49]; t+=(dpint)a[60]*b[48]; t+=(dpint)a[61]*b[47]; t+=(dpint)a[62]*b[46]; t+=(dpint)a[63]*b[45]; t+=(dpint)a[64]*b[44]; t+=(dpint)a[65]*b[43]; t+=(dpint)a[66]*b[42]; t+=(dpint)a[67]*b[41]; t+=(dpint)a[68]*b[40]; t+=(dpint)v40*(dpint)p68;  c[39]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[41]*b[68]; t+=(dpint)a[42]*b[67]; t+=(dpint)a[43]*b[66]; t+=(dpint)a[44]*b[65]; t+=(dpint)a[45]*b[64]; t+=(dpint)a[46]*b[63]; t+=(dpint)a[47]*b[62]; t+=(dpint)a[48]*b[61]; t+=(dpint)a[49]*b[60]; t+=(dpint)a[50]*b[59]; t+=(dpint)a[51]*b[58]; t+=(dpint)a[52]*b[57]; t+=(dpint)a[53]*b[56]; t+=(dpint)a[54]*b[55]; t+=(dpint)a[55]*b[54]; t+=(dpint)a[56]*b[53]; t+=(dpint)a[57]*b[52]; t+=(dpint)a[58]*b[51]; t+=(dpint)a[59]*b[50]; t+=(dpint)a[60]*b[49]; t+=(dpint)a[61]*b[48]; t+=(dpint)a[62]*b[47]; t+=(dpint)a[63]*b[46]; t+=(dpint)a[64]*b[45]; t+=(dpint)a[65]*b[44]; t+=(dpint)a[66]*b[43]; t+=(dpint)a[67]*b[42]; t+=(dpint)a[68]*b[41]; t+=(dpint)v41*(dpint)p68;  c[40]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[42]*b[68]; t+=(dpint)a[43]*b[67]; t+=(dpint)a[44]*b[66]; t+=(dpint)a[45]*b[65]; t+=(dpint)a[46]*b[64]; t+=(dpint)a[47]*b[63]; t+=(dpint)a[48]*b[62]; t+=(dpint)a[49]*b[61]; t+=(dpint)a[50]*b[60]; t+=(dpint)a[51]*b[59]; t+=(dpint)a[52]*b[58]; t+=(dpint)a[53]*b[57]; t+=(dpint)a[54]*b[56]; t+=(dpint)a[55]*b[55]; t+=(dpint)a[56]*b[54]; t+=(dpint)a[57]*b[53]; t+=(dpint)a[58]*b[52]; t+=(dpint)a[59]*b[51]; t+=(dpint)a[60]*b[50]; t+=(dpint)a[61]*b[49]; t+=(dpint)a[62]*b[48]; t+=(dpint)a[63]*b[47]; t+=(dpint)a[64]*b[46]; t+=(dpint)a[65]*b[45]; t+=(dpint)a[66]*b[44]; t+=(dpint)a[67]*b[43]; t+=(dpint)a[68]*b[42]; t+=(dpint)v42*(dpint)p68;  c[41]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[43]*b[68]; t+=(dpint)a[44]*b[67]; t+=(dpint)a[45]*b[66]; t+=(dpint)a[46]*b[65]; t+=(dpint)a[47]*b[64]; t+=(dpint)a[48]*b[63]; t+=(dpint)a[49]*b[62]; t+=(dpint)a[50]*b[61]; t+=(dpint)a[51]*b[60]; t+=(dpint)a[52]*b[59]; t+=(dpint)a[53]*b[58]; t+=(dpint)a[54]*b[57]; t+=(dpint)a[55]*b[56]; t+=(dpint)a[56]*b[55]; t+=(dpint)a[57]*b[54]; t+=(dpint)a[58]*b[53]; t+=(dpint)a[59]*b[52]; t+=(dpint)a[60]*b[51]; t+=(dpint)a[61]*b[50]; t+=(dpint)a[62]*b[49]; t+=(dpint)a[63]*b[48]; t+=(dpint)a[64]*b[47]; t+=(dpint)a[65]*b[46]; t+=(dpint)a[66]*b[45]; t+=(dpint)a[67]*b[44]; t+=(dpint)a[68]*b[43]; t+=(dpint)v43*(dpint)p68;  c[42]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[44]*b[68]; t+=(dpint)a[45]*b[67]; t+=(dpint)a[46]*b[66]; t+=(dpint)a[47]*b[65]; t+=(dpint)a[48]*b[64]; t+=(dpint)a[49]*b[63]; t+=(dpint)a[50]*b[62]; t+=(dpint)a[51]*b[61]; t+=(dpint)a[52]*b[60]; t+=(dpint)a[53]*b[59]; t+=(dpint)a[54]*b[58]; t+=(dpint)a[55]*b[57]; t+=(dpint)a[56]*b[56]; t+=(dpint)a[57]*b[55]; t+=(dpint)a[58]*b[54]; t+=(dpint)a[59]*b[53]; t+=(dpint)a[60]*b[52]; t+=(dpint)a[61]*b[51]; t+=(dpint)a[62]*b[50]; t+=(dpint)a[63]*b[49]; t+=(dpint)a[64]*b[48]; t+=(dpint)a[65]*b[47]; t+=(dpint)a[66]*b[46]; t+=(dpint)a[67]*b[45]; t+=(dpint)a[68]*b[44]; t+=(dpint)v44*(dpint)p68;  c[43]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[45]*b[68]; t+=(dpint)a[46]*b[67]; t+=(dpint)a[47]*b[66]; t+=(dpint)a[48]*b[65]; t+=(dpint)a[49]*b[64]; t+=(dpint)a[50]*b[63]; t+=(dpint)a[51]*b[62]; t+=(dpint)a[52]*b[61]; t+=(dpint)a[53]*b[60]; t+=(dpint)a[54]*b[59]; t+=(dpint)a[55]*b[58]; t+=(dpint)a[56]*b[57]; t+=(dpint)a[57]*b[56]; t+=(dpint)a[58]*b[55]; t+=(dpint)a[59]*b[54]; t+=(dpint)a[60]*b[53]; t+=(dpint)a[61]*b[52]; t+=(dpint)a[62]*b[51]; t+=(dpint)a[63]*b[50]; t+=(dpint)a[64]*b[49]; t+=(dpint)a[65]*b[48]; t+=(dpint)a[66]*b[47]; t+=(dpint)a[67]*b[46]; t+=(dpint)a[68]*b[45]; t+=(dpint)v45*(dpint)p68;  c[44]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[46]*b[68]; t+=(dpint)a[47]*b[67]; t+=(dpint)a[48]*b[66]; t+=(dpint)a[49]*b[65]; t+=(dpint)a[50]*b[64]; t+=(dpint)a[51]*b[63]; t+=(dpint)a[52]*b[62]; t+=(dpint)a[53]*b[61]; t+=(dpint)a[54]*b[60]; t+=(dpint)a[55]*b[59]; t+=(dpint)a[56]*b[58]; t+=(dpint)a[57]*b[57]; t+=(dpint)a[58]*b[56]; t+=(dpint)a[59]*b[55]; t+=(dpint)a[60]*b[54]; t+=(dpint)a[61]*b[53]; t+=(dpint)a[62]*b[52]; t+=(dpint)a[63]*b[51]; t+=(dpint)a[64]*b[50]; t+=(dpint)a[65]*b[49]; t+=(dpint)a[66]*b[48]; t+=(dpint)a[67]*b[47]; t+=(dpint)a[68]*b[46]; t+=(dpint)v46*(dpint)p68;  c[45]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[47]*b[68]; t+=(dpint)a[48]*b[67]; t+=(dpint)a[49]*b[66]; t+=(dpint)a[50]*b[65]; t+=(dpint)a[51]*b[64]; t+=(dpint)a[52]*b[63]; t+=(dpint)a[53]*b[62]; t+=(dpint)a[54]*b[61]; t+=(dpint)a[55]*b[60]; t+=(dpint)a[56]*b[59]; t+=(dpint)a[57]*b[58]; t+=(dpint)a[58]*b[57]; t+=(dpint)a[59]*b[56]; t+=(dpint)a[60]*b[55]; t+=(dpint)a[61]*b[54]; t+=(dpint)a[62]*b[53]; t+=(dpint)a[63]*b[52]; t+=(dpint)a[64]*b[51]; t+=(dpint)a[65]*b[50]; t+=(dpint)a[66]*b[49]; t+=(dpint)a[67]*b[48]; t+=(dpint)a[68]*b[47]; t+=(dpint)v47*(dpint)p68;  c[46]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[48]*b[68]; t+=(dpint)a[49]*b[67]; t+=(dpint)a[50]*b[66]; t+=(dpint)a[51]*b[65]; t+=(dpint)a[52]*b[64]; t+=(dpint)a[53]*b[63]; t+=(dpint)a[54]*b[62]; t+=(dpint)a[55]*b[61]; t+=(dpint)a[56]*b[60]; t+=(dpint)a[57]*b[59]; t+=(dpint)a[58]*b[58]; t+=(dpint)a[59]*b[57]; t+=(dpint)a[60]*b[56]; t+=(dpint)a[61]*b[55]; t+=(dpint)a[62]*b[54]; t+=(dpint)a[63]*b[53]; t+=(dpint)a[64]*b[52]; t+=(dpint)a[65]*b[51]; t+=(dpint)a[66]*b[50]; t+=(dpint)a[67]*b[49]; t+=(dpint)a[68]*b[48]; t+=(dpint)v48*(dpint)p68;  c[47]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[49]*b[68]; t+=(dpint)a[50]*b[67]; t+=(dpint)a[51]*b[66]; t+=(dpint)a[52]*b[65]; t+=(dpint)a[53]*b[64]; t+=(dpint)a[54]*b[63]; t+=(dpint)a[55]*b[62]; t+=(dpint)a[56]*b[61]; t+=(dpint)a[57]*b[60]; t+=(dpint)a[58]*b[59]; t+=(dpint)a[59]*b[58]; t+=(dpint)a[60]*b[57]; t+=(dpint)a[61]*b[56]; t+=(dpint)a[62]*b[55]; t+=(dpint)a[63]*b[54]; t+=(dpint)a[64]*b[53]; t+=(dpint)a[65]*b[52]; t+=(dpint)a[66]*b[51]; t+=(dpint)a[67]*b[50]; t+=(dpint)a[68]*b[49]; t+=(dpint)v49*(dpint)p68;  c[48]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[50]*b[68]; t+=(dpint)a[51]*b[67]; t+=(dpint)a[52]*b[66]; t+=(dpint)a[53]*b[65]; t+=(dpint)a[54]*b[64]; t+=(dpint)a[55]*b[63]; t+=(dpint)a[56]*b[62]; t+=(dpint)a[57]*b[61]; t+=(dpint)a[58]*b[60]; t+=(dpint)a[59]*b[59]; t+=(dpint)a[60]*b[58]; t+=(dpint)a[61]*b[57]; t+=(dpint)a[62]*b[56]; t+=(dpint)a[63]*b[55]; t+=(dpint)a[64]*b[54]; t+=(dpint)a[65]*b[53]; t+=(dpint)a[66]*b[52]; t+=(dpint)a[67]*b[51]; t+=(dpint)a[68]*b[50]; t+=(dpint)v50*(dpint)p68;  c[49]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[51]*b[68]; t+=(dpint)a[52]*b[67]; t+=(dpint)a[53]*b[66]; t+=(dpint)a[54]*b[65]; t+=(dpint)a[55]*b[64]; t+=(dpint)a[56]*b[63]; t+=(dpint)a[57]*b[62]; t+=(dpint)a[58]*b[61]; t+=(dpint)a[59]*b[60]; t+=(dpint)a[60]*b[59]; t+=(dpint)a[61]*b[58]; t+=(dpint)a[62]*b[57]; t+=(dpint)a[63]*b[56]; t+=(dpint)a[64]*b[55]; t+=(dpint)a[65]*b[54]; t+=(dpint)a[66]*b[53]; t+=(dpint)a[67]*b[52]; t+=(dpint)a[68]*b[51]; t+=(dpint)v51*(dpint)p68;  c[50]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[52]*b[68]; t+=(dpint)a[53]*b[67]; t+=(dpint)a[54]*b[66]; t+=(dpint)a[55]*b[65]; t+=(dpint)a[56]*b[64]; t+=(dpint)a[57]*b[63]; t+=(dpint)a[58]*b[62]; t+=(dpint)a[59]*b[61]; t+=(dpint)a[60]*b[60]; t+=(dpint)a[61]*b[59]; t+=(dpint)a[62]*b[58]; t+=(dpint)a[63]*b[57]; t+=(dpint)a[64]*b[56]; t+=(dpint)a[65]*b[55]; t+=(dpint)a[66]*b[54]; t+=(dpint)a[67]*b[53]; t+=(dpint)a[68]*b[52]; t+=(dpint)v52*(dpint)p68;  c[51]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[53]*b[68]; t+=(dpint)a[54]*b[67]; t+=(dpint)a[55]*b[66]; t+=(dpint)a[56]*b[65]; t+=(dpint)a[57]*b[64]; t+=(dpint)a[58]*b[63]; t+=(dpint)a[59]*b[62]; t+=(dpint)a[60]*b[61]; t+=(dpint)a[61]*b[60]; t+=(dpint)a[62]*b[59]; t+=(dpint)a[63]*b[58]; t+=(dpint)a[64]*b[57]; t+=(dpint)a[65]*b[56]; t+=(dpint)a[66]*b[55]; t+=(dpint)a[67]*b[54]; t+=(dpint)a[68]*b[53]; t+=(dpint)v53*(dpint)p68;  c[52]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[54]*b[68]; t+=(dpint)a[55]*b[67]; t+=(dpint)a[56]*b[66]; t+=(dpint)a[57]*b[65]; t+=(dpint)a[58]*b[64]; t+=(dpint)a[59]*b[63]; t+=(dpint)a[60]*b[62]; t+=(dpint)a[61]*b[61]; t+=(dpint)a[62]*b[60]; t+=(dpint)a[63]*b[59]; t+=(dpint)a[64]*b[58]; t+=(dpint)a[65]*b[57]; t+=(dpint)a[66]*b[56]; t+=(dpint)a[67]*b[55]; t+=(dpint)a[68]*b[54]; t+=(dpint)v54*(dpint)p68;  c[53]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[55]*b[68]; t+=(dpint)a[56]*b[67]; t+=(dpint)a[57]*b[66]; t+=(dpint)a[58]*b[65]; t+=(dpint)a[59]*b[64]; t+=(dpint)a[60]*b[63]; t+=(dpint)a[61]*b[62]; t+=(dpint)a[62]*b[61]; t+=(dpint)a[63]*b[60]; t+=(dpint)a[64]*b[59]; t+=(dpint)a[65]*b[58]; t+=(dpint)a[66]*b[57]; t+=(dpint)a[67]*b[56]; t+=(dpint)a[68]*b[55]; t+=(dpint)v55*(dpint)p68;  c[54]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[56]*b[68]; t+=(dpint)a[57]*b[67]; t+=(dpint)a[58]*b[66]; t+=(dpint)a[59]*b[65]; t+=(dpint)a[60]*b[64]; t+=(dpint)a[61]*b[63]; t+=(dpint)a[62]*b[62]; t+=(dpint)a[63]*b[61]; t+=(dpint)a[64]*b[60]; t+=(dpint)a[65]*b[59]; t+=(dpint)a[66]*b[58]; t+=(dpint)a[67]*b[57]; t+=(dpint)a[68]*b[56]; t+=(dpint)v56*(dpint)p68;  c[55]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[57]*b[68]; t+=(dpint)a[58]*b[67]; t+=(dpint)a[59]*b[66]; t+=(dpint)a[60]*b[65]; t+=(dpint)a[61]*b[64]; t+=(dpint)a[62]*b[63]; t+=(dpint)a[63]*b[62]; t+=(dpint)a[64]*b[61]; t+=(dpint)a[65]*b[60]; t+=(dpint)a[66]*b[59]; t+=(dpint)a[67]*b[58]; t+=(dpint)a[68]*b[57]; t+=(dpint)v57*(dpint)p68;  c[56]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[58]*b[68]; t+=(dpint)a[59]*b[67]; t+=(dpint)a[60]*b[66]; t+=(dpint)a[61]*b[65]; t+=(dpint)a[62]*b[64]; t+=(dpint)a[63]*b[63]; t+=(dpint)a[64]*b[62]; t+=(dpint)a[65]*b[61]; t+=(dpint)a[66]*b[60]; t+=(dpint)a[67]*b[59]; t+=(dpint)a[68]*b[58]; t+=(dpint)v58*(dpint)p68;  c[57]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[59]*b[68]; t+=(dpint)a[60]*b[67]; t+=(dpint)a[61]*b[66]; t+=(dpint)a[62]*b[65]; t+=(dpint)a[63]*b[64]; t+=(dpint)a[64]*b[63]; t+=(dpint)a[65]*b[62]; t+=(dpint)a[66]*b[61]; t+=(dpint)a[67]*b[60]; t+=(dpint)a[68]*b[59]; t+=(dpint)v59*(dpint)p68;  c[58]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[60]*b[68]; t+=(dpint)a[61]*b[67]; t+=(dpint)a[62]*b[66]; t+=(dpint)a[63]*b[65]; t+=(dpint)a[64]*b[64]; t+=(dpint)a[65]*b[63]; t+=(dpint)a[66]*b[62]; t+=(dpint)a[67]*b[61]; t+=(dpint)a[68]*b[60]; t+=(dpint)v60*(dpint)p68;  c[59]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[61]*b[68]; t+=(dpint)a[62]*b[67]; t+=(dpint)a[63]*b[66]; t+=(dpint)a[64]*b[65]; t+=(dpint)a[65]*b[64]; t+=(dpint)a[66]*b[63]; t+=(dpint)a[67]*b[62]; t+=(dpint)a[68]*b[61]; t+=(dpint)v61*(dpint)p68;  c[60]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[62]*b[68]; t+=(dpint)a[63]*b[67]; t+=(dpint)a[64]*b[66]; t+=(dpint)a[65]*b[65]; t+=(dpint)a[66]*b[64]; t+=(dpint)a[67]*b[63]; t+=(dpint)a[68]*b[62]; t+=(dpint)v62*(dpint)p68;  c[61]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[63]*b[68]; t+=(dpint)a[64]*b[67]; t+=(dpint)a[65]*b[66]; t+=(dpint)a[66]*b[65]; t+=(dpint)a[67]*b[64]; t+=(dpint)a[68]*b[63]; t+=(dpint)v63*(dpint)p68;  c[62]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[64]*b[68]; t+=(dpint)a[65]*b[67]; t+=(dpint)a[66]*b[66]; t+=(dpint)a[67]*b[65]; t+=(dpint)a[68]*b[64]; t+=(dpint)v64*(dpint)p68;  c[63]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[65]*b[68]; t+=(dpint)a[66]*b[67]; t+=(dpint)a[67]*b[66]; t+=(dpint)a[68]*b[65]; t+=(dpint)v65*(dpint)p68;  c[64]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[66]*b[68]; t+=(dpint)a[67]*b[67]; t+=(dpint)a[68]*b[66]; t+=(dpint)v66*(dpint)p68;  c[65]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[67]*b[68]; t+=(dpint)a[68]*b[67]; t+=(dpint)v67*(dpint)p68;  c[66]=((spint)t & mask);  t>>=60;
	t+=(dpint)a[68]*b[68]; t+=(dpint)v68*(dpint)p68;  c[67]=((spint)t & mask);  t>>=60;
	c[68] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void modsqr(const spint *a,spint *c) {
	udpint tot;
	udpint t=0;
	spint p68=0x3f0u;
	spint q=((spint)1<<60u); // q is unsaturated radix 
	spint mask=(spint)(q-(spint)1);
	tot=(udpint)a[0]*a[0]; t=tot; spint v0=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[1]; tot*=2; t+=tot;  spint v1=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[2]; tot*=2; tot+=(udpint)a[1]*a[1]; t+=tot;  spint v2=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[3]; tot+=(udpint)a[1]*a[2]; tot*=2; t+=tot;  spint v3=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[4]; tot+=(udpint)a[1]*a[3]; tot*=2; tot+=(udpint)a[2]*a[2]; t+=tot;  spint v4=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[5]; tot+=(udpint)a[1]*a[4]; tot+=(udpint)a[2]*a[3]; tot*=2; t+=tot;  spint v5=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[6]; tot+=(udpint)a[1]*a[5]; tot+=(udpint)a[2]*a[4]; tot*=2; tot+=(udpint)a[3]*a[3]; t+=tot;  spint v6=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[7]; tot+=(udpint)a[1]*a[6]; tot+=(udpint)a[2]*a[5]; tot+=(udpint)a[3]*a[4]; tot*=2; t+=tot;  spint v7=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[8]; tot+=(udpint)a[1]*a[7]; tot+=(udpint)a[2]*a[6]; tot+=(udpint)a[3]*a[5]; tot*=2; tot+=(udpint)a[4]*a[4]; t+=tot;  spint v8=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[9]; tot+=(udpint)a[1]*a[8]; tot+=(udpint)a[2]*a[7]; tot+=(udpint)a[3]*a[6]; tot+=(udpint)a[4]*a[5]; tot*=2; t+=tot;  spint v9=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[10]; tot+=(udpint)a[1]*a[9]; tot+=(udpint)a[2]*a[8]; tot+=(udpint)a[3]*a[7]; tot+=(udpint)a[4]*a[6]; tot*=2; tot+=(udpint)a[5]*a[5]; t+=tot;  spint v10=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[11]; tot+=(udpint)a[1]*a[10]; tot+=(udpint)a[2]*a[9]; tot+=(udpint)a[3]*a[8]; tot+=(udpint)a[4]*a[7]; tot+=(udpint)a[5]*a[6]; tot*=2; t+=tot;  spint v11=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[12]; tot+=(udpint)a[1]*a[11]; tot+=(udpint)a[2]*a[10]; tot+=(udpint)a[3]*a[9]; tot+=(udpint)a[4]*a[8]; tot+=(udpint)a[5]*a[7]; tot*=2; tot+=(udpint)a[6]*a[6]; t+=tot;  spint v12=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[13]; tot+=(udpint)a[1]*a[12]; tot+=(udpint)a[2]*a[11]; tot+=(udpint)a[3]*a[10]; tot+=(udpint)a[4]*a[9]; tot+=(udpint)a[5]*a[8]; tot+=(udpint)a[6]*a[7]; tot*=2; t+=tot;  spint v13=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[14]; tot+=(udpint)a[1]*a[13]; tot+=(udpint)a[2]*a[12]; tot+=(udpint)a[3]*a[11]; tot+=(udpint)a[4]*a[10]; tot+=(udpint)a[5]*a[9]; tot+=(udpint)a[6]*a[8]; tot*=2; tot+=(udpint)a[7]*a[7]; t+=tot;  spint v14=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[15]; tot+=(udpint)a[1]*a[14]; tot+=(udpint)a[2]*a[13]; tot+=(udpint)a[3]*a[12]; tot+=(udpint)a[4]*a[11]; tot+=(udpint)a[5]*a[10]; tot+=(udpint)a[6]*a[9]; tot+=(udpint)a[7]*a[8]; tot*=2; t+=tot;  spint v15=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[16]; tot+=(udpint)a[1]*a[15]; tot+=(udpint)a[2]*a[14]; tot+=(udpint)a[3]*a[13]; tot+=(udpint)a[4]*a[12]; tot+=(udpint)a[5]*a[11]; tot+=(udpint)a[6]*a[10]; tot+=(udpint)a[7]*a[9]; tot*=2; tot+=(udpint)a[8]*a[8]; t+=tot;  spint v16=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[17]; tot+=(udpint)a[1]*a[16]; tot+=(udpint)a[2]*a[15]; tot+=(udpint)a[3]*a[14]; tot+=(udpint)a[4]*a[13]; tot+=(udpint)a[5]*a[12]; tot+=(udpint)a[6]*a[11]; tot+=(udpint)a[7]*a[10]; tot+=(udpint)a[8]*a[9]; tot*=2; t+=tot;  spint v17=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[18]; tot+=(udpint)a[1]*a[17]; tot+=(udpint)a[2]*a[16]; tot+=(udpint)a[3]*a[15]; tot+=(udpint)a[4]*a[14]; tot+=(udpint)a[5]*a[13]; tot+=(udpint)a[6]*a[12]; tot+=(udpint)a[7]*a[11]; tot+=(udpint)a[8]*a[10]; tot*=2; tot+=(udpint)a[9]*a[9]; t+=tot;  spint v18=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[19]; tot+=(udpint)a[1]*a[18]; tot+=(udpint)a[2]*a[17]; tot+=(udpint)a[3]*a[16]; tot+=(udpint)a[4]*a[15]; tot+=(udpint)a[5]*a[14]; tot+=(udpint)a[6]*a[13]; tot+=(udpint)a[7]*a[12]; tot+=(udpint)a[8]*a[11]; tot+=(udpint)a[9]*a[10]; tot*=2; t+=tot;  spint v19=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[20]; tot+=(udpint)a[1]*a[19]; tot+=(udpint)a[2]*a[18]; tot+=(udpint)a[3]*a[17]; tot+=(udpint)a[4]*a[16]; tot+=(udpint)a[5]*a[15]; tot+=(udpint)a[6]*a[14]; tot+=(udpint)a[7]*a[13]; tot+=(udpint)a[8]*a[12]; tot+=(udpint)a[9]*a[11]; tot*=2; tot+=(udpint)a[10]*a[10]; t+=tot;  spint v20=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[21]; tot+=(udpint)a[1]*a[20]; tot+=(udpint)a[2]*a[19]; tot+=(udpint)a[3]*a[18]; tot+=(udpint)a[4]*a[17]; tot+=(udpint)a[5]*a[16]; tot+=(udpint)a[6]*a[15]; tot+=(udpint)a[7]*a[14]; tot+=(udpint)a[8]*a[13]; tot+=(udpint)a[9]*a[12]; tot+=(udpint)a[10]*a[11]; tot*=2; t+=tot;  spint v21=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[22]; tot+=(udpint)a[1]*a[21]; tot+=(udpint)a[2]*a[20]; tot+=(udpint)a[3]*a[19]; tot+=(udpint)a[4]*a[18]; tot+=(udpint)a[5]*a[17]; tot+=(udpint)a[6]*a[16]; tot+=(udpint)a[7]*a[15]; tot+=(udpint)a[8]*a[14]; tot+=(udpint)a[9]*a[13]; tot+=(udpint)a[10]*a[12]; tot*=2; tot+=(udpint)a[11]*a[11]; t+=tot;  spint v22=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[23]; tot+=(udpint)a[1]*a[22]; tot+=(udpint)a[2]*a[21]; tot+=(udpint)a[3]*a[20]; tot+=(udpint)a[4]*a[19]; tot+=(udpint)a[5]*a[18]; tot+=(udpint)a[6]*a[17]; tot+=(udpint)a[7]*a[16]; tot+=(udpint)a[8]*a[15]; tot+=(udpint)a[9]*a[14]; tot+=(udpint)a[10]*a[13]; tot+=(udpint)a[11]*a[12]; tot*=2; t+=tot;  spint v23=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[24]; tot+=(udpint)a[1]*a[23]; tot+=(udpint)a[2]*a[22]; tot+=(udpint)a[3]*a[21]; tot+=(udpint)a[4]*a[20]; tot+=(udpint)a[5]*a[19]; tot+=(udpint)a[6]*a[18]; tot+=(udpint)a[7]*a[17]; tot+=(udpint)a[8]*a[16]; tot+=(udpint)a[9]*a[15]; tot+=(udpint)a[10]*a[14]; tot+=(udpint)a[11]*a[13]; tot*=2; tot+=(udpint)a[12]*a[12]; t+=tot;  spint v24=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[25]; tot+=(udpint)a[1]*a[24]; tot+=(udpint)a[2]*a[23]; tot+=(udpint)a[3]*a[22]; tot+=(udpint)a[4]*a[21]; tot+=(udpint)a[5]*a[20]; tot+=(udpint)a[6]*a[19]; tot+=(udpint)a[7]*a[18]; tot+=(udpint)a[8]*a[17]; tot+=(udpint)a[9]*a[16]; tot+=(udpint)a[10]*a[15]; tot+=(udpint)a[11]*a[14]; tot+=(udpint)a[12]*a[13]; tot*=2; t+=tot;  spint v25=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[26]; tot+=(udpint)a[1]*a[25]; tot+=(udpint)a[2]*a[24]; tot+=(udpint)a[3]*a[23]; tot+=(udpint)a[4]*a[22]; tot+=(udpint)a[5]*a[21]; tot+=(udpint)a[6]*a[20]; tot+=(udpint)a[7]*a[19]; tot+=(udpint)a[8]*a[18]; tot+=(udpint)a[9]*a[17]; tot+=(udpint)a[10]*a[16]; tot+=(udpint)a[11]*a[15]; tot+=(udpint)a[12]*a[14]; tot*=2; tot+=(udpint)a[13]*a[13]; t+=tot;  spint v26=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[27]; tot+=(udpint)a[1]*a[26]; tot+=(udpint)a[2]*a[25]; tot+=(udpint)a[3]*a[24]; tot+=(udpint)a[4]*a[23]; tot+=(udpint)a[5]*a[22]; tot+=(udpint)a[6]*a[21]; tot+=(udpint)a[7]*a[20]; tot+=(udpint)a[8]*a[19]; tot+=(udpint)a[9]*a[18]; tot+=(udpint)a[10]*a[17]; tot+=(udpint)a[11]*a[16]; tot+=(udpint)a[12]*a[15]; tot+=(udpint)a[13]*a[14]; tot*=2; t+=tot;  spint v27=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[28]; tot+=(udpint)a[1]*a[27]; tot+=(udpint)a[2]*a[26]; tot+=(udpint)a[3]*a[25]; tot+=(udpint)a[4]*a[24]; tot+=(udpint)a[5]*a[23]; tot+=(udpint)a[6]*a[22]; tot+=(udpint)a[7]*a[21]; tot+=(udpint)a[8]*a[20]; tot+=(udpint)a[9]*a[19]; tot+=(udpint)a[10]*a[18]; tot+=(udpint)a[11]*a[17]; tot+=(udpint)a[12]*a[16]; tot+=(udpint)a[13]*a[15]; tot*=2; tot+=(udpint)a[14]*a[14]; t+=tot;  spint v28=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[29]; tot+=(udpint)a[1]*a[28]; tot+=(udpint)a[2]*a[27]; tot+=(udpint)a[3]*a[26]; tot+=(udpint)a[4]*a[25]; tot+=(udpint)a[5]*a[24]; tot+=(udpint)a[6]*a[23]; tot+=(udpint)a[7]*a[22]; tot+=(udpint)a[8]*a[21]; tot+=(udpint)a[9]*a[20]; tot+=(udpint)a[10]*a[19]; tot+=(udpint)a[11]*a[18]; tot+=(udpint)a[12]*a[17]; tot+=(udpint)a[13]*a[16]; tot+=(udpint)a[14]*a[15]; tot*=2; t+=tot;  spint v29=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[30]; tot+=(udpint)a[1]*a[29]; tot+=(udpint)a[2]*a[28]; tot+=(udpint)a[3]*a[27]; tot+=(udpint)a[4]*a[26]; tot+=(udpint)a[5]*a[25]; tot+=(udpint)a[6]*a[24]; tot+=(udpint)a[7]*a[23]; tot+=(udpint)a[8]*a[22]; tot+=(udpint)a[9]*a[21]; tot+=(udpint)a[10]*a[20]; tot+=(udpint)a[11]*a[19]; tot+=(udpint)a[12]*a[18]; tot+=(udpint)a[13]*a[17]; tot+=(udpint)a[14]*a[16]; tot*=2; tot+=(udpint)a[15]*a[15]; t+=tot;  spint v30=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[31]; tot+=(udpint)a[1]*a[30]; tot+=(udpint)a[2]*a[29]; tot+=(udpint)a[3]*a[28]; tot+=(udpint)a[4]*a[27]; tot+=(udpint)a[5]*a[26]; tot+=(udpint)a[6]*a[25]; tot+=(udpint)a[7]*a[24]; tot+=(udpint)a[8]*a[23]; tot+=(udpint)a[9]*a[22]; tot+=(udpint)a[10]*a[21]; tot+=(udpint)a[11]*a[20]; tot+=(udpint)a[12]*a[19]; tot+=(udpint)a[13]*a[18]; tot+=(udpint)a[14]*a[17]; tot+=(udpint)a[15]*a[16]; tot*=2; t+=tot;  spint v31=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[32]; tot+=(udpint)a[1]*a[31]; tot+=(udpint)a[2]*a[30]; tot+=(udpint)a[3]*a[29]; tot+=(udpint)a[4]*a[28]; tot+=(udpint)a[5]*a[27]; tot+=(udpint)a[6]*a[26]; tot+=(udpint)a[7]*a[25]; tot+=(udpint)a[8]*a[24]; tot+=(udpint)a[9]*a[23]; tot+=(udpint)a[10]*a[22]; tot+=(udpint)a[11]*a[21]; tot+=(udpint)a[12]*a[20]; tot+=(udpint)a[13]*a[19]; tot+=(udpint)a[14]*a[18]; tot+=(udpint)a[15]*a[17]; tot*=2; tot+=(udpint)a[16]*a[16]; t+=tot;  spint v32=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[33]; tot+=(udpint)a[1]*a[32]; tot+=(udpint)a[2]*a[31]; tot+=(udpint)a[3]*a[30]; tot+=(udpint)a[4]*a[29]; tot+=(udpint)a[5]*a[28]; tot+=(udpint)a[6]*a[27]; tot+=(udpint)a[7]*a[26]; tot+=(udpint)a[8]*a[25]; tot+=(udpint)a[9]*a[24]; tot+=(udpint)a[10]*a[23]; tot+=(udpint)a[11]*a[22]; tot+=(udpint)a[12]*a[21]; tot+=(udpint)a[13]*a[20]; tot+=(udpint)a[14]*a[19]; tot+=(udpint)a[15]*a[18]; tot+=(udpint)a[16]*a[17]; tot*=2; t+=tot;  spint v33=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[34]; tot+=(udpint)a[1]*a[33]; tot+=(udpint)a[2]*a[32]; tot+=(udpint)a[3]*a[31]; tot+=(udpint)a[4]*a[30]; tot+=(udpint)a[5]*a[29]; tot+=(udpint)a[6]*a[28]; tot+=(udpint)a[7]*a[27]; tot+=(udpint)a[8]*a[26]; tot+=(udpint)a[9]*a[25]; tot+=(udpint)a[10]*a[24]; tot+=(udpint)a[11]*a[23]; tot+=(udpint)a[12]*a[22]; tot+=(udpint)a[13]*a[21]; tot+=(udpint)a[14]*a[20]; tot+=(udpint)a[15]*a[19]; tot+=(udpint)a[16]*a[18]; tot*=2; tot+=(udpint)a[17]*a[17]; t+=tot;  spint v34=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[35]; tot+=(udpint)a[1]*a[34]; tot+=(udpint)a[2]*a[33]; tot+=(udpint)a[3]*a[32]; tot+=(udpint)a[4]*a[31]; tot+=(udpint)a[5]*a[30]; tot+=(udpint)a[6]*a[29]; tot+=(udpint)a[7]*a[28]; tot+=(udpint)a[8]*a[27]; tot+=(udpint)a[9]*a[26]; tot+=(udpint)a[10]*a[25]; tot+=(udpint)a[11]*a[24]; tot+=(udpint)a[12]*a[23]; tot+=(udpint)a[13]*a[22]; tot+=(udpint)a[14]*a[21]; tot+=(udpint)a[15]*a[20]; tot+=(udpint)a[16]*a[19]; tot+=(udpint)a[17]*a[18]; tot*=2; t+=tot;  spint v35=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[36]; tot+=(udpint)a[1]*a[35]; tot+=(udpint)a[2]*a[34]; tot+=(udpint)a[3]*a[33]; tot+=(udpint)a[4]*a[32]; tot+=(udpint)a[5]*a[31]; tot+=(udpint)a[6]*a[30]; tot+=(udpint)a[7]*a[29]; tot+=(udpint)a[8]*a[28]; tot+=(udpint)a[9]*a[27]; tot+=(udpint)a[10]*a[26]; tot+=(udpint)a[11]*a[25]; tot+=(udpint)a[12]*a[24]; tot+=(udpint)a[13]*a[23]; tot+=(udpint)a[14]*a[22]; tot+=(udpint)a[15]*a[21]; tot+=(udpint)a[16]*a[20]; tot+=(udpint)a[17]*a[19]; tot*=2; tot+=(udpint)a[18]*a[18]; t+=tot;  spint v36=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[37]; tot+=(udpint)a[1]*a[36]; tot+=(udpint)a[2]*a[35]; tot+=(udpint)a[3]*a[34]; tot+=(udpint)a[4]*a[33]; tot+=(udpint)a[5]*a[32]; tot+=(udpint)a[6]*a[31]; tot+=(udpint)a[7]*a[30]; tot+=(udpint)a[8]*a[29]; tot+=(udpint)a[9]*a[28]; tot+=(udpint)a[10]*a[27]; tot+=(udpint)a[11]*a[26]; tot+=(udpint)a[12]*a[25]; tot+=(udpint)a[13]*a[24]; tot+=(udpint)a[14]*a[23]; tot+=(udpint)a[15]*a[22]; tot+=(udpint)a[16]*a[21]; tot+=(udpint)a[17]*a[20]; tot+=(udpint)a[18]*a[19]; tot*=2; t+=tot;  spint v37=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[38]; tot+=(udpint)a[1]*a[37]; tot+=(udpint)a[2]*a[36]; tot+=(udpint)a[3]*a[35]; tot+=(udpint)a[4]*a[34]; tot+=(udpint)a[5]*a[33]; tot+=(udpint)a[6]*a[32]; tot+=(udpint)a[7]*a[31]; tot+=(udpint)a[8]*a[30]; tot+=(udpint)a[9]*a[29]; tot+=(udpint)a[10]*a[28]; tot+=(udpint)a[11]*a[27]; tot+=(udpint)a[12]*a[26]; tot+=(udpint)a[13]*a[25]; tot+=(udpint)a[14]*a[24]; tot+=(udpint)a[15]*a[23]; tot+=(udpint)a[16]*a[22]; tot+=(udpint)a[17]*a[21]; tot+=(udpint)a[18]*a[20]; tot*=2; tot+=(udpint)a[19]*a[19]; t+=tot;  spint v38=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[39]; tot+=(udpint)a[1]*a[38]; tot+=(udpint)a[2]*a[37]; tot+=(udpint)a[3]*a[36]; tot+=(udpint)a[4]*a[35]; tot+=(udpint)a[5]*a[34]; tot+=(udpint)a[6]*a[33]; tot+=(udpint)a[7]*a[32]; tot+=(udpint)a[8]*a[31]; tot+=(udpint)a[9]*a[30]; tot+=(udpint)a[10]*a[29]; tot+=(udpint)a[11]*a[28]; tot+=(udpint)a[12]*a[27]; tot+=(udpint)a[13]*a[26]; tot+=(udpint)a[14]*a[25]; tot+=(udpint)a[15]*a[24]; tot+=(udpint)a[16]*a[23]; tot+=(udpint)a[17]*a[22]; tot+=(udpint)a[18]*a[21]; tot+=(udpint)a[19]*a[20]; tot*=2; t+=tot;  spint v39=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[40]; tot+=(udpint)a[1]*a[39]; tot+=(udpint)a[2]*a[38]; tot+=(udpint)a[3]*a[37]; tot+=(udpint)a[4]*a[36]; tot+=(udpint)a[5]*a[35]; tot+=(udpint)a[6]*a[34]; tot+=(udpint)a[7]*a[33]; tot+=(udpint)a[8]*a[32]; tot+=(udpint)a[9]*a[31]; tot+=(udpint)a[10]*a[30]; tot+=(udpint)a[11]*a[29]; tot+=(udpint)a[12]*a[28]; tot+=(udpint)a[13]*a[27]; tot+=(udpint)a[14]*a[26]; tot+=(udpint)a[15]*a[25]; tot+=(udpint)a[16]*a[24]; tot+=(udpint)a[17]*a[23]; tot+=(udpint)a[18]*a[22]; tot+=(udpint)a[19]*a[21]; tot*=2; tot+=(udpint)a[20]*a[20]; t+=tot;  spint v40=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[41]; tot+=(udpint)a[1]*a[40]; tot+=(udpint)a[2]*a[39]; tot+=(udpint)a[3]*a[38]; tot+=(udpint)a[4]*a[37]; tot+=(udpint)a[5]*a[36]; tot+=(udpint)a[6]*a[35]; tot+=(udpint)a[7]*a[34]; tot+=(udpint)a[8]*a[33]; tot+=(udpint)a[9]*a[32]; tot+=(udpint)a[10]*a[31]; tot+=(udpint)a[11]*a[30]; tot+=(udpint)a[12]*a[29]; tot+=(udpint)a[13]*a[28]; tot+=(udpint)a[14]*a[27]; tot+=(udpint)a[15]*a[26]; tot+=(udpint)a[16]*a[25]; tot+=(udpint)a[17]*a[24]; tot+=(udpint)a[18]*a[23]; tot+=(udpint)a[19]*a[22]; tot+=(udpint)a[20]*a[21]; tot*=2; t+=tot;  spint v41=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[42]; tot+=(udpint)a[1]*a[41]; tot+=(udpint)a[2]*a[40]; tot+=(udpint)a[3]*a[39]; tot+=(udpint)a[4]*a[38]; tot+=(udpint)a[5]*a[37]; tot+=(udpint)a[6]*a[36]; tot+=(udpint)a[7]*a[35]; tot+=(udpint)a[8]*a[34]; tot+=(udpint)a[9]*a[33]; tot+=(udpint)a[10]*a[32]; tot+=(udpint)a[11]*a[31]; tot+=(udpint)a[12]*a[30]; tot+=(udpint)a[13]*a[29]; tot+=(udpint)a[14]*a[28]; tot+=(udpint)a[15]*a[27]; tot+=(udpint)a[16]*a[26]; tot+=(udpint)a[17]*a[25]; tot+=(udpint)a[18]*a[24]; tot+=(udpint)a[19]*a[23]; tot+=(udpint)a[20]*a[22]; tot*=2; tot+=(udpint)a[21]*a[21]; t+=tot;  spint v42=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[43]; tot+=(udpint)a[1]*a[42]; tot+=(udpint)a[2]*a[41]; tot+=(udpint)a[3]*a[40]; tot+=(udpint)a[4]*a[39]; tot+=(udpint)a[5]*a[38]; tot+=(udpint)a[6]*a[37]; tot+=(udpint)a[7]*a[36]; tot+=(udpint)a[8]*a[35]; tot+=(udpint)a[9]*a[34]; tot+=(udpint)a[10]*a[33]; tot+=(udpint)a[11]*a[32]; tot+=(udpint)a[12]*a[31]; tot+=(udpint)a[13]*a[30]; tot+=(udpint)a[14]*a[29]; tot+=(udpint)a[15]*a[28]; tot+=(udpint)a[16]*a[27]; tot+=(udpint)a[17]*a[26]; tot+=(udpint)a[18]*a[25]; tot+=(udpint)a[19]*a[24]; tot+=(udpint)a[20]*a[23]; tot+=(udpint)a[21]*a[22]; tot*=2; t+=tot;  spint v43=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[44]; tot+=(udpint)a[1]*a[43]; tot+=(udpint)a[2]*a[42]; tot+=(udpint)a[3]*a[41]; tot+=(udpint)a[4]*a[40]; tot+=(udpint)a[5]*a[39]; tot+=(udpint)a[6]*a[38]; tot+=(udpint)a[7]*a[37]; tot+=(udpint)a[8]*a[36]; tot+=(udpint)a[9]*a[35]; tot+=(udpint)a[10]*a[34]; tot+=(udpint)a[11]*a[33]; tot+=(udpint)a[12]*a[32]; tot+=(udpint)a[13]*a[31]; tot+=(udpint)a[14]*a[30]; tot+=(udpint)a[15]*a[29]; tot+=(udpint)a[16]*a[28]; tot+=(udpint)a[17]*a[27]; tot+=(udpint)a[18]*a[26]; tot+=(udpint)a[19]*a[25]; tot+=(udpint)a[20]*a[24]; tot+=(udpint)a[21]*a[23]; tot*=2; tot+=(udpint)a[22]*a[22]; t+=tot;  spint v44=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[45]; tot+=(udpint)a[1]*a[44]; tot+=(udpint)a[2]*a[43]; tot+=(udpint)a[3]*a[42]; tot+=(udpint)a[4]*a[41]; tot+=(udpint)a[5]*a[40]; tot+=(udpint)a[6]*a[39]; tot+=(udpint)a[7]*a[38]; tot+=(udpint)a[8]*a[37]; tot+=(udpint)a[9]*a[36]; tot+=(udpint)a[10]*a[35]; tot+=(udpint)a[11]*a[34]; tot+=(udpint)a[12]*a[33]; tot+=(udpint)a[13]*a[32]; tot+=(udpint)a[14]*a[31]; tot+=(udpint)a[15]*a[30]; tot+=(udpint)a[16]*a[29]; tot+=(udpint)a[17]*a[28]; tot+=(udpint)a[18]*a[27]; tot+=(udpint)a[19]*a[26]; tot+=(udpint)a[20]*a[25]; tot+=(udpint)a[21]*a[24]; tot+=(udpint)a[22]*a[23]; tot*=2; t+=tot;  spint v45=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[46]; tot+=(udpint)a[1]*a[45]; tot+=(udpint)a[2]*a[44]; tot+=(udpint)a[3]*a[43]; tot+=(udpint)a[4]*a[42]; tot+=(udpint)a[5]*a[41]; tot+=(udpint)a[6]*a[40]; tot+=(udpint)a[7]*a[39]; tot+=(udpint)a[8]*a[38]; tot+=(udpint)a[9]*a[37]; tot+=(udpint)a[10]*a[36]; tot+=(udpint)a[11]*a[35]; tot+=(udpint)a[12]*a[34]; tot+=(udpint)a[13]*a[33]; tot+=(udpint)a[14]*a[32]; tot+=(udpint)a[15]*a[31]; tot+=(udpint)a[16]*a[30]; tot+=(udpint)a[17]*a[29]; tot+=(udpint)a[18]*a[28]; tot+=(udpint)a[19]*a[27]; tot+=(udpint)a[20]*a[26]; tot+=(udpint)a[21]*a[25]; tot+=(udpint)a[22]*a[24]; tot*=2; tot+=(udpint)a[23]*a[23]; t+=tot;  spint v46=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[47]; tot+=(udpint)a[1]*a[46]; tot+=(udpint)a[2]*a[45]; tot+=(udpint)a[3]*a[44]; tot+=(udpint)a[4]*a[43]; tot+=(udpint)a[5]*a[42]; tot+=(udpint)a[6]*a[41]; tot+=(udpint)a[7]*a[40]; tot+=(udpint)a[8]*a[39]; tot+=(udpint)a[9]*a[38]; tot+=(udpint)a[10]*a[37]; tot+=(udpint)a[11]*a[36]; tot+=(udpint)a[12]*a[35]; tot+=(udpint)a[13]*a[34]; tot+=(udpint)a[14]*a[33]; tot+=(udpint)a[15]*a[32]; tot+=(udpint)a[16]*a[31]; tot+=(udpint)a[17]*a[30]; tot+=(udpint)a[18]*a[29]; tot+=(udpint)a[19]*a[28]; tot+=(udpint)a[20]*a[27]; tot+=(udpint)a[21]*a[26]; tot+=(udpint)a[22]*a[25]; tot+=(udpint)a[23]*a[24]; tot*=2; t+=tot;  spint v47=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[48]; tot+=(udpint)a[1]*a[47]; tot+=(udpint)a[2]*a[46]; tot+=(udpint)a[3]*a[45]; tot+=(udpint)a[4]*a[44]; tot+=(udpint)a[5]*a[43]; tot+=(udpint)a[6]*a[42]; tot+=(udpint)a[7]*a[41]; tot+=(udpint)a[8]*a[40]; tot+=(udpint)a[9]*a[39]; tot+=(udpint)a[10]*a[38]; tot+=(udpint)a[11]*a[37]; tot+=(udpint)a[12]*a[36]; tot+=(udpint)a[13]*a[35]; tot+=(udpint)a[14]*a[34]; tot+=(udpint)a[15]*a[33]; tot+=(udpint)a[16]*a[32]; tot+=(udpint)a[17]*a[31]; tot+=(udpint)a[18]*a[30]; tot+=(udpint)a[19]*a[29]; tot+=(udpint)a[20]*a[28]; tot+=(udpint)a[21]*a[27]; tot+=(udpint)a[22]*a[26]; tot+=(udpint)a[23]*a[25]; tot*=2; tot+=(udpint)a[24]*a[24]; t+=tot;  spint v48=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[49]; tot+=(udpint)a[1]*a[48]; tot+=(udpint)a[2]*a[47]; tot+=(udpint)a[3]*a[46]; tot+=(udpint)a[4]*a[45]; tot+=(udpint)a[5]*a[44]; tot+=(udpint)a[6]*a[43]; tot+=(udpint)a[7]*a[42]; tot+=(udpint)a[8]*a[41]; tot+=(udpint)a[9]*a[40]; tot+=(udpint)a[10]*a[39]; tot+=(udpint)a[11]*a[38]; tot+=(udpint)a[12]*a[37]; tot+=(udpint)a[13]*a[36]; tot+=(udpint)a[14]*a[35]; tot+=(udpint)a[15]*a[34]; tot+=(udpint)a[16]*a[33]; tot+=(udpint)a[17]*a[32]; tot+=(udpint)a[18]*a[31]; tot+=(udpint)a[19]*a[30]; tot+=(udpint)a[20]*a[29]; tot+=(udpint)a[21]*a[28]; tot+=(udpint)a[22]*a[27]; tot+=(udpint)a[23]*a[26]; tot+=(udpint)a[24]*a[25]; tot*=2; t+=tot;  spint v49=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[50]; tot+=(udpint)a[1]*a[49]; tot+=(udpint)a[2]*a[48]; tot+=(udpint)a[3]*a[47]; tot+=(udpint)a[4]*a[46]; tot+=(udpint)a[5]*a[45]; tot+=(udpint)a[6]*a[44]; tot+=(udpint)a[7]*a[43]; tot+=(udpint)a[8]*a[42]; tot+=(udpint)a[9]*a[41]; tot+=(udpint)a[10]*a[40]; tot+=(udpint)a[11]*a[39]; tot+=(udpint)a[12]*a[38]; tot+=(udpint)a[13]*a[37]; tot+=(udpint)a[14]*a[36]; tot+=(udpint)a[15]*a[35]; tot+=(udpint)a[16]*a[34]; tot+=(udpint)a[17]*a[33]; tot+=(udpint)a[18]*a[32]; tot+=(udpint)a[19]*a[31]; tot+=(udpint)a[20]*a[30]; tot+=(udpint)a[21]*a[29]; tot+=(udpint)a[22]*a[28]; tot+=(udpint)a[23]*a[27]; tot+=(udpint)a[24]*a[26]; tot*=2; tot+=(udpint)a[25]*a[25]; t+=tot;  spint v50=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[51]; tot+=(udpint)a[1]*a[50]; tot+=(udpint)a[2]*a[49]; tot+=(udpint)a[3]*a[48]; tot+=(udpint)a[4]*a[47]; tot+=(udpint)a[5]*a[46]; tot+=(udpint)a[6]*a[45]; tot+=(udpint)a[7]*a[44]; tot+=(udpint)a[8]*a[43]; tot+=(udpint)a[9]*a[42]; tot+=(udpint)a[10]*a[41]; tot+=(udpint)a[11]*a[40]; tot+=(udpint)a[12]*a[39]; tot+=(udpint)a[13]*a[38]; tot+=(udpint)a[14]*a[37]; tot+=(udpint)a[15]*a[36]; tot+=(udpint)a[16]*a[35]; tot+=(udpint)a[17]*a[34]; tot+=(udpint)a[18]*a[33]; tot+=(udpint)a[19]*a[32]; tot+=(udpint)a[20]*a[31]; tot+=(udpint)a[21]*a[30]; tot+=(udpint)a[22]*a[29]; tot+=(udpint)a[23]*a[28]; tot+=(udpint)a[24]*a[27]; tot+=(udpint)a[25]*a[26]; tot*=2; t+=tot;  spint v51=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[52]; tot+=(udpint)a[1]*a[51]; tot+=(udpint)a[2]*a[50]; tot+=(udpint)a[3]*a[49]; tot+=(udpint)a[4]*a[48]; tot+=(udpint)a[5]*a[47]; tot+=(udpint)a[6]*a[46]; tot+=(udpint)a[7]*a[45]; tot+=(udpint)a[8]*a[44]; tot+=(udpint)a[9]*a[43]; tot+=(udpint)a[10]*a[42]; tot+=(udpint)a[11]*a[41]; tot+=(udpint)a[12]*a[40]; tot+=(udpint)a[13]*a[39]; tot+=(udpint)a[14]*a[38]; tot+=(udpint)a[15]*a[37]; tot+=(udpint)a[16]*a[36]; tot+=(udpint)a[17]*a[35]; tot+=(udpint)a[18]*a[34]; tot+=(udpint)a[19]*a[33]; tot+=(udpint)a[20]*a[32]; tot+=(udpint)a[21]*a[31]; tot+=(udpint)a[22]*a[30]; tot+=(udpint)a[23]*a[29]; tot+=(udpint)a[24]*a[28]; tot+=(udpint)a[25]*a[27]; tot*=2; tot+=(udpint)a[26]*a[26]; t+=tot;  spint v52=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[53]; tot+=(udpint)a[1]*a[52]; tot+=(udpint)a[2]*a[51]; tot+=(udpint)a[3]*a[50]; tot+=(udpint)a[4]*a[49]; tot+=(udpint)a[5]*a[48]; tot+=(udpint)a[6]*a[47]; tot+=(udpint)a[7]*a[46]; tot+=(udpint)a[8]*a[45]; tot+=(udpint)a[9]*a[44]; tot+=(udpint)a[10]*a[43]; tot+=(udpint)a[11]*a[42]; tot+=(udpint)a[12]*a[41]; tot+=(udpint)a[13]*a[40]; tot+=(udpint)a[14]*a[39]; tot+=(udpint)a[15]*a[38]; tot+=(udpint)a[16]*a[37]; tot+=(udpint)a[17]*a[36]; tot+=(udpint)a[18]*a[35]; tot+=(udpint)a[19]*a[34]; tot+=(udpint)a[20]*a[33]; tot+=(udpint)a[21]*a[32]; tot+=(udpint)a[22]*a[31]; tot+=(udpint)a[23]*a[30]; tot+=(udpint)a[24]*a[29]; tot+=(udpint)a[25]*a[28]; tot+=(udpint)a[26]*a[27]; tot*=2; t+=tot;  spint v53=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[54]; tot+=(udpint)a[1]*a[53]; tot+=(udpint)a[2]*a[52]; tot+=(udpint)a[3]*a[51]; tot+=(udpint)a[4]*a[50]; tot+=(udpint)a[5]*a[49]; tot+=(udpint)a[6]*a[48]; tot+=(udpint)a[7]*a[47]; tot+=(udpint)a[8]*a[46]; tot+=(udpint)a[9]*a[45]; tot+=(udpint)a[10]*a[44]; tot+=(udpint)a[11]*a[43]; tot+=(udpint)a[12]*a[42]; tot+=(udpint)a[13]*a[41]; tot+=(udpint)a[14]*a[40]; tot+=(udpint)a[15]*a[39]; tot+=(udpint)a[16]*a[38]; tot+=(udpint)a[17]*a[37]; tot+=(udpint)a[18]*a[36]; tot+=(udpint)a[19]*a[35]; tot+=(udpint)a[20]*a[34]; tot+=(udpint)a[21]*a[33]; tot+=(udpint)a[22]*a[32]; tot+=(udpint)a[23]*a[31]; tot+=(udpint)a[24]*a[30]; tot+=(udpint)a[25]*a[29]; tot+=(udpint)a[26]*a[28]; tot*=2; tot+=(udpint)a[27]*a[27]; t+=tot;  spint v54=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[55]; tot+=(udpint)a[1]*a[54]; tot+=(udpint)a[2]*a[53]; tot+=(udpint)a[3]*a[52]; tot+=(udpint)a[4]*a[51]; tot+=(udpint)a[5]*a[50]; tot+=(udpint)a[6]*a[49]; tot+=(udpint)a[7]*a[48]; tot+=(udpint)a[8]*a[47]; tot+=(udpint)a[9]*a[46]; tot+=(udpint)a[10]*a[45]; tot+=(udpint)a[11]*a[44]; tot+=(udpint)a[12]*a[43]; tot+=(udpint)a[13]*a[42]; tot+=(udpint)a[14]*a[41]; tot+=(udpint)a[15]*a[40]; tot+=(udpint)a[16]*a[39]; tot+=(udpint)a[17]*a[38]; tot+=(udpint)a[18]*a[37]; tot+=(udpint)a[19]*a[36]; tot+=(udpint)a[20]*a[35]; tot+=(udpint)a[21]*a[34]; tot+=(udpint)a[22]*a[33]; tot+=(udpint)a[23]*a[32]; tot+=(udpint)a[24]*a[31]; tot+=(udpint)a[25]*a[30]; tot+=(udpint)a[26]*a[29]; tot+=(udpint)a[27]*a[28]; tot*=2; t+=tot;  spint v55=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[56]; tot+=(udpint)a[1]*a[55]; tot+=(udpint)a[2]*a[54]; tot+=(udpint)a[3]*a[53]; tot+=(udpint)a[4]*a[52]; tot+=(udpint)a[5]*a[51]; tot+=(udpint)a[6]*a[50]; tot+=(udpint)a[7]*a[49]; tot+=(udpint)a[8]*a[48]; tot+=(udpint)a[9]*a[47]; tot+=(udpint)a[10]*a[46]; tot+=(udpint)a[11]*a[45]; tot+=(udpint)a[12]*a[44]; tot+=(udpint)a[13]*a[43]; tot+=(udpint)a[14]*a[42]; tot+=(udpint)a[15]*a[41]; tot+=(udpint)a[16]*a[40]; tot+=(udpint)a[17]*a[39]; tot+=(udpint)a[18]*a[38]; tot+=(udpint)a[19]*a[37]; tot+=(udpint)a[20]*a[36]; tot+=(udpint)a[21]*a[35]; tot+=(udpint)a[22]*a[34]; tot+=(udpint)a[23]*a[33]; tot+=(udpint)a[24]*a[32]; tot+=(udpint)a[25]*a[31]; tot+=(udpint)a[26]*a[30]; tot+=(udpint)a[27]*a[29]; tot*=2; tot+=(udpint)a[28]*a[28]; t+=tot;  spint v56=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[57]; tot+=(udpint)a[1]*a[56]; tot+=(udpint)a[2]*a[55]; tot+=(udpint)a[3]*a[54]; tot+=(udpint)a[4]*a[53]; tot+=(udpint)a[5]*a[52]; tot+=(udpint)a[6]*a[51]; tot+=(udpint)a[7]*a[50]; tot+=(udpint)a[8]*a[49]; tot+=(udpint)a[9]*a[48]; tot+=(udpint)a[10]*a[47]; tot+=(udpint)a[11]*a[46]; tot+=(udpint)a[12]*a[45]; tot+=(udpint)a[13]*a[44]; tot+=(udpint)a[14]*a[43]; tot+=(udpint)a[15]*a[42]; tot+=(udpint)a[16]*a[41]; tot+=(udpint)a[17]*a[40]; tot+=(udpint)a[18]*a[39]; tot+=(udpint)a[19]*a[38]; tot+=(udpint)a[20]*a[37]; tot+=(udpint)a[21]*a[36]; tot+=(udpint)a[22]*a[35]; tot+=(udpint)a[23]*a[34]; tot+=(udpint)a[24]*a[33]; tot+=(udpint)a[25]*a[32]; tot+=(udpint)a[26]*a[31]; tot+=(udpint)a[27]*a[30]; tot+=(udpint)a[28]*a[29]; tot*=2; t+=tot;  spint v57=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[58]; tot+=(udpint)a[1]*a[57]; tot+=(udpint)a[2]*a[56]; tot+=(udpint)a[3]*a[55]; tot+=(udpint)a[4]*a[54]; tot+=(udpint)a[5]*a[53]; tot+=(udpint)a[6]*a[52]; tot+=(udpint)a[7]*a[51]; tot+=(udpint)a[8]*a[50]; tot+=(udpint)a[9]*a[49]; tot+=(udpint)a[10]*a[48]; tot+=(udpint)a[11]*a[47]; tot+=(udpint)a[12]*a[46]; tot+=(udpint)a[13]*a[45]; tot+=(udpint)a[14]*a[44]; tot+=(udpint)a[15]*a[43]; tot+=(udpint)a[16]*a[42]; tot+=(udpint)a[17]*a[41]; tot+=(udpint)a[18]*a[40]; tot+=(udpint)a[19]*a[39]; tot+=(udpint)a[20]*a[38]; tot+=(udpint)a[21]*a[37]; tot+=(udpint)a[22]*a[36]; tot+=(udpint)a[23]*a[35]; tot+=(udpint)a[24]*a[34]; tot+=(udpint)a[25]*a[33]; tot+=(udpint)a[26]*a[32]; tot+=(udpint)a[27]*a[31]; tot+=(udpint)a[28]*a[30]; tot*=2; tot+=(udpint)a[29]*a[29]; t+=tot;  spint v58=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[59]; tot+=(udpint)a[1]*a[58]; tot+=(udpint)a[2]*a[57]; tot+=(udpint)a[3]*a[56]; tot+=(udpint)a[4]*a[55]; tot+=(udpint)a[5]*a[54]; tot+=(udpint)a[6]*a[53]; tot+=(udpint)a[7]*a[52]; tot+=(udpint)a[8]*a[51]; tot+=(udpint)a[9]*a[50]; tot+=(udpint)a[10]*a[49]; tot+=(udpint)a[11]*a[48]; tot+=(udpint)a[12]*a[47]; tot+=(udpint)a[13]*a[46]; tot+=(udpint)a[14]*a[45]; tot+=(udpint)a[15]*a[44]; tot+=(udpint)a[16]*a[43]; tot+=(udpint)a[17]*a[42]; tot+=(udpint)a[18]*a[41]; tot+=(udpint)a[19]*a[40]; tot+=(udpint)a[20]*a[39]; tot+=(udpint)a[21]*a[38]; tot+=(udpint)a[22]*a[37]; tot+=(udpint)a[23]*a[36]; tot+=(udpint)a[24]*a[35]; tot+=(udpint)a[25]*a[34]; tot+=(udpint)a[26]*a[33]; tot+=(udpint)a[27]*a[32]; tot+=(udpint)a[28]*a[31]; tot+=(udpint)a[29]*a[30]; tot*=2; t+=tot;  spint v59=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[60]; tot+=(udpint)a[1]*a[59]; tot+=(udpint)a[2]*a[58]; tot+=(udpint)a[3]*a[57]; tot+=(udpint)a[4]*a[56]; tot+=(udpint)a[5]*a[55]; tot+=(udpint)a[6]*a[54]; tot+=(udpint)a[7]*a[53]; tot+=(udpint)a[8]*a[52]; tot+=(udpint)a[9]*a[51]; tot+=(udpint)a[10]*a[50]; tot+=(udpint)a[11]*a[49]; tot+=(udpint)a[12]*a[48]; tot+=(udpint)a[13]*a[47]; tot+=(udpint)a[14]*a[46]; tot+=(udpint)a[15]*a[45]; tot+=(udpint)a[16]*a[44]; tot+=(udpint)a[17]*a[43]; tot+=(udpint)a[18]*a[42]; tot+=(udpint)a[19]*a[41]; tot+=(udpint)a[20]*a[40]; tot+=(udpint)a[21]*a[39]; tot+=(udpint)a[22]*a[38]; tot+=(udpint)a[23]*a[37]; tot+=(udpint)a[24]*a[36]; tot+=(udpint)a[25]*a[35]; tot+=(udpint)a[26]*a[34]; tot+=(udpint)a[27]*a[33]; tot+=(udpint)a[28]*a[32]; tot+=(udpint)a[29]*a[31]; tot*=2; tot+=(udpint)a[30]*a[30]; t+=tot;  spint v60=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[61]; tot+=(udpint)a[1]*a[60]; tot+=(udpint)a[2]*a[59]; tot+=(udpint)a[3]*a[58]; tot+=(udpint)a[4]*a[57]; tot+=(udpint)a[5]*a[56]; tot+=(udpint)a[6]*a[55]; tot+=(udpint)a[7]*a[54]; tot+=(udpint)a[8]*a[53]; tot+=(udpint)a[9]*a[52]; tot+=(udpint)a[10]*a[51]; tot+=(udpint)a[11]*a[50]; tot+=(udpint)a[12]*a[49]; tot+=(udpint)a[13]*a[48]; tot+=(udpint)a[14]*a[47]; tot+=(udpint)a[15]*a[46]; tot+=(udpint)a[16]*a[45]; tot+=(udpint)a[17]*a[44]; tot+=(udpint)a[18]*a[43]; tot+=(udpint)a[19]*a[42]; tot+=(udpint)a[20]*a[41]; tot+=(udpint)a[21]*a[40]; tot+=(udpint)a[22]*a[39]; tot+=(udpint)a[23]*a[38]; tot+=(udpint)a[24]*a[37]; tot+=(udpint)a[25]*a[36]; tot+=(udpint)a[26]*a[35]; tot+=(udpint)a[27]*a[34]; tot+=(udpint)a[28]*a[33]; tot+=(udpint)a[29]*a[32]; tot+=(udpint)a[30]*a[31]; tot*=2; t+=tot;  spint v61=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[62]; tot+=(udpint)a[1]*a[61]; tot+=(udpint)a[2]*a[60]; tot+=(udpint)a[3]*a[59]; tot+=(udpint)a[4]*a[58]; tot+=(udpint)a[5]*a[57]; tot+=(udpint)a[6]*a[56]; tot+=(udpint)a[7]*a[55]; tot+=(udpint)a[8]*a[54]; tot+=(udpint)a[9]*a[53]; tot+=(udpint)a[10]*a[52]; tot+=(udpint)a[11]*a[51]; tot+=(udpint)a[12]*a[50]; tot+=(udpint)a[13]*a[49]; tot+=(udpint)a[14]*a[48]; tot+=(udpint)a[15]*a[47]; tot+=(udpint)a[16]*a[46]; tot+=(udpint)a[17]*a[45]; tot+=(udpint)a[18]*a[44]; tot+=(udpint)a[19]*a[43]; tot+=(udpint)a[20]*a[42]; tot+=(udpint)a[21]*a[41]; tot+=(udpint)a[22]*a[40]; tot+=(udpint)a[23]*a[39]; tot+=(udpint)a[24]*a[38]; tot+=(udpint)a[25]*a[37]; tot+=(udpint)a[26]*a[36]; tot+=(udpint)a[27]*a[35]; tot+=(udpint)a[28]*a[34]; tot+=(udpint)a[29]*a[33]; tot+=(udpint)a[30]*a[32]; tot*=2; tot+=(udpint)a[31]*a[31]; t+=tot;  spint v62=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[63]; tot+=(udpint)a[1]*a[62]; tot+=(udpint)a[2]*a[61]; tot+=(udpint)a[3]*a[60]; tot+=(udpint)a[4]*a[59]; tot+=(udpint)a[5]*a[58]; tot+=(udpint)a[6]*a[57]; tot+=(udpint)a[7]*a[56]; tot+=(udpint)a[8]*a[55]; tot+=(udpint)a[9]*a[54]; tot+=(udpint)a[10]*a[53]; tot+=(udpint)a[11]*a[52]; tot+=(udpint)a[12]*a[51]; tot+=(udpint)a[13]*a[50]; tot+=(udpint)a[14]*a[49]; tot+=(udpint)a[15]*a[48]; tot+=(udpint)a[16]*a[47]; tot+=(udpint)a[17]*a[46]; tot+=(udpint)a[18]*a[45]; tot+=(udpint)a[19]*a[44]; tot+=(udpint)a[20]*a[43]; tot+=(udpint)a[21]*a[42]; tot+=(udpint)a[22]*a[41]; tot+=(udpint)a[23]*a[40]; tot+=(udpint)a[24]*a[39]; tot+=(udpint)a[25]*a[38]; tot+=(udpint)a[26]*a[37]; tot+=(udpint)a[27]*a[36]; tot+=(udpint)a[28]*a[35]; tot+=(udpint)a[29]*a[34]; tot+=(udpint)a[30]*a[33]; tot+=(udpint)a[31]*a[32]; tot*=2; t+=tot;  spint v63=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[64]; tot+=(udpint)a[1]*a[63]; tot+=(udpint)a[2]*a[62]; tot+=(udpint)a[3]*a[61]; tot+=(udpint)a[4]*a[60]; tot+=(udpint)a[5]*a[59]; tot+=(udpint)a[6]*a[58]; tot+=(udpint)a[7]*a[57]; tot+=(udpint)a[8]*a[56]; tot+=(udpint)a[9]*a[55]; tot+=(udpint)a[10]*a[54]; tot+=(udpint)a[11]*a[53]; tot+=(udpint)a[12]*a[52]; tot+=(udpint)a[13]*a[51]; tot+=(udpint)a[14]*a[50]; tot+=(udpint)a[15]*a[49]; tot+=(udpint)a[16]*a[48]; tot+=(udpint)a[17]*a[47]; tot+=(udpint)a[18]*a[46]; tot+=(udpint)a[19]*a[45]; tot+=(udpint)a[20]*a[44]; tot+=(udpint)a[21]*a[43]; tot+=(udpint)a[22]*a[42]; tot+=(udpint)a[23]*a[41]; tot+=(udpint)a[24]*a[40]; tot+=(udpint)a[25]*a[39]; tot+=(udpint)a[26]*a[38]; tot+=(udpint)a[27]*a[37]; tot+=(udpint)a[28]*a[36]; tot+=(udpint)a[29]*a[35]; tot+=(udpint)a[30]*a[34]; tot+=(udpint)a[31]*a[33]; tot*=2; tot+=(udpint)a[32]*a[32]; t+=tot;  spint v64=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[65]; tot+=(udpint)a[1]*a[64]; tot+=(udpint)a[2]*a[63]; tot+=(udpint)a[3]*a[62]; tot+=(udpint)a[4]*a[61]; tot+=(udpint)a[5]*a[60]; tot+=(udpint)a[6]*a[59]; tot+=(udpint)a[7]*a[58]; tot+=(udpint)a[8]*a[57]; tot+=(udpint)a[9]*a[56]; tot+=(udpint)a[10]*a[55]; tot+=(udpint)a[11]*a[54]; tot+=(udpint)a[12]*a[53]; tot+=(udpint)a[13]*a[52]; tot+=(udpint)a[14]*a[51]; tot+=(udpint)a[15]*a[50]; tot+=(udpint)a[16]*a[49]; tot+=(udpint)a[17]*a[48]; tot+=(udpint)a[18]*a[47]; tot+=(udpint)a[19]*a[46]; tot+=(udpint)a[20]*a[45]; tot+=(udpint)a[21]*a[44]; tot+=(udpint)a[22]*a[43]; tot+=(udpint)a[23]*a[42]; tot+=(udpint)a[24]*a[41]; tot+=(udpint)a[25]*a[40]; tot+=(udpint)a[26]*a[39]; tot+=(udpint)a[27]*a[38]; tot+=(udpint)a[28]*a[37]; tot+=(udpint)a[29]*a[36]; tot+=(udpint)a[30]*a[35]; tot+=(udpint)a[31]*a[34]; tot+=(udpint)a[32]*a[33]; tot*=2; t+=tot;  spint v65=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[66]; tot+=(udpint)a[1]*a[65]; tot+=(udpint)a[2]*a[64]; tot+=(udpint)a[3]*a[63]; tot+=(udpint)a[4]*a[62]; tot+=(udpint)a[5]*a[61]; tot+=(udpint)a[6]*a[60]; tot+=(udpint)a[7]*a[59]; tot+=(udpint)a[8]*a[58]; tot+=(udpint)a[9]*a[57]; tot+=(udpint)a[10]*a[56]; tot+=(udpint)a[11]*a[55]; tot+=(udpint)a[12]*a[54]; tot+=(udpint)a[13]*a[53]; tot+=(udpint)a[14]*a[52]; tot+=(udpint)a[15]*a[51]; tot+=(udpint)a[16]*a[50]; tot+=(udpint)a[17]*a[49]; tot+=(udpint)a[18]*a[48]; tot+=(udpint)a[19]*a[47]; tot+=(udpint)a[20]*a[46]; tot+=(udpint)a[21]*a[45]; tot+=(udpint)a[22]*a[44]; tot+=(udpint)a[23]*a[43]; tot+=(udpint)a[24]*a[42]; tot+=(udpint)a[25]*a[41]; tot+=(udpint)a[26]*a[40]; tot+=(udpint)a[27]*a[39]; tot+=(udpint)a[28]*a[38]; tot+=(udpint)a[29]*a[37]; tot+=(udpint)a[30]*a[36]; tot+=(udpint)a[31]*a[35]; tot+=(udpint)a[32]*a[34]; tot*=2; tot+=(udpint)a[33]*a[33]; t+=tot;  spint v66=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[67]; tot+=(udpint)a[1]*a[66]; tot+=(udpint)a[2]*a[65]; tot+=(udpint)a[3]*a[64]; tot+=(udpint)a[4]*a[63]; tot+=(udpint)a[5]*a[62]; tot+=(udpint)a[6]*a[61]; tot+=(udpint)a[7]*a[60]; tot+=(udpint)a[8]*a[59]; tot+=(udpint)a[9]*a[58]; tot+=(udpint)a[10]*a[57]; tot+=(udpint)a[11]*a[56]; tot+=(udpint)a[12]*a[55]; tot+=(udpint)a[13]*a[54]; tot+=(udpint)a[14]*a[53]; tot+=(udpint)a[15]*a[52]; tot+=(udpint)a[16]*a[51]; tot+=(udpint)a[17]*a[50]; tot+=(udpint)a[18]*a[49]; tot+=(udpint)a[19]*a[48]; tot+=(udpint)a[20]*a[47]; tot+=(udpint)a[21]*a[46]; tot+=(udpint)a[22]*a[45]; tot+=(udpint)a[23]*a[44]; tot+=(udpint)a[24]*a[43]; tot+=(udpint)a[25]*a[42]; tot+=(udpint)a[26]*a[41]; tot+=(udpint)a[27]*a[40]; tot+=(udpint)a[28]*a[39]; tot+=(udpint)a[29]*a[38]; tot+=(udpint)a[30]*a[37]; tot+=(udpint)a[31]*a[36]; tot+=(udpint)a[32]*a[35]; tot+=(udpint)a[33]*a[34]; tot*=2; t+=tot;  spint v67=((spint)t & mask); t>>=60;
	tot=(udpint)a[0]*a[68]; tot+=(udpint)a[1]*a[67]; tot+=(udpint)a[2]*a[66]; tot+=(udpint)a[3]*a[65]; tot+=(udpint)a[4]*a[64]; tot+=(udpint)a[5]*a[63]; tot+=(udpint)a[6]*a[62]; tot+=(udpint)a[7]*a[61]; tot+=(udpint)a[8]*a[60]; tot+=(udpint)a[9]*a[59]; tot+=(udpint)a[10]*a[58]; tot+=(udpint)a[11]*a[57]; tot+=(udpint)a[12]*a[56]; tot+=(udpint)a[13]*a[55]; tot+=(udpint)a[14]*a[54]; tot+=(udpint)a[15]*a[53]; tot+=(udpint)a[16]*a[52]; tot+=(udpint)a[17]*a[51]; tot+=(udpint)a[18]*a[50]; tot+=(udpint)a[19]*a[49]; tot+=(udpint)a[20]*a[48]; tot+=(udpint)a[21]*a[47]; tot+=(udpint)a[22]*a[46]; tot+=(udpint)a[23]*a[45]; tot+=(udpint)a[24]*a[44]; tot+=(udpint)a[25]*a[43]; tot+=(udpint)a[26]*a[42]; tot+=(udpint)a[27]*a[41]; tot+=(udpint)a[28]*a[40]; tot+=(udpint)a[29]*a[39]; tot+=(udpint)a[30]*a[38]; tot+=(udpint)a[31]*a[37]; tot+=(udpint)a[32]*a[36]; tot+=(udpint)a[33]*a[35]; tot*=2; tot+=(udpint)a[34]*a[34]; t+=tot;  t+=(udpint)v0*p68;  spint v68=((spint)t & mask); t>>=60;
	tot=(udpint)a[1]*a[68]; tot+=(udpint)a[2]*a[67]; tot+=(udpint)a[3]*a[66]; tot+=(udpint)a[4]*a[65]; tot+=(udpint)a[5]*a[64]; tot+=(udpint)a[6]*a[63]; tot+=(udpint)a[7]*a[62]; tot+=(udpint)a[8]*a[61]; tot+=(udpint)a[9]*a[60]; tot+=(udpint)a[10]*a[59]; tot+=(udpint)a[11]*a[58]; tot+=(udpint)a[12]*a[57]; tot+=(udpint)a[13]*a[56]; tot+=(udpint)a[14]*a[55]; tot+=(udpint)a[15]*a[54]; tot+=(udpint)a[16]*a[53]; tot+=(udpint)a[17]*a[52]; tot+=(udpint)a[18]*a[51]; tot+=(udpint)a[19]*a[50]; tot+=(udpint)a[20]*a[49]; tot+=(udpint)a[21]*a[48]; tot+=(udpint)a[22]*a[47]; tot+=(udpint)a[23]*a[46]; tot+=(udpint)a[24]*a[45]; tot+=(udpint)a[25]*a[44]; tot+=(udpint)a[26]*a[43]; tot+=(udpint)a[27]*a[42]; tot+=(udpint)a[28]*a[41]; tot+=(udpint)a[29]*a[40]; tot+=(udpint)a[30]*a[39]; tot+=(udpint)a[31]*a[38]; tot+=(udpint)a[32]*a[37]; tot+=(udpint)a[33]*a[36]; tot+=(udpint)a[34]*a[35]; tot*=2; t+=tot;  t+=(udpint)v1*p68;  c[0]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[2]*a[68]; tot+=(udpint)a[3]*a[67]; tot+=(udpint)a[4]*a[66]; tot+=(udpint)a[5]*a[65]; tot+=(udpint)a[6]*a[64]; tot+=(udpint)a[7]*a[63]; tot+=(udpint)a[8]*a[62]; tot+=(udpint)a[9]*a[61]; tot+=(udpint)a[10]*a[60]; tot+=(udpint)a[11]*a[59]; tot+=(udpint)a[12]*a[58]; tot+=(udpint)a[13]*a[57]; tot+=(udpint)a[14]*a[56]; tot+=(udpint)a[15]*a[55]; tot+=(udpint)a[16]*a[54]; tot+=(udpint)a[17]*a[53]; tot+=(udpint)a[18]*a[52]; tot+=(udpint)a[19]*a[51]; tot+=(udpint)a[20]*a[50]; tot+=(udpint)a[21]*a[49]; tot+=(udpint)a[22]*a[48]; tot+=(udpint)a[23]*a[47]; tot+=(udpint)a[24]*a[46]; tot+=(udpint)a[25]*a[45]; tot+=(udpint)a[26]*a[44]; tot+=(udpint)a[27]*a[43]; tot+=(udpint)a[28]*a[42]; tot+=(udpint)a[29]*a[41]; tot+=(udpint)a[30]*a[40]; tot+=(udpint)a[31]*a[39]; tot+=(udpint)a[32]*a[38]; tot+=(udpint)a[33]*a[37]; tot+=(udpint)a[34]*a[36]; tot*=2; tot+=(udpint)a[35]*a[35]; t+=tot;  t+=(udpint)v2*p68;  c[1]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[3]*a[68]; tot+=(udpint)a[4]*a[67]; tot+=(udpint)a[5]*a[66]; tot+=(udpint)a[6]*a[65]; tot+=(udpint)a[7]*a[64]; tot+=(udpint)a[8]*a[63]; tot+=(udpint)a[9]*a[62]; tot+=(udpint)a[10]*a[61]; tot+=(udpint)a[11]*a[60]; tot+=(udpint)a[12]*a[59]; tot+=(udpint)a[13]*a[58]; tot+=(udpint)a[14]*a[57]; tot+=(udpint)a[15]*a[56]; tot+=(udpint)a[16]*a[55]; tot+=(udpint)a[17]*a[54]; tot+=(udpint)a[18]*a[53]; tot+=(udpint)a[19]*a[52]; tot+=(udpint)a[20]*a[51]; tot+=(udpint)a[21]*a[50]; tot+=(udpint)a[22]*a[49]; tot+=(udpint)a[23]*a[48]; tot+=(udpint)a[24]*a[47]; tot+=(udpint)a[25]*a[46]; tot+=(udpint)a[26]*a[45]; tot+=(udpint)a[27]*a[44]; tot+=(udpint)a[28]*a[43]; tot+=(udpint)a[29]*a[42]; tot+=(udpint)a[30]*a[41]; tot+=(udpint)a[31]*a[40]; tot+=(udpint)a[32]*a[39]; tot+=(udpint)a[33]*a[38]; tot+=(udpint)a[34]*a[37]; tot+=(udpint)a[35]*a[36]; tot*=2; t+=tot;  t+=(udpint)v3*p68;  c[2]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[4]*a[68]; tot+=(udpint)a[5]*a[67]; tot+=(udpint)a[6]*a[66]; tot+=(udpint)a[7]*a[65]; tot+=(udpint)a[8]*a[64]; tot+=(udpint)a[9]*a[63]; tot+=(udpint)a[10]*a[62]; tot+=(udpint)a[11]*a[61]; tot+=(udpint)a[12]*a[60]; tot+=(udpint)a[13]*a[59]; tot+=(udpint)a[14]*a[58]; tot+=(udpint)a[15]*a[57]; tot+=(udpint)a[16]*a[56]; tot+=(udpint)a[17]*a[55]; tot+=(udpint)a[18]*a[54]; tot+=(udpint)a[19]*a[53]; tot+=(udpint)a[20]*a[52]; tot+=(udpint)a[21]*a[51]; tot+=(udpint)a[22]*a[50]; tot+=(udpint)a[23]*a[49]; tot+=(udpint)a[24]*a[48]; tot+=(udpint)a[25]*a[47]; tot+=(udpint)a[26]*a[46]; tot+=(udpint)a[27]*a[45]; tot+=(udpint)a[28]*a[44]; tot+=(udpint)a[29]*a[43]; tot+=(udpint)a[30]*a[42]; tot+=(udpint)a[31]*a[41]; tot+=(udpint)a[32]*a[40]; tot+=(udpint)a[33]*a[39]; tot+=(udpint)a[34]*a[38]; tot+=(udpint)a[35]*a[37]; tot*=2; tot+=(udpint)a[36]*a[36]; t+=tot;  t+=(udpint)v4*p68;  c[3]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[5]*a[68]; tot+=(udpint)a[6]*a[67]; tot+=(udpint)a[7]*a[66]; tot+=(udpint)a[8]*a[65]; tot+=(udpint)a[9]*a[64]; tot+=(udpint)a[10]*a[63]; tot+=(udpint)a[11]*a[62]; tot+=(udpint)a[12]*a[61]; tot+=(udpint)a[13]*a[60]; tot+=(udpint)a[14]*a[59]; tot+=(udpint)a[15]*a[58]; tot+=(udpint)a[16]*a[57]; tot+=(udpint)a[17]*a[56]; tot+=(udpint)a[18]*a[55]; tot+=(udpint)a[19]*a[54]; tot+=(udpint)a[20]*a[53]; tot+=(udpint)a[21]*a[52]; tot+=(udpint)a[22]*a[51]; tot+=(udpint)a[23]*a[50]; tot+=(udpint)a[24]*a[49]; tot+=(udpint)a[25]*a[48]; tot+=(udpint)a[26]*a[47]; tot+=(udpint)a[27]*a[46]; tot+=(udpint)a[28]*a[45]; tot+=(udpint)a[29]*a[44]; tot+=(udpint)a[30]*a[43]; tot+=(udpint)a[31]*a[42]; tot+=(udpint)a[32]*a[41]; tot+=(udpint)a[33]*a[40]; tot+=(udpint)a[34]*a[39]; tot+=(udpint)a[35]*a[38]; tot+=(udpint)a[36]*a[37]; tot*=2; t+=tot;  t+=(udpint)v5*p68;  c[4]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[6]*a[68]; tot+=(udpint)a[7]*a[67]; tot+=(udpint)a[8]*a[66]; tot+=(udpint)a[9]*a[65]; tot+=(udpint)a[10]*a[64]; tot+=(udpint)a[11]*a[63]; tot+=(udpint)a[12]*a[62]; tot+=(udpint)a[13]*a[61]; tot+=(udpint)a[14]*a[60]; tot+=(udpint)a[15]*a[59]; tot+=(udpint)a[16]*a[58]; tot+=(udpint)a[17]*a[57]; tot+=(udpint)a[18]*a[56]; tot+=(udpint)a[19]*a[55]; tot+=(udpint)a[20]*a[54]; tot+=(udpint)a[21]*a[53]; tot+=(udpint)a[22]*a[52]; tot+=(udpint)a[23]*a[51]; tot+=(udpint)a[24]*a[50]; tot+=(udpint)a[25]*a[49]; tot+=(udpint)a[26]*a[48]; tot+=(udpint)a[27]*a[47]; tot+=(udpint)a[28]*a[46]; tot+=(udpint)a[29]*a[45]; tot+=(udpint)a[30]*a[44]; tot+=(udpint)a[31]*a[43]; tot+=(udpint)a[32]*a[42]; tot+=(udpint)a[33]*a[41]; tot+=(udpint)a[34]*a[40]; tot+=(udpint)a[35]*a[39]; tot+=(udpint)a[36]*a[38]; tot*=2; tot+=(udpint)a[37]*a[37]; t+=tot;  t+=(udpint)v6*p68;  c[5]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[7]*a[68]; tot+=(udpint)a[8]*a[67]; tot+=(udpint)a[9]*a[66]; tot+=(udpint)a[10]*a[65]; tot+=(udpint)a[11]*a[64]; tot+=(udpint)a[12]*a[63]; tot+=(udpint)a[13]*a[62]; tot+=(udpint)a[14]*a[61]; tot+=(udpint)a[15]*a[60]; tot+=(udpint)a[16]*a[59]; tot+=(udpint)a[17]*a[58]; tot+=(udpint)a[18]*a[57]; tot+=(udpint)a[19]*a[56]; tot+=(udpint)a[20]*a[55]; tot+=(udpint)a[21]*a[54]; tot+=(udpint)a[22]*a[53]; tot+=(udpint)a[23]*a[52]; tot+=(udpint)a[24]*a[51]; tot+=(udpint)a[25]*a[50]; tot+=(udpint)a[26]*a[49]; tot+=(udpint)a[27]*a[48]; tot+=(udpint)a[28]*a[47]; tot+=(udpint)a[29]*a[46]; tot+=(udpint)a[30]*a[45]; tot+=(udpint)a[31]*a[44]; tot+=(udpint)a[32]*a[43]; tot+=(udpint)a[33]*a[42]; tot+=(udpint)a[34]*a[41]; tot+=(udpint)a[35]*a[40]; tot+=(udpint)a[36]*a[39]; tot+=(udpint)a[37]*a[38]; tot*=2; t+=tot;  t+=(udpint)v7*p68;  c[6]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[8]*a[68]; tot+=(udpint)a[9]*a[67]; tot+=(udpint)a[10]*a[66]; tot+=(udpint)a[11]*a[65]; tot+=(udpint)a[12]*a[64]; tot+=(udpint)a[13]*a[63]; tot+=(udpint)a[14]*a[62]; tot+=(udpint)a[15]*a[61]; tot+=(udpint)a[16]*a[60]; tot+=(udpint)a[17]*a[59]; tot+=(udpint)a[18]*a[58]; tot+=(udpint)a[19]*a[57]; tot+=(udpint)a[20]*a[56]; tot+=(udpint)a[21]*a[55]; tot+=(udpint)a[22]*a[54]; tot+=(udpint)a[23]*a[53]; tot+=(udpint)a[24]*a[52]; tot+=(udpint)a[25]*a[51]; tot+=(udpint)a[26]*a[50]; tot+=(udpint)a[27]*a[49]; tot+=(udpint)a[28]*a[48]; tot+=(udpint)a[29]*a[47]; tot+=(udpint)a[30]*a[46]; tot+=(udpint)a[31]*a[45]; tot+=(udpint)a[32]*a[44]; tot+=(udpint)a[33]*a[43]; tot+=(udpint)a[34]*a[42]; tot+=(udpint)a[35]*a[41]; tot+=(udpint)a[36]*a[40]; tot+=(udpint)a[37]*a[39]; tot*=2; tot+=(udpint)a[38]*a[38]; t+=tot;  t+=(udpint)v8*p68;  c[7]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[9]*a[68]; tot+=(udpint)a[10]*a[67]; tot+=(udpint)a[11]*a[66]; tot+=(udpint)a[12]*a[65]; tot+=(udpint)a[13]*a[64]; tot+=(udpint)a[14]*a[63]; tot+=(udpint)a[15]*a[62]; tot+=(udpint)a[16]*a[61]; tot+=(udpint)a[17]*a[60]; tot+=(udpint)a[18]*a[59]; tot+=(udpint)a[19]*a[58]; tot+=(udpint)a[20]*a[57]; tot+=(udpint)a[21]*a[56]; tot+=(udpint)a[22]*a[55]; tot+=(udpint)a[23]*a[54]; tot+=(udpint)a[24]*a[53]; tot+=(udpint)a[25]*a[52]; tot+=(udpint)a[26]*a[51]; tot+=(udpint)a[27]*a[50]; tot+=(udpint)a[28]*a[49]; tot+=(udpint)a[29]*a[48]; tot+=(udpint)a[30]*a[47]; tot+=(udpint)a[31]*a[46]; tot+=(udpint)a[32]*a[45]; tot+=(udpint)a[33]*a[44]; tot+=(udpint)a[34]*a[43]; tot+=(udpint)a[35]*a[42]; tot+=(udpint)a[36]*a[41]; tot+=(udpint)a[37]*a[40]; tot+=(udpint)a[38]*a[39]; tot*=2; t+=tot;  t+=(udpint)v9*p68;  c[8]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[10]*a[68]; tot+=(udpint)a[11]*a[67]; tot+=(udpint)a[12]*a[66]; tot+=(udpint)a[13]*a[65]; tot+=(udpint)a[14]*a[64]; tot+=(udpint)a[15]*a[63]; tot+=(udpint)a[16]*a[62]; tot+=(udpint)a[17]*a[61]; tot+=(udpint)a[18]*a[60]; tot+=(udpint)a[19]*a[59]; tot+=(udpint)a[20]*a[58]; tot+=(udpint)a[21]*a[57]; tot+=(udpint)a[22]*a[56]; tot+=(udpint)a[23]*a[55]; tot+=(udpint)a[24]*a[54]; tot+=(udpint)a[25]*a[53]; tot+=(udpint)a[26]*a[52]; tot+=(udpint)a[27]*a[51]; tot+=(udpint)a[28]*a[50]; tot+=(udpint)a[29]*a[49]; tot+=(udpint)a[30]*a[48]; tot+=(udpint)a[31]*a[47]; tot+=(udpint)a[32]*a[46]; tot+=(udpint)a[33]*a[45]; tot+=(udpint)a[34]*a[44]; tot+=(udpint)a[35]*a[43]; tot+=(udpint)a[36]*a[42]; tot+=(udpint)a[37]*a[41]; tot+=(udpint)a[38]*a[40]; tot*=2; tot+=(udpint)a[39]*a[39]; t+=tot;  t+=(udpint)v10*p68;  c[9]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[11]*a[68]; tot+=(udpint)a[12]*a[67]; tot+=(udpint)a[13]*a[66]; tot+=(udpint)a[14]*a[65]; tot+=(udpint)a[15]*a[64]; tot+=(udpint)a[16]*a[63]; tot+=(udpint)a[17]*a[62]; tot+=(udpint)a[18]*a[61]; tot+=(udpint)a[19]*a[60]; tot+=(udpint)a[20]*a[59]; tot+=(udpint)a[21]*a[58]; tot+=(udpint)a[22]*a[57]; tot+=(udpint)a[23]*a[56]; tot+=(udpint)a[24]*a[55]; tot+=(udpint)a[25]*a[54]; tot+=(udpint)a[26]*a[53]; tot+=(udpint)a[27]*a[52]; tot+=(udpint)a[28]*a[51]; tot+=(udpint)a[29]*a[50]; tot+=(udpint)a[30]*a[49]; tot+=(udpint)a[31]*a[48]; tot+=(udpint)a[32]*a[47]; tot+=(udpint)a[33]*a[46]; tot+=(udpint)a[34]*a[45]; tot+=(udpint)a[35]*a[44]; tot+=(udpint)a[36]*a[43]; tot+=(udpint)a[37]*a[42]; tot+=(udpint)a[38]*a[41]; tot+=(udpint)a[39]*a[40]; tot*=2; t+=tot;  t+=(udpint)v11*p68;  c[10]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[12]*a[68]; tot+=(udpint)a[13]*a[67]; tot+=(udpint)a[14]*a[66]; tot+=(udpint)a[15]*a[65]; tot+=(udpint)a[16]*a[64]; tot+=(udpint)a[17]*a[63]; tot+=(udpint)a[18]*a[62]; tot+=(udpint)a[19]*a[61]; tot+=(udpint)a[20]*a[60]; tot+=(udpint)a[21]*a[59]; tot+=(udpint)a[22]*a[58]; tot+=(udpint)a[23]*a[57]; tot+=(udpint)a[24]*a[56]; tot+=(udpint)a[25]*a[55]; tot+=(udpint)a[26]*a[54]; tot+=(udpint)a[27]*a[53]; tot+=(udpint)a[28]*a[52]; tot+=(udpint)a[29]*a[51]; tot+=(udpint)a[30]*a[50]; tot+=(udpint)a[31]*a[49]; tot+=(udpint)a[32]*a[48]; tot+=(udpint)a[33]*a[47]; tot+=(udpint)a[34]*a[46]; tot+=(udpint)a[35]*a[45]; tot+=(udpint)a[36]*a[44]; tot+=(udpint)a[37]*a[43]; tot+=(udpint)a[38]*a[42]; tot+=(udpint)a[39]*a[41]; tot*=2; tot+=(udpint)a[40]*a[40]; t+=tot;  t+=(udpint)v12*p68;  c[11]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[13]*a[68]; tot+=(udpint)a[14]*a[67]; tot+=(udpint)a[15]*a[66]; tot+=(udpint)a[16]*a[65]; tot+=(udpint)a[17]*a[64]; tot+=(udpint)a[18]*a[63]; tot+=(udpint)a[19]*a[62]; tot+=(udpint)a[20]*a[61]; tot+=(udpint)a[21]*a[60]; tot+=(udpint)a[22]*a[59]; tot+=(udpint)a[23]*a[58]; tot+=(udpint)a[24]*a[57]; tot+=(udpint)a[25]*a[56]; tot+=(udpint)a[26]*a[55]; tot+=(udpint)a[27]*a[54]; tot+=(udpint)a[28]*a[53]; tot+=(udpint)a[29]*a[52]; tot+=(udpint)a[30]*a[51]; tot+=(udpint)a[31]*a[50]; tot+=(udpint)a[32]*a[49]; tot+=(udpint)a[33]*a[48]; tot+=(udpint)a[34]*a[47]; tot+=(udpint)a[35]*a[46]; tot+=(udpint)a[36]*a[45]; tot+=(udpint)a[37]*a[44]; tot+=(udpint)a[38]*a[43]; tot+=(udpint)a[39]*a[42]; tot+=(udpint)a[40]*a[41]; tot*=2; t+=tot;  t+=(udpint)v13*p68;  c[12]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[14]*a[68]; tot+=(udpint)a[15]*a[67]; tot+=(udpint)a[16]*a[66]; tot+=(udpint)a[17]*a[65]; tot+=(udpint)a[18]*a[64]; tot+=(udpint)a[19]*a[63]; tot+=(udpint)a[20]*a[62]; tot+=(udpint)a[21]*a[61]; tot+=(udpint)a[22]*a[60]; tot+=(udpint)a[23]*a[59]; tot+=(udpint)a[24]*a[58]; tot+=(udpint)a[25]*a[57]; tot+=(udpint)a[26]*a[56]; tot+=(udpint)a[27]*a[55]; tot+=(udpint)a[28]*a[54]; tot+=(udpint)a[29]*a[53]; tot+=(udpint)a[30]*a[52]; tot+=(udpint)a[31]*a[51]; tot+=(udpint)a[32]*a[50]; tot+=(udpint)a[33]*a[49]; tot+=(udpint)a[34]*a[48]; tot+=(udpint)a[35]*a[47]; tot+=(udpint)a[36]*a[46]; tot+=(udpint)a[37]*a[45]; tot+=(udpint)a[38]*a[44]; tot+=(udpint)a[39]*a[43]; tot+=(udpint)a[40]*a[42]; tot*=2; tot+=(udpint)a[41]*a[41]; t+=tot;  t+=(udpint)v14*p68;  c[13]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[15]*a[68]; tot+=(udpint)a[16]*a[67]; tot+=(udpint)a[17]*a[66]; tot+=(udpint)a[18]*a[65]; tot+=(udpint)a[19]*a[64]; tot+=(udpint)a[20]*a[63]; tot+=(udpint)a[21]*a[62]; tot+=(udpint)a[22]*a[61]; tot+=(udpint)a[23]*a[60]; tot+=(udpint)a[24]*a[59]; tot+=(udpint)a[25]*a[58]; tot+=(udpint)a[26]*a[57]; tot+=(udpint)a[27]*a[56]; tot+=(udpint)a[28]*a[55]; tot+=(udpint)a[29]*a[54]; tot+=(udpint)a[30]*a[53]; tot+=(udpint)a[31]*a[52]; tot+=(udpint)a[32]*a[51]; tot+=(udpint)a[33]*a[50]; tot+=(udpint)a[34]*a[49]; tot+=(udpint)a[35]*a[48]; tot+=(udpint)a[36]*a[47]; tot+=(udpint)a[37]*a[46]; tot+=(udpint)a[38]*a[45]; tot+=(udpint)a[39]*a[44]; tot+=(udpint)a[40]*a[43]; tot+=(udpint)a[41]*a[42]; tot*=2; t+=tot;  t+=(udpint)v15*p68;  c[14]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[16]*a[68]; tot+=(udpint)a[17]*a[67]; tot+=(udpint)a[18]*a[66]; tot+=(udpint)a[19]*a[65]; tot+=(udpint)a[20]*a[64]; tot+=(udpint)a[21]*a[63]; tot+=(udpint)a[22]*a[62]; tot+=(udpint)a[23]*a[61]; tot+=(udpint)a[24]*a[60]; tot+=(udpint)a[25]*a[59]; tot+=(udpint)a[26]*a[58]; tot+=(udpint)a[27]*a[57]; tot+=(udpint)a[28]*a[56]; tot+=(udpint)a[29]*a[55]; tot+=(udpint)a[30]*a[54]; tot+=(udpint)a[31]*a[53]; tot+=(udpint)a[32]*a[52]; tot+=(udpint)a[33]*a[51]; tot+=(udpint)a[34]*a[50]; tot+=(udpint)a[35]*a[49]; tot+=(udpint)a[36]*a[48]; tot+=(udpint)a[37]*a[47]; tot+=(udpint)a[38]*a[46]; tot+=(udpint)a[39]*a[45]; tot+=(udpint)a[40]*a[44]; tot+=(udpint)a[41]*a[43]; tot*=2; tot+=(udpint)a[42]*a[42]; t+=tot;  t+=(udpint)v16*p68;  c[15]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[17]*a[68]; tot+=(udpint)a[18]*a[67]; tot+=(udpint)a[19]*a[66]; tot+=(udpint)a[20]*a[65]; tot+=(udpint)a[21]*a[64]; tot+=(udpint)a[22]*a[63]; tot+=(udpint)a[23]*a[62]; tot+=(udpint)a[24]*a[61]; tot+=(udpint)a[25]*a[60]; tot+=(udpint)a[26]*a[59]; tot+=(udpint)a[27]*a[58]; tot+=(udpint)a[28]*a[57]; tot+=(udpint)a[29]*a[56]; tot+=(udpint)a[30]*a[55]; tot+=(udpint)a[31]*a[54]; tot+=(udpint)a[32]*a[53]; tot+=(udpint)a[33]*a[52]; tot+=(udpint)a[34]*a[51]; tot+=(udpint)a[35]*a[50]; tot+=(udpint)a[36]*a[49]; tot+=(udpint)a[37]*a[48]; tot+=(udpint)a[38]*a[47]; tot+=(udpint)a[39]*a[46]; tot+=(udpint)a[40]*a[45]; tot+=(udpint)a[41]*a[44]; tot+=(udpint)a[42]*a[43]; tot*=2; t+=tot;  t+=(udpint)v17*p68;  c[16]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[18]*a[68]; tot+=(udpint)a[19]*a[67]; tot+=(udpint)a[20]*a[66]; tot+=(udpint)a[21]*a[65]; tot+=(udpint)a[22]*a[64]; tot+=(udpint)a[23]*a[63]; tot+=(udpint)a[24]*a[62]; tot+=(udpint)a[25]*a[61]; tot+=(udpint)a[26]*a[60]; tot+=(udpint)a[27]*a[59]; tot+=(udpint)a[28]*a[58]; tot+=(udpint)a[29]*a[57]; tot+=(udpint)a[30]*a[56]; tot+=(udpint)a[31]*a[55]; tot+=(udpint)a[32]*a[54]; tot+=(udpint)a[33]*a[53]; tot+=(udpint)a[34]*a[52]; tot+=(udpint)a[35]*a[51]; tot+=(udpint)a[36]*a[50]; tot+=(udpint)a[37]*a[49]; tot+=(udpint)a[38]*a[48]; tot+=(udpint)a[39]*a[47]; tot+=(udpint)a[40]*a[46]; tot+=(udpint)a[41]*a[45]; tot+=(udpint)a[42]*a[44]; tot*=2; tot+=(udpint)a[43]*a[43]; t+=tot;  t+=(udpint)v18*p68;  c[17]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[19]*a[68]; tot+=(udpint)a[20]*a[67]; tot+=(udpint)a[21]*a[66]; tot+=(udpint)a[22]*a[65]; tot+=(udpint)a[23]*a[64]; tot+=(udpint)a[24]*a[63]; tot+=(udpint)a[25]*a[62]; tot+=(udpint)a[26]*a[61]; tot+=(udpint)a[27]*a[60]; tot+=(udpint)a[28]*a[59]; tot+=(udpint)a[29]*a[58]; tot+=(udpint)a[30]*a[57]; tot+=(udpint)a[31]*a[56]; tot+=(udpint)a[32]*a[55]; tot+=(udpint)a[33]*a[54]; tot+=(udpint)a[34]*a[53]; tot+=(udpint)a[35]*a[52]; tot+=(udpint)a[36]*a[51]; tot+=(udpint)a[37]*a[50]; tot+=(udpint)a[38]*a[49]; tot+=(udpint)a[39]*a[48]; tot+=(udpint)a[40]*a[47]; tot+=(udpint)a[41]*a[46]; tot+=(udpint)a[42]*a[45]; tot+=(udpint)a[43]*a[44]; tot*=2; t+=tot;  t+=(udpint)v19*p68;  c[18]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[20]*a[68]; tot+=(udpint)a[21]*a[67]; tot+=(udpint)a[22]*a[66]; tot+=(udpint)a[23]*a[65]; tot+=(udpint)a[24]*a[64]; tot+=(udpint)a[25]*a[63]; tot+=(udpint)a[26]*a[62]; tot+=(udpint)a[27]*a[61]; tot+=(udpint)a[28]*a[60]; tot+=(udpint)a[29]*a[59]; tot+=(udpint)a[30]*a[58]; tot+=(udpint)a[31]*a[57]; tot+=(udpint)a[32]*a[56]; tot+=(udpint)a[33]*a[55]; tot+=(udpint)a[34]*a[54]; tot+=(udpint)a[35]*a[53]; tot+=(udpint)a[36]*a[52]; tot+=(udpint)a[37]*a[51]; tot+=(udpint)a[38]*a[50]; tot+=(udpint)a[39]*a[49]; tot+=(udpint)a[40]*a[48]; tot+=(udpint)a[41]*a[47]; tot+=(udpint)a[42]*a[46]; tot+=(udpint)a[43]*a[45]; tot*=2; tot+=(udpint)a[44]*a[44]; t+=tot;  t+=(udpint)v20*p68;  c[19]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[21]*a[68]; tot+=(udpint)a[22]*a[67]; tot+=(udpint)a[23]*a[66]; tot+=(udpint)a[24]*a[65]; tot+=(udpint)a[25]*a[64]; tot+=(udpint)a[26]*a[63]; tot+=(udpint)a[27]*a[62]; tot+=(udpint)a[28]*a[61]; tot+=(udpint)a[29]*a[60]; tot+=(udpint)a[30]*a[59]; tot+=(udpint)a[31]*a[58]; tot+=(udpint)a[32]*a[57]; tot+=(udpint)a[33]*a[56]; tot+=(udpint)a[34]*a[55]; tot+=(udpint)a[35]*a[54]; tot+=(udpint)a[36]*a[53]; tot+=(udpint)a[37]*a[52]; tot+=(udpint)a[38]*a[51]; tot+=(udpint)a[39]*a[50]; tot+=(udpint)a[40]*a[49]; tot+=(udpint)a[41]*a[48]; tot+=(udpint)a[42]*a[47]; tot+=(udpint)a[43]*a[46]; tot+=(udpint)a[44]*a[45]; tot*=2; t+=tot;  t+=(udpint)v21*p68;  c[20]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[22]*a[68]; tot+=(udpint)a[23]*a[67]; tot+=(udpint)a[24]*a[66]; tot+=(udpint)a[25]*a[65]; tot+=(udpint)a[26]*a[64]; tot+=(udpint)a[27]*a[63]; tot+=(udpint)a[28]*a[62]; tot+=(udpint)a[29]*a[61]; tot+=(udpint)a[30]*a[60]; tot+=(udpint)a[31]*a[59]; tot+=(udpint)a[32]*a[58]; tot+=(udpint)a[33]*a[57]; tot+=(udpint)a[34]*a[56]; tot+=(udpint)a[35]*a[55]; tot+=(udpint)a[36]*a[54]; tot+=(udpint)a[37]*a[53]; tot+=(udpint)a[38]*a[52]; tot+=(udpint)a[39]*a[51]; tot+=(udpint)a[40]*a[50]; tot+=(udpint)a[41]*a[49]; tot+=(udpint)a[42]*a[48]; tot+=(udpint)a[43]*a[47]; tot+=(udpint)a[44]*a[46]; tot*=2; tot+=(udpint)a[45]*a[45]; t+=tot;  t+=(udpint)v22*p68;  c[21]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[23]*a[68]; tot+=(udpint)a[24]*a[67]; tot+=(udpint)a[25]*a[66]; tot+=(udpint)a[26]*a[65]; tot+=(udpint)a[27]*a[64]; tot+=(udpint)a[28]*a[63]; tot+=(udpint)a[29]*a[62]; tot+=(udpint)a[30]*a[61]; tot+=(udpint)a[31]*a[60]; tot+=(udpint)a[32]*a[59]; tot+=(udpint)a[33]*a[58]; tot+=(udpint)a[34]*a[57]; tot+=(udpint)a[35]*a[56]; tot+=(udpint)a[36]*a[55]; tot+=(udpint)a[37]*a[54]; tot+=(udpint)a[38]*a[53]; tot+=(udpint)a[39]*a[52]; tot+=(udpint)a[40]*a[51]; tot+=(udpint)a[41]*a[50]; tot+=(udpint)a[42]*a[49]; tot+=(udpint)a[43]*a[48]; tot+=(udpint)a[44]*a[47]; tot+=(udpint)a[45]*a[46]; tot*=2; t+=tot;  t+=(udpint)v23*p68;  c[22]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[24]*a[68]; tot+=(udpint)a[25]*a[67]; tot+=(udpint)a[26]*a[66]; tot+=(udpint)a[27]*a[65]; tot+=(udpint)a[28]*a[64]; tot+=(udpint)a[29]*a[63]; tot+=(udpint)a[30]*a[62]; tot+=(udpint)a[31]*a[61]; tot+=(udpint)a[32]*a[60]; tot+=(udpint)a[33]*a[59]; tot+=(udpint)a[34]*a[58]; tot+=(udpint)a[35]*a[57]; tot+=(udpint)a[36]*a[56]; tot+=(udpint)a[37]*a[55]; tot+=(udpint)a[38]*a[54]; tot+=(udpint)a[39]*a[53]; tot+=(udpint)a[40]*a[52]; tot+=(udpint)a[41]*a[51]; tot+=(udpint)a[42]*a[50]; tot+=(udpint)a[43]*a[49]; tot+=(udpint)a[44]*a[48]; tot+=(udpint)a[45]*a[47]; tot*=2; tot+=(udpint)a[46]*a[46]; t+=tot;  t+=(udpint)v24*p68;  c[23]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[25]*a[68]; tot+=(udpint)a[26]*a[67]; tot+=(udpint)a[27]*a[66]; tot+=(udpint)a[28]*a[65]; tot+=(udpint)a[29]*a[64]; tot+=(udpint)a[30]*a[63]; tot+=(udpint)a[31]*a[62]; tot+=(udpint)a[32]*a[61]; tot+=(udpint)a[33]*a[60]; tot+=(udpint)a[34]*a[59]; tot+=(udpint)a[35]*a[58]; tot+=(udpint)a[36]*a[57]; tot+=(udpint)a[37]*a[56]; tot+=(udpint)a[38]*a[55]; tot+=(udpint)a[39]*a[54]; tot+=(udpint)a[40]*a[53]; tot+=(udpint)a[41]*a[52]; tot+=(udpint)a[42]*a[51]; tot+=(udpint)a[43]*a[50]; tot+=(udpint)a[44]*a[49]; tot+=(udpint)a[45]*a[48]; tot+=(udpint)a[46]*a[47]; tot*=2; t+=tot;  t+=(udpint)v25*p68;  c[24]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[26]*a[68]; tot+=(udpint)a[27]*a[67]; tot+=(udpint)a[28]*a[66]; tot+=(udpint)a[29]*a[65]; tot+=(udpint)a[30]*a[64]; tot+=(udpint)a[31]*a[63]; tot+=(udpint)a[32]*a[62]; tot+=(udpint)a[33]*a[61]; tot+=(udpint)a[34]*a[60]; tot+=(udpint)a[35]*a[59]; tot+=(udpint)a[36]*a[58]; tot+=(udpint)a[37]*a[57]; tot+=(udpint)a[38]*a[56]; tot+=(udpint)a[39]*a[55]; tot+=(udpint)a[40]*a[54]; tot+=(udpint)a[41]*a[53]; tot+=(udpint)a[42]*a[52]; tot+=(udpint)a[43]*a[51]; tot+=(udpint)a[44]*a[50]; tot+=(udpint)a[45]*a[49]; tot+=(udpint)a[46]*a[48]; tot*=2; tot+=(udpint)a[47]*a[47]; t+=tot;  t+=(udpint)v26*p68;  c[25]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[27]*a[68]; tot+=(udpint)a[28]*a[67]; tot+=(udpint)a[29]*a[66]; tot+=(udpint)a[30]*a[65]; tot+=(udpint)a[31]*a[64]; tot+=(udpint)a[32]*a[63]; tot+=(udpint)a[33]*a[62]; tot+=(udpint)a[34]*a[61]; tot+=(udpint)a[35]*a[60]; tot+=(udpint)a[36]*a[59]; tot+=(udpint)a[37]*a[58]; tot+=(udpint)a[38]*a[57]; tot+=(udpint)a[39]*a[56]; tot+=(udpint)a[40]*a[55]; tot+=(udpint)a[41]*a[54]; tot+=(udpint)a[42]*a[53]; tot+=(udpint)a[43]*a[52]; tot+=(udpint)a[44]*a[51]; tot+=(udpint)a[45]*a[50]; tot+=(udpint)a[46]*a[49]; tot+=(udpint)a[47]*a[48]; tot*=2; t+=tot;  t+=(udpint)v27*p68;  c[26]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[28]*a[68]; tot+=(udpint)a[29]*a[67]; tot+=(udpint)a[30]*a[66]; tot+=(udpint)a[31]*a[65]; tot+=(udpint)a[32]*a[64]; tot+=(udpint)a[33]*a[63]; tot+=(udpint)a[34]*a[62]; tot+=(udpint)a[35]*a[61]; tot+=(udpint)a[36]*a[60]; tot+=(udpint)a[37]*a[59]; tot+=(udpint)a[38]*a[58]; tot+=(udpint)a[39]*a[57]; tot+=(udpint)a[40]*a[56]; tot+=(udpint)a[41]*a[55]; tot+=(udpint)a[42]*a[54]; tot+=(udpint)a[43]*a[53]; tot+=(udpint)a[44]*a[52]; tot+=(udpint)a[45]*a[51]; tot+=(udpint)a[46]*a[50]; tot+=(udpint)a[47]*a[49]; tot*=2; tot+=(udpint)a[48]*a[48]; t+=tot;  t+=(udpint)v28*p68;  c[27]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[29]*a[68]; tot+=(udpint)a[30]*a[67]; tot+=(udpint)a[31]*a[66]; tot+=(udpint)a[32]*a[65]; tot+=(udpint)a[33]*a[64]; tot+=(udpint)a[34]*a[63]; tot+=(udpint)a[35]*a[62]; tot+=(udpint)a[36]*a[61]; tot+=(udpint)a[37]*a[60]; tot+=(udpint)a[38]*a[59]; tot+=(udpint)a[39]*a[58]; tot+=(udpint)a[40]*a[57]; tot+=(udpint)a[41]*a[56]; tot+=(udpint)a[42]*a[55]; tot+=(udpint)a[43]*a[54]; tot+=(udpint)a[44]*a[53]; tot+=(udpint)a[45]*a[52]; tot+=(udpint)a[46]*a[51]; tot+=(udpint)a[47]*a[50]; tot+=(udpint)a[48]*a[49]; tot*=2; t+=tot;  t+=(udpint)v29*p68;  c[28]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[30]*a[68]; tot+=(udpint)a[31]*a[67]; tot+=(udpint)a[32]*a[66]; tot+=(udpint)a[33]*a[65]; tot+=(udpint)a[34]*a[64]; tot+=(udpint)a[35]*a[63]; tot+=(udpint)a[36]*a[62]; tot+=(udpint)a[37]*a[61]; tot+=(udpint)a[38]*a[60]; tot+=(udpint)a[39]*a[59]; tot+=(udpint)a[40]*a[58]; tot+=(udpint)a[41]*a[57]; tot+=(udpint)a[42]*a[56]; tot+=(udpint)a[43]*a[55]; tot+=(udpint)a[44]*a[54]; tot+=(udpint)a[45]*a[53]; tot+=(udpint)a[46]*a[52]; tot+=(udpint)a[47]*a[51]; tot+=(udpint)a[48]*a[50]; tot*=2; tot+=(udpint)a[49]*a[49]; t+=tot;  t+=(udpint)v30*p68;  c[29]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[31]*a[68]; tot+=(udpint)a[32]*a[67]; tot+=(udpint)a[33]*a[66]; tot+=(udpint)a[34]*a[65]; tot+=(udpint)a[35]*a[64]; tot+=(udpint)a[36]*a[63]; tot+=(udpint)a[37]*a[62]; tot+=(udpint)a[38]*a[61]; tot+=(udpint)a[39]*a[60]; tot+=(udpint)a[40]*a[59]; tot+=(udpint)a[41]*a[58]; tot+=(udpint)a[42]*a[57]; tot+=(udpint)a[43]*a[56]; tot+=(udpint)a[44]*a[55]; tot+=(udpint)a[45]*a[54]; tot+=(udpint)a[46]*a[53]; tot+=(udpint)a[47]*a[52]; tot+=(udpint)a[48]*a[51]; tot+=(udpint)a[49]*a[50]; tot*=2; t+=tot;  t+=(udpint)v31*p68;  c[30]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[32]*a[68]; tot+=(udpint)a[33]*a[67]; tot+=(udpint)a[34]*a[66]; tot+=(udpint)a[35]*a[65]; tot+=(udpint)a[36]*a[64]; tot+=(udpint)a[37]*a[63]; tot+=(udpint)a[38]*a[62]; tot+=(udpint)a[39]*a[61]; tot+=(udpint)a[40]*a[60]; tot+=(udpint)a[41]*a[59]; tot+=(udpint)a[42]*a[58]; tot+=(udpint)a[43]*a[57]; tot+=(udpint)a[44]*a[56]; tot+=(udpint)a[45]*a[55]; tot+=(udpint)a[46]*a[54]; tot+=(udpint)a[47]*a[53]; tot+=(udpint)a[48]*a[52]; tot+=(udpint)a[49]*a[51]; tot*=2; tot+=(udpint)a[50]*a[50]; t+=tot;  t+=(udpint)v32*p68;  c[31]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[33]*a[68]; tot+=(udpint)a[34]*a[67]; tot+=(udpint)a[35]*a[66]; tot+=(udpint)a[36]*a[65]; tot+=(udpint)a[37]*a[64]; tot+=(udpint)a[38]*a[63]; tot+=(udpint)a[39]*a[62]; tot+=(udpint)a[40]*a[61]; tot+=(udpint)a[41]*a[60]; tot+=(udpint)a[42]*a[59]; tot+=(udpint)a[43]*a[58]; tot+=(udpint)a[44]*a[57]; tot+=(udpint)a[45]*a[56]; tot+=(udpint)a[46]*a[55]; tot+=(udpint)a[47]*a[54]; tot+=(udpint)a[48]*a[53]; tot+=(udpint)a[49]*a[52]; tot+=(udpint)a[50]*a[51]; tot*=2; t+=tot;  t+=(udpint)v33*p68;  c[32]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[34]*a[68]; tot+=(udpint)a[35]*a[67]; tot+=(udpint)a[36]*a[66]; tot+=(udpint)a[37]*a[65]; tot+=(udpint)a[38]*a[64]; tot+=(udpint)a[39]*a[63]; tot+=(udpint)a[40]*a[62]; tot+=(udpint)a[41]*a[61]; tot+=(udpint)a[42]*a[60]; tot+=(udpint)a[43]*a[59]; tot+=(udpint)a[44]*a[58]; tot+=(udpint)a[45]*a[57]; tot+=(udpint)a[46]*a[56]; tot+=(udpint)a[47]*a[55]; tot+=(udpint)a[48]*a[54]; tot+=(udpint)a[49]*a[53]; tot+=(udpint)a[50]*a[52]; tot*=2; tot+=(udpint)a[51]*a[51]; t+=tot;  t+=(udpint)v34*p68;  c[33]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[35]*a[68]; tot+=(udpint)a[36]*a[67]; tot+=(udpint)a[37]*a[66]; tot+=(udpint)a[38]*a[65]; tot+=(udpint)a[39]*a[64]; tot+=(udpint)a[40]*a[63]; tot+=(udpint)a[41]*a[62]; tot+=(udpint)a[42]*a[61]; tot+=(udpint)a[43]*a[60]; tot+=(udpint)a[44]*a[59]; tot+=(udpint)a[45]*a[58]; tot+=(udpint)a[46]*a[57]; tot+=(udpint)a[47]*a[56]; tot+=(udpint)a[48]*a[55]; tot+=(udpint)a[49]*a[54]; tot+=(udpint)a[50]*a[53]; tot+=(udpint)a[51]*a[52]; tot*=2; t+=tot;  t+=(udpint)v35*p68;  c[34]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[36]*a[68]; tot+=(udpint)a[37]*a[67]; tot+=(udpint)a[38]*a[66]; tot+=(udpint)a[39]*a[65]; tot+=(udpint)a[40]*a[64]; tot+=(udpint)a[41]*a[63]; tot+=(udpint)a[42]*a[62]; tot+=(udpint)a[43]*a[61]; tot+=(udpint)a[44]*a[60]; tot+=(udpint)a[45]*a[59]; tot+=(udpint)a[46]*a[58]; tot+=(udpint)a[47]*a[57]; tot+=(udpint)a[48]*a[56]; tot+=(udpint)a[49]*a[55]; tot+=(udpint)a[50]*a[54]; tot+=(udpint)a[51]*a[53]; tot*=2; tot+=(udpint)a[52]*a[52]; t+=tot;  t+=(udpint)v36*p68;  c[35]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[37]*a[68]; tot+=(udpint)a[38]*a[67]; tot+=(udpint)a[39]*a[66]; tot+=(udpint)a[40]*a[65]; tot+=(udpint)a[41]*a[64]; tot+=(udpint)a[42]*a[63]; tot+=(udpint)a[43]*a[62]; tot+=(udpint)a[44]*a[61]; tot+=(udpint)a[45]*a[60]; tot+=(udpint)a[46]*a[59]; tot+=(udpint)a[47]*a[58]; tot+=(udpint)a[48]*a[57]; tot+=(udpint)a[49]*a[56]; tot+=(udpint)a[50]*a[55]; tot+=(udpint)a[51]*a[54]; tot+=(udpint)a[52]*a[53]; tot*=2; t+=tot;  t+=(udpint)v37*p68;  c[36]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[38]*a[68]; tot+=(udpint)a[39]*a[67]; tot+=(udpint)a[40]*a[66]; tot+=(udpint)a[41]*a[65]; tot+=(udpint)a[42]*a[64]; tot+=(udpint)a[43]*a[63]; tot+=(udpint)a[44]*a[62]; tot+=(udpint)a[45]*a[61]; tot+=(udpint)a[46]*a[60]; tot+=(udpint)a[47]*a[59]; tot+=(udpint)a[48]*a[58]; tot+=(udpint)a[49]*a[57]; tot+=(udpint)a[50]*a[56]; tot+=(udpint)a[51]*a[55]; tot+=(udpint)a[52]*a[54]; tot*=2; tot+=(udpint)a[53]*a[53]; t+=tot;  t+=(udpint)v38*p68;  c[37]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[39]*a[68]; tot+=(udpint)a[40]*a[67]; tot+=(udpint)a[41]*a[66]; tot+=(udpint)a[42]*a[65]; tot+=(udpint)a[43]*a[64]; tot+=(udpint)a[44]*a[63]; tot+=(udpint)a[45]*a[62]; tot+=(udpint)a[46]*a[61]; tot+=(udpint)a[47]*a[60]; tot+=(udpint)a[48]*a[59]; tot+=(udpint)a[49]*a[58]; tot+=(udpint)a[50]*a[57]; tot+=(udpint)a[51]*a[56]; tot+=(udpint)a[52]*a[55]; tot+=(udpint)a[53]*a[54]; tot*=2; t+=tot;  t+=(udpint)v39*p68;  c[38]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[40]*a[68]; tot+=(udpint)a[41]*a[67]; tot+=(udpint)a[42]*a[66]; tot+=(udpint)a[43]*a[65]; tot+=(udpint)a[44]*a[64]; tot+=(udpint)a[45]*a[63]; tot+=(udpint)a[46]*a[62]; tot+=(udpint)a[47]*a[61]; tot+=(udpint)a[48]*a[60]; tot+=(udpint)a[49]*a[59]; tot+=(udpint)a[50]*a[58]; tot+=(udpint)a[51]*a[57]; tot+=(udpint)a[52]*a[56]; tot+=(udpint)a[53]*a[55]; tot*=2; tot+=(udpint)a[54]*a[54]; t+=tot;  t+=(udpint)v40*p68;  c[39]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[41]*a[68]; tot+=(udpint)a[42]*a[67]; tot+=(udpint)a[43]*a[66]; tot+=(udpint)a[44]*a[65]; tot+=(udpint)a[45]*a[64]; tot+=(udpint)a[46]*a[63]; tot+=(udpint)a[47]*a[62]; tot+=(udpint)a[48]*a[61]; tot+=(udpint)a[49]*a[60]; tot+=(udpint)a[50]*a[59]; tot+=(udpint)a[51]*a[58]; tot+=(udpint)a[52]*a[57]; tot+=(udpint)a[53]*a[56]; tot+=(udpint)a[54]*a[55]; tot*=2; t+=tot;  t+=(udpint)v41*p68;  c[40]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[42]*a[68]; tot+=(udpint)a[43]*a[67]; tot+=(udpint)a[44]*a[66]; tot+=(udpint)a[45]*a[65]; tot+=(udpint)a[46]*a[64]; tot+=(udpint)a[47]*a[63]; tot+=(udpint)a[48]*a[62]; tot+=(udpint)a[49]*a[61]; tot+=(udpint)a[50]*a[60]; tot+=(udpint)a[51]*a[59]; tot+=(udpint)a[52]*a[58]; tot+=(udpint)a[53]*a[57]; tot+=(udpint)a[54]*a[56]; tot*=2; tot+=(udpint)a[55]*a[55]; t+=tot;  t+=(udpint)v42*p68;  c[41]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[43]*a[68]; tot+=(udpint)a[44]*a[67]; tot+=(udpint)a[45]*a[66]; tot+=(udpint)a[46]*a[65]; tot+=(udpint)a[47]*a[64]; tot+=(udpint)a[48]*a[63]; tot+=(udpint)a[49]*a[62]; tot+=(udpint)a[50]*a[61]; tot+=(udpint)a[51]*a[60]; tot+=(udpint)a[52]*a[59]; tot+=(udpint)a[53]*a[58]; tot+=(udpint)a[54]*a[57]; tot+=(udpint)a[55]*a[56]; tot*=2; t+=tot;  t+=(udpint)v43*p68;  c[42]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[44]*a[68]; tot+=(udpint)a[45]*a[67]; tot+=(udpint)a[46]*a[66]; tot+=(udpint)a[47]*a[65]; tot+=(udpint)a[48]*a[64]; tot+=(udpint)a[49]*a[63]; tot+=(udpint)a[50]*a[62]; tot+=(udpint)a[51]*a[61]; tot+=(udpint)a[52]*a[60]; tot+=(udpint)a[53]*a[59]; tot+=(udpint)a[54]*a[58]; tot+=(udpint)a[55]*a[57]; tot*=2; tot+=(udpint)a[56]*a[56]; t+=tot;  t+=(udpint)v44*p68;  c[43]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[45]*a[68]; tot+=(udpint)a[46]*a[67]; tot+=(udpint)a[47]*a[66]; tot+=(udpint)a[48]*a[65]; tot+=(udpint)a[49]*a[64]; tot+=(udpint)a[50]*a[63]; tot+=(udpint)a[51]*a[62]; tot+=(udpint)a[52]*a[61]; tot+=(udpint)a[53]*a[60]; tot+=(udpint)a[54]*a[59]; tot+=(udpint)a[55]*a[58]; tot+=(udpint)a[56]*a[57]; tot*=2; t+=tot;  t+=(udpint)v45*p68;  c[44]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[46]*a[68]; tot+=(udpint)a[47]*a[67]; tot+=(udpint)a[48]*a[66]; tot+=(udpint)a[49]*a[65]; tot+=(udpint)a[50]*a[64]; tot+=(udpint)a[51]*a[63]; tot+=(udpint)a[52]*a[62]; tot+=(udpint)a[53]*a[61]; tot+=(udpint)a[54]*a[60]; tot+=(udpint)a[55]*a[59]; tot+=(udpint)a[56]*a[58]; tot*=2; tot+=(udpint)a[57]*a[57]; t+=tot;  t+=(udpint)v46*p68;  c[45]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[47]*a[68]; tot+=(udpint)a[48]*a[67]; tot+=(udpint)a[49]*a[66]; tot+=(udpint)a[50]*a[65]; tot+=(udpint)a[51]*a[64]; tot+=(udpint)a[52]*a[63]; tot+=(udpint)a[53]*a[62]; tot+=(udpint)a[54]*a[61]; tot+=(udpint)a[55]*a[60]; tot+=(udpint)a[56]*a[59]; tot+=(udpint)a[57]*a[58]; tot*=2; t+=tot;  t+=(udpint)v47*p68;  c[46]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[48]*a[68]; tot+=(udpint)a[49]*a[67]; tot+=(udpint)a[50]*a[66]; tot+=(udpint)a[51]*a[65]; tot+=(udpint)a[52]*a[64]; tot+=(udpint)a[53]*a[63]; tot+=(udpint)a[54]*a[62]; tot+=(udpint)a[55]*a[61]; tot+=(udpint)a[56]*a[60]; tot+=(udpint)a[57]*a[59]; tot*=2; tot+=(udpint)a[58]*a[58]; t+=tot;  t+=(udpint)v48*p68;  c[47]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[49]*a[68]; tot+=(udpint)a[50]*a[67]; tot+=(udpint)a[51]*a[66]; tot+=(udpint)a[52]*a[65]; tot+=(udpint)a[53]*a[64]; tot+=(udpint)a[54]*a[63]; tot+=(udpint)a[55]*a[62]; tot+=(udpint)a[56]*a[61]; tot+=(udpint)a[57]*a[60]; tot+=(udpint)a[58]*a[59]; tot*=2; t+=tot;  t+=(udpint)v49*p68;  c[48]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[50]*a[68]; tot+=(udpint)a[51]*a[67]; tot+=(udpint)a[52]*a[66]; tot+=(udpint)a[53]*a[65]; tot+=(udpint)a[54]*a[64]; tot+=(udpint)a[55]*a[63]; tot+=(udpint)a[56]*a[62]; tot+=(udpint)a[57]*a[61]; tot+=(udpint)a[58]*a[60]; tot*=2; tot+=(udpint)a[59]*a[59]; t+=tot;  t+=(udpint)v50*p68;  c[49]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[51]*a[68]; tot+=(udpint)a[52]*a[67]; tot+=(udpint)a[53]*a[66]; tot+=(udpint)a[54]*a[65]; tot+=(udpint)a[55]*a[64]; tot+=(udpint)a[56]*a[63]; tot+=(udpint)a[57]*a[62]; tot+=(udpint)a[58]*a[61]; tot+=(udpint)a[59]*a[60]; tot*=2; t+=tot;  t+=(udpint)v51*p68;  c[50]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[52]*a[68]; tot+=(udpint)a[53]*a[67]; tot+=(udpint)a[54]*a[66]; tot+=(udpint)a[55]*a[65]; tot+=(udpint)a[56]*a[64]; tot+=(udpint)a[57]*a[63]; tot+=(udpint)a[58]*a[62]; tot+=(udpint)a[59]*a[61]; tot*=2; tot+=(udpint)a[60]*a[60]; t+=tot;  t+=(udpint)v52*p68;  c[51]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[53]*a[68]; tot+=(udpint)a[54]*a[67]; tot+=(udpint)a[55]*a[66]; tot+=(udpint)a[56]*a[65]; tot+=(udpint)a[57]*a[64]; tot+=(udpint)a[58]*a[63]; tot+=(udpint)a[59]*a[62]; tot+=(udpint)a[60]*a[61]; tot*=2; t+=tot;  t+=(udpint)v53*p68;  c[52]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[54]*a[68]; tot+=(udpint)a[55]*a[67]; tot+=(udpint)a[56]*a[66]; tot+=(udpint)a[57]*a[65]; tot+=(udpint)a[58]*a[64]; tot+=(udpint)a[59]*a[63]; tot+=(udpint)a[60]*a[62]; tot*=2; tot+=(udpint)a[61]*a[61]; t+=tot;  t+=(udpint)v54*p68;  c[53]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[55]*a[68]; tot+=(udpint)a[56]*a[67]; tot+=(udpint)a[57]*a[66]; tot+=(udpint)a[58]*a[65]; tot+=(udpint)a[59]*a[64]; tot+=(udpint)a[60]*a[63]; tot+=(udpint)a[61]*a[62]; tot*=2; t+=tot;  t+=(udpint)v55*p68;  c[54]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[56]*a[68]; tot+=(udpint)a[57]*a[67]; tot+=(udpint)a[58]*a[66]; tot+=(udpint)a[59]*a[65]; tot+=(udpint)a[60]*a[64]; tot+=(udpint)a[61]*a[63]; tot*=2; tot+=(udpint)a[62]*a[62]; t+=tot;  t+=(udpint)v56*p68;  c[55]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[57]*a[68]; tot+=(udpint)a[58]*a[67]; tot+=(udpint)a[59]*a[66]; tot+=(udpint)a[60]*a[65]; tot+=(udpint)a[61]*a[64]; tot+=(udpint)a[62]*a[63]; tot*=2; t+=tot;  t+=(udpint)v57*p68;  c[56]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[58]*a[68]; tot+=(udpint)a[59]*a[67]; tot+=(udpint)a[60]*a[66]; tot+=(udpint)a[61]*a[65]; tot+=(udpint)a[62]*a[64]; tot*=2; tot+=(udpint)a[63]*a[63]; t+=tot;  t+=(udpint)v58*p68;  c[57]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[59]*a[68]; tot+=(udpint)a[60]*a[67]; tot+=(udpint)a[61]*a[66]; tot+=(udpint)a[62]*a[65]; tot+=(udpint)a[63]*a[64]; tot*=2; t+=tot;  t+=(udpint)v59*p68;  c[58]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[60]*a[68]; tot+=(udpint)a[61]*a[67]; tot+=(udpint)a[62]*a[66]; tot+=(udpint)a[63]*a[65]; tot*=2; tot+=(udpint)a[64]*a[64]; t+=tot;  t+=(udpint)v60*p68;  c[59]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[61]*a[68]; tot+=(udpint)a[62]*a[67]; tot+=(udpint)a[63]*a[66]; tot+=(udpint)a[64]*a[65]; tot*=2; t+=tot;  t+=(udpint)v61*p68;  c[60]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[62]*a[68]; tot+=(udpint)a[63]*a[67]; tot+=(udpint)a[64]*a[66]; tot*=2; tot+=(udpint)a[65]*a[65]; t+=tot;  t+=(udpint)v62*p68;  c[61]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[63]*a[68]; tot+=(udpint)a[64]*a[67]; tot+=(udpint)a[65]*a[66]; tot*=2; t+=tot;  t+=(udpint)v63*p68;  c[62]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[64]*a[68]; tot+=(udpint)a[65]*a[67]; tot*=2; tot+=(udpint)a[66]*a[66]; t+=tot;  t+=(udpint)v64*p68;  c[63]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[65]*a[68]; tot+=(udpint)a[66]*a[67]; tot*=2; t+=tot;  t+=(udpint)v65*p68;  c[64]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[66]*a[68]; tot*=2; tot+=(udpint)a[67]*a[67]; t+=tot;  t+=(udpint)v66*p68;  c[65]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[67]*a[68]; tot*=2; t+=tot;  t+=(udpint)v67*p68;  c[66]=((spint)t & mask);  t>>=60;
	tot=(udpint)a[68]*a[68]; t+=tot;  t+=(udpint)v68*p68;  c[67]=((spint)t & mask);  t>>=60;
	c[68] = (spint)t;
}

//copy
static void modcpy(const spint *a,spint *c) {
	int i;
	for (i=0;i<69;i++) {
		c[i]=a[i];
	}
}

//square n times
static void modnsqr(spint *a,int n) {
	int i;
	for (i=0;i<n;i++) {
		modsqr(a,a);
	}
}

//Calculate progenitor
static void modpro(const spint *w,spint *z) {
	spint x[69];
	spint t0[69];
	spint t1[69];
	spint t2[69];
	spint t3[69];
	modcpy(w,x);
	modsqr(x,z);
	modmul(x,z,t0);
	modcpy(t0,z);
	modnsqr(z,2);
	modmul(t0,z,z);
	modnsqr(z,2);
	modmul(t0,z,z);
	modcpy(z,t1);
	modnsqr(t1,2);
	modmul(t0,t1,t1);
	modnsqr(t1,6);
	modmul(z,t1,z);
	modcpy(z,t1);
	modnsqr(t1,2);
	modmul(t0,t1,t1);
	modnsqr(t1,14);
	modmul(z,t1,z);
	modcpy(z,t1);
	modnsqr(t1,30);
	modmul(z,t1,z);
	modcpy(z,t1);
	modnsqr(t1,60);
	modmul(z,t1,z);
	modcpy(z,t1);
	modnsqr(t1,2);
	modmul(t0,t1,t1);
	modmul(x,t1,t0);
	modsqr(t0,t2);
	modmul(t0,t2,t2);
	modsqr(t2,t2);
	modmul(t0,t2,t3);
	modnsqr(t3,3);
	modmul(t2,t3,t2);
	modmul(t1,t2,t1);
	modmul(t0,t1,t0);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t1);
	modnsqr(t1,128);
	modmul(t0,t1,t0);
	modnsqr(t0,120);
	modmul(z,t0,z);
}

//calculate inverse, provide progenitor h if available
static void modinv(const spint *x,const spint *h,spint *z) {
	spint s[69];
	spint t[69];
	if (h==NULL) {
		modpro(x,t);
	} else {
		modcpy(h,t);
	}
	modcpy(x,s);
	modnsqr(t,2);
	modmul(s,t,z);
}

//Convert m to n-residue form, n=nres(m) 
static void nres(const spint *m,spint *n) {
	const spint c[69]={0x1969b71d79f8218u,0x104114935155976u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x104104104104104u,0x284u};
	modmul(m,c,n);
}

//Convert n back to normal form, m=redc(n) 
static void redc(const spint *n,spint *m) {
	int i;
	spint c[69];
	c[0]=1;
	for (i=1;i<69;i++) {
		c[i]=0;
	}
	modmul(n,c,m);
	(void)modfsb(m);
}

//is unity?
static int modis1(const spint *a) {
	int i;
	spint c[69];
	spint c0;
	spint d=0;
	redc(a,c);
	for (i=1;i<69;i++) {
		d|=c[i];
	}
	c0=(spint)c[0];
	return ((spint)1 & ((d-(spint)1)>>60u) & (((c0^(spint)1)-(spint)1)>>60u));
}

//is zero?
static int modis0(const spint *a) {
	int i;
	spint c[69];
	spint d=0;
	redc(a,c);
	for (i=0;i<69;i++) {
		d|=c[i];
	}
	return ((spint)1 & ((d-(spint)1)>>60u));
}

//set to zero
static void modzer(spint *a) {
	int i;
	for (i=0;i<69;i++) {
		a[i]=0;
	}
}

//set to one
static void modone(spint *a) {
	int i;
	a[0]=1;
	for (i=1;i<69;i++) {
		a[i]=0;
	}
	nres(a,a);
}

//set to integer
static void modint(int x,spint *a) {
	int i;
	a[0]=(spint)x;
	for (i=1;i<69;i++) {
		a[i]=0;
	}
	nres(a,a);
}

// Modular multiplication by an integer, c=a*b mod 2p
// uses special method for trinomials, otherwise Barrett-Dhem reduction
static void modmli(const spint *a,int b,spint *c) {
	spint p68=0x3f0u;
	spint mask=((spint)1<<60u)-(spint)1;
	udpint t=0;
	spint q,h,r=0x1041041041041041;
	t+=(udpint)a[0]*(udpint)b; c[0]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[1]*(udpint)b; c[1]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[2]*(udpint)b; c[2]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[3]*(udpint)b; c[3]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[4]*(udpint)b; c[4]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[5]*(udpint)b; c[5]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[6]*(udpint)b; c[6]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[7]*(udpint)b; c[7]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[8]*(udpint)b; c[8]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[9]*(udpint)b; c[9]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[10]*(udpint)b; c[10]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[11]*(udpint)b; c[11]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[12]*(udpint)b; c[12]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[13]*(udpint)b; c[13]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[14]*(udpint)b; c[14]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[15]*(udpint)b; c[15]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[16]*(udpint)b; c[16]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[17]*(udpint)b; c[17]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[18]*(udpint)b; c[18]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[19]*(udpint)b; c[19]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[20]*(udpint)b; c[20]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[21]*(udpint)b; c[21]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[22]*(udpint)b; c[22]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[23]*(udpint)b; c[23]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[24]*(udpint)b; c[24]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[25]*(udpint)b; c[25]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[26]*(udpint)b; c[26]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[27]*(udpint)b; c[27]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[28]*(udpint)b; c[28]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[29]*(udpint)b; c[29]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[30]*(udpint)b; c[30]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[31]*(udpint)b; c[31]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[32]*(udpint)b; c[32]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[33]*(udpint)b; c[33]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[34]*(udpint)b; c[34]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[35]*(udpint)b; c[35]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[36]*(udpint)b; c[36]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[37]*(udpint)b; c[37]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[38]*(udpint)b; c[38]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[39]*(udpint)b; c[39]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[40]*(udpint)b; c[40]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[41]*(udpint)b; c[41]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[42]*(udpint)b; c[42]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[43]*(udpint)b; c[43]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[44]*(udpint)b; c[44]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[45]*(udpint)b; c[45]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[46]*(udpint)b; c[46]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[47]*(udpint)b; c[47]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[48]*(udpint)b; c[48]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[49]*(udpint)b; c[49]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[50]*(udpint)b; c[50]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[51]*(udpint)b; c[51]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[52]*(udpint)b; c[52]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[53]*(udpint)b; c[53]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[54]*(udpint)b; c[54]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[55]*(udpint)b; c[55]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[56]*(udpint)b; c[56]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[57]*(udpint)b; c[57]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[58]*(udpint)b; c[58]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[59]*(udpint)b; c[59]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[60]*(udpint)b; c[60]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[61]*(udpint)b; c[61]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[62]*(udpint)b; c[62]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[63]*(udpint)b; c[63]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[64]*(udpint)b; c[64]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[65]*(udpint)b; c[65]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[66]*(udpint)b; c[66]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[67]*(udpint)b; c[67]=(spint)t & mask; t=t>>60u;
	t+=(udpint)a[68]*(udpint)b; c[68]=(spint)t;
	
//Barrett-Dhem reduction
	h = (spint)(t>>6u);
	q=(spint)(((udpint)h*(udpint)r)>>64u);
	c[0]+=q;
	c[68]-=q*p68;
}

//Test for quadratic residue 
static int modqr(const spint *h,const spint *x) {
	spint r[69];
	if (h==NULL) {
		modpro(x,r);
		modsqr(r,r);
	} else {
		modsqr(h,r);
	}
	modmul(r,x,r);
	return modis1(r) | modis0(x);
}

//conditional move g to f if d=1
//strongly recommend inlining be disabled using compiler specific syntax
static void __attribute__ ((noinline)) modcmv(int b,const spint *g,volatile spint *f) {
	int i;
	spint c0,c1,s,t,w,aux;
	static spint R=0;
	R+=0x3cc3c33c5aa5a55au;
	w=R;
		c0=(~b)&(w+1);
		c1=b+w;
	for (i=0;i<69;i++) {
		s=g[i]; t=f[i];
		f[i] = aux = c0*t+c1*s;
		f[i] = aux - w*(t+s);
	}
}

//conditional swap g and f if d=1
//strongly recommend inlining be disabled using compiler specific syntax
static void __attribute__ ((noinline)) modcsw(int b,volatile spint *g,volatile spint *f) {
	int i;
	spint c0,c1,s,t,w,v,aux;
	static spint R=0;
	R+=0x3cc3c33c5aa5a55au;
	w=R;
		c0=(~b)&(w+1);
		c1=b+w;
	for (i=0;i<69;i++) {
		s=g[i]; t=f[i];
		v=w*(t+s);
		f[i] = aux = c0*t+c1*s;
		f[i] = aux - v;
		g[i] = aux = c0*s+c1*t;
		g[i] = aux - v;
	}
}

//Modular square root, provide progenitor h if available, NULL if not
static void modsqrt(const spint *x,const spint *h,spint *r) {
	spint s[69];
	spint y[69];
	if (h==NULL) {
		modpro(x,y);
	} else {
		modcpy(h,y);
	}
	modmul(y,x,s);
	modcpy(s,r);
}

//shift left by less than a word
static void modshl(unsigned int n,spint *a) {
	int i;
	a[68]=((a[68]<<n)) | (a[67]>>(60u-n));
	for (i=67;i>0;i--) {
		a[i]=((a[i]<<n)&(spint)0xfffffffffffffff) | (a[i-1]>>(60u-n));
	}
	a[0]=(a[0]<<n)&(spint)0xfffffffffffffff;
}

//shift right by less than a word. Return shifted out part
static int modshr(unsigned int n,spint *a) {
	int i;
	spint r=a[0]&(((spint)1<<n)-(spint)1);
	for (i=0;i<68;i++) {
		a[i]=(a[i]>>n) | ((a[i+1]<<(60u-n))&(spint)0xfffffffffffffff);
	}
	a[68]=a[68]>>n;
	return r;
}

//set a= 2^r
static void mod2r(unsigned int r,spint *a) {
	unsigned int n=r/60u;
	unsigned int m=r%60u;
	modzer(a);
	if (r>=512*8) return;
	a[n]=1; a[n]<<=m;
nres(a,a);
}

//export to byte array
static void modexp(const spint *a,char *b) {
	int i;
	spint c[69];
	redc(a,c);
	for (i=511;i>=0;i--) {
		b[i]=c[0]&(spint)0xff;
		(void)modshr(8,c);
	}
}

//import from byte array
//returns 1 if in range, else 0
static int modimp(const char *b, spint *a) {
	int i,res;
	for (i=0;i<69;i++) {
		a[i]=0;
	}
	for (i=0;i<512;i++) {
		modshl(8,a);
		a[0]+=(spint)(unsigned char)b[i];
	}
	res=modfsb(a);
	nres(a,a);
	return res;
}

//determine sign
static int modsign(const spint *a) {
	spint c[69];
	redc(a,c);
	return c[0]%2;
}

//return true if equal
static int modcmp(const spint *a,const spint *b) {
	spint c[69],d[69];
	int i,eq=1;
	redc(a,c);
	redc(b,d);
	for (i=0;i<69;i++) {
		eq&=(((c[i]^d[i])-1)>>60)&1;
	}
	return eq;
}


// clang-format on
/******************************************************************************
 API functions calling generated code above
 ******************************************************************************/

#include <fp.h>

const digit_t ZERO[NWORDS_FIELD] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t ONE[NWORDS_FIELD] = {0x4104104104104, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x40};
// Montgomery representation of 2^-1
static const digit_t TWO_INV[NWORDS_FIELD] = {0x2082082082082, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20};
// Montgomery representation of 3^-1
static const digit_t THREE_INV[NWORDS_FIELD] = {0x556b015ac056b01, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x555555555555555, 0x165};
// Montgomery representation of 2^4096
static const digit_t R2[NWORDS_FIELD] = {0x149351559761969, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x41041041041041, 0x2d1};


void
fp_set_small(fp_t *x, const digit_t val)
{
    modint((int)val, *x);
}

void
fp_mul_small(fp_t *x, const fp_t *a, const uint32_t val)
{
    modmli(*a, (int)val, *x);
}

void
fp_set_zero(fp_t *x)
{
    modzer(*x);
}

void
fp_set_one(fp_t *x)
{
    modone(*x);
}

uint32_t
fp_is_equal(const fp_t *a, const fp_t *b)
{
    return -(uint32_t)modcmp(*a, *b);
}

uint32_t
fp_is_zero(const fp_t *a)
{
    return -(uint32_t)modis0(*a);
}

void
fp_copy(fp_t *out, const fp_t *a)
{
    modcpy(*a, *out);
}

void
fp_cswap(fp_t *a, fp_t *b, uint32_t ctl)
{
    modcsw((int)(ctl & 0x1), *a, *b);
}

void
fp_add(fp_t *out, const fp_t *a, const fp_t *b)
{
    modadd(*a, *b, *out);
}

void
fp_sub(fp_t *out, const fp_t *a, const fp_t *b)
{
    modsub(*a, *b, *out);
}

void
fp_neg(fp_t *out, const fp_t *a)
{
    modneg(*a, *out);
}

void
fp_sqr(fp_t *out, const fp_t *a)
{
    modsqr(*a, *out);
}

void
fp_mul(fp_t *out, const fp_t *a, const fp_t *b)
{
    modmul(*a, *b, *out);
}

void
fp_inv(fp_t *x)
{
    modinv(*x, NULL, *x);
}

uint32_t
fp_is_square(const fp_t *a)
{
    return -(uint32_t)modqr(NULL, *a);
}

void
fp_sqrt(fp_t *a)
{
    modsqrt(*a, NULL, *a);
}

void
fp_half(fp_t *out, const fp_t *a)
{
    modmul(TWO_INV, *a, *out);
}

void
fp_exp3div4(fp_t *out, const fp_t *a)
{
    modpro(*a, *out);
}

void
fp_div3(fp_t *out, const fp_t *a)
{
    modmul(THREE_INV, *a, *out);
}
void
fp_encode(void *dst, const fp_t *a)
{
    // Modified version of modexp()
    int i;
    spint c[69];
    redc(*a, c);
    for (i = 0; i < 512; i++) {
        ((char *)dst)[i] = c[0] & (spint)0xff;
        (void)modshr(8, c);
    }
}
uint32_t
fp_decode(fp_t *d, const void *src)
{
    // Modified version of modimp()
    int i;
    spint res;
    const unsigned char *b = src;
    for (i = 0; i < 69; i++) {
        (*d)[i] = 0;
    }
    for (i = 511; i >= 0; i--) {
        modshl(8, *d);
        (*d)[0] += (spint)b[i];
    }
    res = (spint)-modfsb(*d);
    nres(*d, *d);
    // If the value was canonical then res = -1; otherwise, res = 0
    for (i = 0; i < 69; i++) {
        (*d)[i] &= res;
    }
    return (uint32_t)res;
}

static inline unsigned char
add_carry(unsigned char cc, spint a, spint b, spint *d)
{
    udpint t = (udpint)a + (udpint)b + cc;
    *d = (spint)t;
    return (unsigned char)(t >> Wordlength);
}

static void
partial_reduce(spint *out, const spint *src)
{
    spint h, l, quo, rem;
    unsigned char cc;

    // Split value in high (12 bits) and low (4084 bits) parts.
    h = src[63] >> 52;
    l = src[63] & 0x000FFFFFFFFFFFFF;

    // 63*2^4084 = 1 mod q; hence, we add floor(h/63) + (h mod 63)*2^4084
    // to the low part.
    quo = (h * 0x1042) >> 18;
    rem = h - (63 * quo);
    cc = add_carry(0, src[0], quo, &out[0]);
    cc = add_carry(cc, src[1], 0, &out[1]);
    cc = add_carry(cc, src[2], 0, &out[2]);
    cc = add_carry(cc, src[3], 0, &out[3]);
    cc = add_carry(cc, src[4], 0, &out[4]);
    cc = add_carry(cc, src[5], 0, &out[5]);
    cc = add_carry(cc, src[6], 0, &out[6]);
    cc = add_carry(cc, src[7], 0, &out[7]);
    cc = add_carry(cc, src[8], 0, &out[8]);
    cc = add_carry(cc, src[9], 0, &out[9]);
    cc = add_carry(cc, src[10], 0, &out[10]);
    cc = add_carry(cc, src[11], 0, &out[11]);
    cc = add_carry(cc, src[12], 0, &out[12]);
    cc = add_carry(cc, src[13], 0, &out[13]);
    cc = add_carry(cc, src[14], 0, &out[14]);
    cc = add_carry(cc, src[15], 0, &out[15]);
    cc = add_carry(cc, src[16], 0, &out[16]);
    cc = add_carry(cc, src[17], 0, &out[17]);
    cc = add_carry(cc, src[18], 0, &out[18]);
    cc = add_carry(cc, src[19], 0, &out[19]);
    cc = add_carry(cc, src[20], 0, &out[20]);
    cc = add_carry(cc, src[21], 0, &out[21]);
    cc = add_carry(cc, src[22], 0, &out[22]);
    cc = add_carry(cc, src[23], 0, &out[23]);
    cc = add_carry(cc, src[24], 0, &out[24]);
    cc = add_carry(cc, src[25], 0, &out[25]);
    cc = add_carry(cc, src[26], 0, &out[26]);
    cc = add_carry(cc, src[27], 0, &out[27]);
    cc = add_carry(cc, src[28], 0, &out[28]);
    cc = add_carry(cc, src[29], 0, &out[29]);
    cc = add_carry(cc, src[30], 0, &out[30]);
    cc = add_carry(cc, src[31], 0, &out[31]);
    cc = add_carry(cc, src[32], 0, &out[32]);
    cc = add_carry(cc, src[33], 0, &out[33]);
    cc = add_carry(cc, src[34], 0, &out[34]);
    cc = add_carry(cc, src[35], 0, &out[35]);
    cc = add_carry(cc, src[36], 0, &out[36]);
    cc = add_carry(cc, src[37], 0, &out[37]);
    cc = add_carry(cc, src[38], 0, &out[38]);
    cc = add_carry(cc, src[39], 0, &out[39]);
    cc = add_carry(cc, src[40], 0, &out[40]);
    cc = add_carry(cc, src[41], 0, &out[41]);
    cc = add_carry(cc, src[42], 0, &out[42]);
    cc = add_carry(cc, src[43], 0, &out[43]);
    cc = add_carry(cc, src[44], 0, &out[44]);
    cc = add_carry(cc, src[45], 0, &out[45]);
    cc = add_carry(cc, src[46], 0, &out[46]);
    cc = add_carry(cc, src[47], 0, &out[47]);
    cc = add_carry(cc, src[48], 0, &out[48]);
    cc = add_carry(cc, src[49], 0, &out[49]);
    cc = add_carry(cc, src[50], 0, &out[50]);
    cc = add_carry(cc, src[51], 0, &out[51]);
    cc = add_carry(cc, src[52], 0, &out[52]);
    cc = add_carry(cc, src[53], 0, &out[53]);
    cc = add_carry(cc, src[54], 0, &out[54]);
    cc = add_carry(cc, src[55], 0, &out[55]);
    cc = add_carry(cc, src[56], 0, &out[56]);
    cc = add_carry(cc, src[57], 0, &out[57]);
    cc = add_carry(cc, src[58], 0, &out[58]);
    cc = add_carry(cc, src[59], 0, &out[59]);
    cc = add_carry(cc, src[60], 0, &out[60]);
    cc = add_carry(cc, src[61], 0, &out[61]);
    cc = add_carry(cc, src[62], 0, &out[62]);
    (void)add_carry(cc, l, rem << 52, &out[63]);
}

// Little-endian encoding of a 64-bit integer.
static inline void
enc64le(void *dst, uint64_t x)
{
    uint8_t *buf = dst;
    buf[0] = (uint8_t)x;
    buf[1] = (uint8_t)(x >> 8);
    buf[2] = (uint8_t)(x >> 16);
    buf[3] = (uint8_t)(x >> 24);
    buf[4] = (uint8_t)(x >> 32);
    buf[5] = (uint8_t)(x >> 40);
    buf[6] = (uint8_t)(x >> 48);
    buf[7] = (uint8_t)(x >> 56);
}

// Little-endian decoding of a 64-bit integer.
static inline uint64_t
dec64le(const void *src)
{
    const uint8_t *buf = src;
    return (spint)buf[0] | ((spint)buf[1] << 8) | ((spint)buf[2] << 16) | ((spint)buf[3] << 24) |
           ((spint)buf[4] << 32) | ((spint)buf[5] << 40) | ((spint)buf[6] << 48) | ((spint)buf[7] << 56);
}

void
fp_decode_reduce(fp_t *d, const void *src, size_t len)
{
    uint64_t t[64];   // Stores Nbytes * 8 bits
    uint8_t tmp[512]; // Nbytes
    const uint8_t *b = src;

    fp_set_zero(d);
    if (len == 0) {
        return;
    }

    size_t rem = len % 512;
    if (rem != 0) {
        // Input size is not a multiple of 512, we decode a partial
        // block, which is already less than 2^4084.
        size_t k = len - rem;
        memcpy(tmp, b + k, len - k);
        memset(tmp + len - k, 0, (sizeof tmp) - (len - k));
        fp_decode(d, tmp);
        len = k;
    }
    // Process all remaining blocks, in descending address order.
    while (len > 0) {
        fp_mul(d, d, &R2);
        len -= 512;
        t[0] = dec64le(b + len + 0);
        t[1] = dec64le(b + len + 8);
        t[2] = dec64le(b + len + 16);
        t[3] = dec64le(b + len + 24);
        t[4] = dec64le(b + len + 32);
        t[5] = dec64le(b + len + 40);
        t[6] = dec64le(b + len + 48);
        t[7] = dec64le(b + len + 56);
        t[8] = dec64le(b + len + 64);
        t[9] = dec64le(b + len + 72);
        t[10] = dec64le(b + len + 80);
        t[11] = dec64le(b + len + 88);
        t[12] = dec64le(b + len + 96);
        t[13] = dec64le(b + len + 104);
        t[14] = dec64le(b + len + 112);
        t[15] = dec64le(b + len + 120);
        t[16] = dec64le(b + len + 128);
        t[17] = dec64le(b + len + 136);
        t[18] = dec64le(b + len + 144);
        t[19] = dec64le(b + len + 152);
        t[20] = dec64le(b + len + 160);
        t[21] = dec64le(b + len + 168);
        t[22] = dec64le(b + len + 176);
        t[23] = dec64le(b + len + 184);
        t[24] = dec64le(b + len + 192);
        t[25] = dec64le(b + len + 200);
        t[26] = dec64le(b + len + 208);
        t[27] = dec64le(b + len + 216);
        t[28] = dec64le(b + len + 224);
        t[29] = dec64le(b + len + 232);
        t[30] = dec64le(b + len + 240);
        t[31] = dec64le(b + len + 248);
        t[32] = dec64le(b + len + 256);
        t[33] = dec64le(b + len + 264);
        t[34] = dec64le(b + len + 272);
        t[35] = dec64le(b + len + 280);
        t[36] = dec64le(b + len + 288);
        t[37] = dec64le(b + len + 296);
        t[38] = dec64le(b + len + 304);
        t[39] = dec64le(b + len + 312);
        t[40] = dec64le(b + len + 320);
        t[41] = dec64le(b + len + 328);
        t[42] = dec64le(b + len + 336);
        t[43] = dec64le(b + len + 344);
        t[44] = dec64le(b + len + 352);
        t[45] = dec64le(b + len + 360);
        t[46] = dec64le(b + len + 368);
        t[47] = dec64le(b + len + 376);
        t[48] = dec64le(b + len + 384);
        t[49] = dec64le(b + len + 392);
        t[50] = dec64le(b + len + 400);
        t[51] = dec64le(b + len + 408);
        t[52] = dec64le(b + len + 416);
        t[53] = dec64le(b + len + 424);
        t[54] = dec64le(b + len + 432);
        t[55] = dec64le(b + len + 440);
        t[56] = dec64le(b + len + 448);
        t[57] = dec64le(b + len + 456);
        t[58] = dec64le(b + len + 464);
        t[59] = dec64le(b + len + 472);
        t[60] = dec64le(b + len + 480);
        t[61] = dec64le(b + len + 488);
        t[62] = dec64le(b + len + 496);
        t[63] = dec64le(b + len + 504);
        partial_reduce(t, t);
        enc64le(tmp + 0, t[0]);
        enc64le(tmp + 8, t[1]);
        enc64le(tmp + 16, t[2]);
        enc64le(tmp + 24, t[3]);
        enc64le(tmp + 32, t[4]);
        enc64le(tmp + 40, t[5]);
        enc64le(tmp + 48, t[6]);
        enc64le(tmp + 56, t[7]);
        enc64le(tmp + 64, t[8]);
        enc64le(tmp + 72, t[9]);
        enc64le(tmp + 80, t[10]);
        enc64le(tmp + 88, t[11]);
        enc64le(tmp + 96, t[12]);
        enc64le(tmp + 104, t[13]);
        enc64le(tmp + 112, t[14]);
        enc64le(tmp + 120, t[15]);
        enc64le(tmp + 128, t[16]);
        enc64le(tmp + 136, t[17]);
        enc64le(tmp + 144, t[18]);
        enc64le(tmp + 152, t[19]);
        enc64le(tmp + 160, t[20]);
        enc64le(tmp + 168, t[21]);
        enc64le(tmp + 176, t[22]);
        enc64le(tmp + 184, t[23]);
        enc64le(tmp + 192, t[24]);
        enc64le(tmp + 200, t[25]);
        enc64le(tmp + 208, t[26]);
        enc64le(tmp + 216, t[27]);
        enc64le(tmp + 224, t[28]);
        enc64le(tmp + 232, t[29]);
        enc64le(tmp + 240, t[30]);
        enc64le(tmp + 248, t[31]);
        enc64le(tmp + 256, t[32]);
        enc64le(tmp + 264, t[33]);
        enc64le(tmp + 272, t[34]);
        enc64le(tmp + 280, t[35]);
        enc64le(tmp + 288, t[36]);
        enc64le(tmp + 296, t[37]);
        enc64le(tmp + 304, t[38]);
        enc64le(tmp + 312, t[39]);
        enc64le(tmp + 320, t[40]);
        enc64le(tmp + 328, t[41]);
        enc64le(tmp + 336, t[42]);
        enc64le(tmp + 344, t[43]);
        enc64le(tmp + 352, t[44]);
        enc64le(tmp + 360, t[45]);
        enc64le(tmp + 368, t[46]);
        enc64le(tmp + 376, t[47]);
        enc64le(tmp + 384, t[48]);
        enc64le(tmp + 392, t[49]);
        enc64le(tmp + 400, t[50]);
        enc64le(tmp + 408, t[51]);
        enc64le(tmp + 416, t[52]);
        enc64le(tmp + 424, t[53]);
        enc64le(tmp + 432, t[54]);
        enc64le(tmp + 440, t[55]);
        enc64le(tmp + 448, t[56]);
        enc64le(tmp + 456, t[57]);
        enc64le(tmp + 464, t[58]);
        enc64le(tmp + 472, t[59]);
        enc64le(tmp + 480, t[60]);
        enc64le(tmp + 488, t[61]);
        enc64le(tmp + 496, t[62]);
        enc64le(tmp + 504, t[63]);
        fp_t a;
        fp_decode(&a, tmp);
        fp_add(d, d, &a);
    }
}


