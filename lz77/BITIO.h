#ifndef lz77_BITIO_h
#define lz77_BITIO_h

#include <stdio.h>

#define PACIFIER_COUNT 2047

typedef struct
{
	FILE * bitfile;
	int rack;
	unsigned char mask;  //掩码
	int pacifier_counter;
} BITFILE;

BITFILE * OpenOutputBitFile(char * chFile);   //打开文件并初始化rack mask pacifier_counter，返回指向结构体BITFILE的指针
void OutputBit(BITFILE * pBitFile, int bit);  //从BITFILE中输出1bit
void OutputBits(BITFILE * pBitFile, unsigned long code, int count);  //从BITFILE中输出n个bit
void CloseOutputBitFile(BITFILE * pBitFile);  //写入最后一个字符，关闭文件，释放结构体

BITFILE * OpenInputBitFile(char * chFile);    //打开文件并初始化结构体中的变量，返回指向结构体BITFILE的指针
int InputBit(BITFILE * pBitFile);    //从BITEFILE里读入1bit
unsigned long InputBits(BITFILE * pBitFile, int bit_count);    //从BITFILE里读入n个bit
void CloseInputBitFile(BITFILE * pBitFile);    //关闭打开的文件，释放结构体

//void FilePrintBinary(FILE * file, unsigned int code, int bits);
//void PrintInBinary(BITFILE * pBitFile);

#endif
