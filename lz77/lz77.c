//
//  lz77.c
//  lz77
//
//  Created by kangyue on 13-4-9.
//  Copyright (c) 2013年 zhangyali. All rights reserved.
//

#include <stdio.h>
/*
 * 源代码的思路参考自 Mark Nelson 所著的<<数据压缩技术原理与范例>>
 * 中的第八章"滑动窗口压缩"，是lz77算法的一种简介直观的实现，但是由于
 * 没有采用如LZSS算法中的二叉搜索树技术，所以在运行速度上不如LZSS算法。
 * 采用了微量缓冲区buf 以加快执行速度。
 *
 * created by chenyong 2008.03.03
 */

#include "lz77.h"

#include <memory.h>

void CompressFile(FILE * inputFile, BITFILE * outputFile)
{
    int i;
    int j;
    int c;
    int look_ahead_bytes;
    int current_pos;
    int replace_count;
    int match_length;
    int match_pos;
    int delta;
    
    //initialize window.
    memset(window, 0, WINDOW_SIZE * sizeof(unsigned char));
    
    current_pos = 1;
    for (i = 0; i < LOOK_AHEAD_SIZE; i++)
    {
        c = getc(inputFile);
        if (EOF == c)
            break;
        
        window[current_pos + i] = (unsigned char)c;
    }
    
    look_ahead_bytes = i;
    match_length = 0;
    match_pos = 0;
    
    while (look_ahead_bytes)
    {
        if (match_length > look_ahead_bytes)
            match_length = look_ahead_bytes;
        
        if (match_length <= BREAK_EVEN)
        {
            replace_count = 1;
            OutputBit(outputFile, 1);
            OutputBits(outputFile, (unsigned long)window[current_pos], 8);
        }
        else
        {
            replace_count = match_length;
            OutputBit(outputFile, 0);
            OutputBits(outputFile, (unsigned long)match_pos, INDEX_BIT_COUNT);
            OutputBits(outputFile, (unsigned long)(match_length - BREAK_EVEN - 1),
                       LENGTH_BIT_COUNT);
        }
        
        for (i = 0; i < replace_count; i++)
        {
            c = getc(inputFile);
            if (EOF == c)
                look_ahead_bytes--;
            else
                window[MOD_WINDOW(current_pos + LOOK_AHEAD_SIZE + i)] = (unsigned char)c;
        }
        current_pos = MOD_WINDOW(current_pos + replace_count);
        
        if (look_ahead_bytes)
        {
            //copy [current_pos, current_pos + 16] to buf.
            for (i = 0; i < LOOK_AHEAD_SIZE; i++)
                buf[i] = window[MOD_WINDOW(current_pos + i)];
            
            match_length = 0;
            
            for (i = MOD_WINDOW(current_pos + LOOK_AHEAD_SIZE); i != current_pos;
                 i = MOD_WINDOW(i + 1))
            {
                //位置END_OF_STREAM(0) 不可以作为match_pos.
                if (END_OF_STREAM == i)
                    continue;
                
                for (j = 0; j < LOOK_AHEAD_SIZE; j++)
                {
                    delta = window[MOD_WINDOW(i + j)] - buf[j];
                    if (0 != delta)
                        break;
                }
                if (j >= match_length)
                {
                    match_length = j;
                    match_pos = i;
                }
            }
        }
    }
    OutputBit(outputFile, 0);
    OutputBits(outputFile, (unsigned long)END_OF_STREAM, INDEX_BIT_COUNT);
}

void ExpandFile(BITFILE * inputFile, FILE * outputFile)
{
    int i;
    int current_pos;
    int c;
    int match_length;
    int match_pos;
    
    //initialize window.
    memset(window, 0, WINDOW_SIZE * sizeof(unsigned char));
    
    current_pos = 1;
    while (1)
    {
        if (InputBit(inputFile))
        {
            c = (int)InputBits(inputFile, 8);
            putc(c, outputFile);
            window[current_pos] = (unsigned char)c;
            current_pos = MOD_WINDOW(current_pos + 1);
        }
        else
        {
            match_pos = (int)InputBits(inputFile, INDEX_BIT_COUNT);
            if (END_OF_STREAM == match_pos)
                break;
            
            match_length = (int)InputBits(inputFile, LENGTH_BIT_COUNT);
            match_length += BREAK_EVEN;
            
            for (i = 0; i <= match_length; i++)
            {
                c = window[MOD_WINDOW(match_pos + i)];
                putc(c, outputFile);
                window[current_pos] = (unsigned char)c;
                current_pos = MOD_WINDOW(current_pos + 1);
            }
        }
    }
}

void testLZ77()
{
    FILE * pFile;
    BITFILE * pOutputBitFile;
    BITFILE * pInputBitFile;
    
    pFile = fopen("test.txt","rb");
    pOutputBitFile = OpenOutputBitFile("output.txt");
    CompressFile(pFile, pOutputBitFile);
    
    fclose(pFile);
    CloseOutputBitFile(pOutputBitFile);  // compress completed.
    
    pInputBitFile = OpenInputBitFile("output.txt");
    pFile = fopen("test_expand.txt","wb");
    ExpandFile(pInputBitFile, pFile);
    
    fclose(pFile);
    CloseInputBitFile(pInputBitFile);   //expand completed.
}