#ifndef lz77_BITIO_h
#define lz77_BITIO_h

#include <stdio.h>

#define PACIFIER_COUNT 2047

typedef struct
{
	FILE * bitfile;
	int rack;
	unsigned char mask;
	int pacifier_counter;
} BITFILE;

BITFILE * OpenOutputBitFile(char * chFile);
void OutputBit(BITFILE * pBitFile, int bit);
void OutputBits(BITFILE * pBitFile, unsigned long code, int count);
void CloseOutputBitFile(BITFILE * pBitFile);

BITFILE * OpenInputBitFile(char * chFile);
int InputBit(BITFILE * pBitFile);
unsigned long InputBits(BITFILE * pBitFile, int bit_count);
void CloseInputBitFile(BITFILE * pBitFile);

void FilePrintBinary(FILE * file, unsigned int code, int bits);

void PrintInBinary(BITFILE * pBitFile);

#endif
