#include <stdlib.h>
#include <stdio.h>
#include "BITIO.h"

BITFILE * OpenOutputBitFile(char * chFile)
{
    BITFILE * pBitFile;
    
    pBitFile = (BITFILE *)malloc(sizeof(BITFILE));
    if (pBitFile == NULL)
        return NULL;
    
    pBitFile->bitfile = fopen(chFile, "wb");
    pBitFile->rack = 0;
    pBitFile->mask = 0x80;
    pBitFile->pacifier_counter = 0;
    
    return pBitFile;
}

void OutputBit(BITFILE * pBitFile, int bit)
{
    if (bit)
        pBitFile->rack |= pBitFile->mask;
    pBitFile->mask >>= 1;
    if (pBitFile->mask == 0)
    {
        if (pBitFile->rack != putc(pBitFile->rack, pBitFile->bitfile))
            printf("OutputBit: putc error.\n");
        else
        {
            pBitFile->pacifier_counter++;
            if ( (pBitFile->pacifier_counter & PACIFIER_COUNT) == 0)
                putc('.', stdout);
        }
        
        pBitFile->rack = 0;
        pBitFile->mask = 0x80;
    }
}

// 1 <= count <= sizeof(unsigned long) * 8
void OutputBits(BITFILE * pBitFile, unsigned long code, int count)
{
    unsigned long mask;
    
    mask = 1 << (count - 1);
    while (mask)
    {
        if (mask & code)
            pBitFile->rack |= pBitFile->mask;
        pBitFile->mask >>= 1;
        if (pBitFile->mask == 0)
        {
            if (pBitFile->rack != putc(pBitFile->rack, pBitFile->bitfile))
                printf("OutputBits: putc error.\n");
            else
            {
                pBitFile->pacifier_counter++;
                if ( (pBitFile->pacifier_counter & PACIFIER_COUNT) == 0)
                    putc('.', stdout);
            }
            
            pBitFile->rack = 0;
            pBitFile->mask = 0x80;
        }
        
        mask >>= 1;
    }
}


void CloseOutputBitFile(BITFILE * pBitFile)
{
    if (pBitFile->mask != 0x80)
    {
        //将最后一个字节写入文件。
        if (pBitFile->rack != putc(pBitFile->rack, pBitFile->bitfile))
            printf("CloseOutputBitFile: putc error.\n");
    }
    fclose(pBitFile->bitfile);
    free((void *)pBitFile);
}

BITFILE * OpenInputBitFile(char * chFile)
{
    BITFILE * pBitFile;
    
    pBitFile = (BITFILE *)malloc(sizeof(BITFILE));
    if (pBitFile == NULL)
        return NULL;
    
    pBitFile->bitfile = fopen(chFile, "rb");
    pBitFile->rack = 0;
    pBitFile->mask = 0x80;
    pBitFile->pacifier_counter = 0;
    
    return pBitFile;
}


int InputBit(BITFILE * pBitFile)
{
    int value;
    
    if (pBitFile->mask == 0x80)
    {
        pBitFile->rack = getc(pBitFile->bitfile);
        if (pBitFile->rack == EOF)
            printf("InputBit: EOF read.\n");
        
        pBitFile->pacifier_counter++;
        if ( (pBitFile->pacifier_counter & PACIFIER_COUNT) == 0)
            putc('.', stdout);
    }
    
    value = pBitFile->rack & pBitFile->mask;
    pBitFile->mask >>= 1;
    if (pBitFile->mask == 0)
        pBitFile->mask = 0x80;
    
    return (value ? 1 : 0);
}

unsigned long InputBits(BITFILE * pBitFile, int bit_count)
{
    unsigned long mask;
    unsigned long ret;
    
    mask = 1 << (bit_count - 1);
    ret = 0;
    while (mask != 0)
    {
        if (pBitFile->mask == 0x80)
        {
            pBitFile->rack = getc(pBitFile->bitfile);
            if (pBitFile->rack == EOF)
                printf("InputBit: EOF read.\n");
            
            pBitFile->pacifier_counter++;
            if ( (pBitFile->pacifier_counter & PACIFIER_COUNT) == 0)
                putc('.', stdout);
        }
        
        if (pBitFile->rack & pBitFile->mask)
            ret |= mask;
        pBitFile->mask >>= 1;
        if (pBitFile->mask == 0)
            pBitFile->mask = 0x80;
        
        mask >>= 1;
    }
    
    return ret;
}

void CloseInputBitFile(BITFILE * pBitFile)
{
    fclose(pBitFile->bitfile);
    free((void *)pBitFile);
}

//void FilePrintBinary(FILE * file, unsigned int code, int bits)
//{
//    unsigned int mask;
//    
//    mask = 1 << (bits - 1);
//    while (mask != 0)
//    {
//        if (code & mask)
//            fputc('1', file);
//        else
//            fputc('0', file);
//        
//        mask >>= 1;
//    }
//}
//
//void PrintInBinary(BITFILE * pBitFile)
//{
//    int value;
//    int count;
//    
//    count = 0;
//    while (1)
//    {
//        if (pBitFile->mask == 0x80)
//        {
//            count++;
//            pBitFile->rack = getc(pBitFile->bitfile);
//            if (pBitFile->rack == EOF)
//                break;
//        }
//        
//        value = pBitFile->rack & pBitFile->mask;
//        if (value)
//            fputc('1', stdout);
//        else
//            fputc('0', stdout);
//        
//        pBitFile->mask >>= 1;
//        if (pBitFile->mask == 0)
//        {
//            pBitFile->mask = 0x80;
//            fputc(' ', stdout);
//            fputc(' ', stdout);
//            if (count == 4)
//            {
//                fputc('\n', stdout);
//                count = 0;
//            }
//        }
//    }
//    printf("\n");
//}
//


