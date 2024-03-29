#include <stdio.h>
#include "lz77.h"
#include <memory.h>

void CompressFile(FILE * inputFile, BITFILE * outputFile)
{
    int i;  //循环变量
    int j;
    int c;
    int look_ahead_bytes;
    int current_pos;
    int replace_count;
    int match_length;
    int match_pos;
    int flag;
    
    //初始化窗口
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
        if (match_length > look_ahead_bytes)   //如果向后看一位匹配到的长度比原来的长，则设置最大长度为新的长度
            match_length = look_ahead_bytes;
        
        if (match_length <= BREAK_EVEN)    //如果匹配到的长度比最小长度小,则单独输出此位置的字节,不进行编码
        {
            replace_count = 1;
            OutputBit(outputFile, 1);
            OutputBits(outputFile, (unsigned long)window[current_pos], 8);
        }
        else    //否则输出编码后的标志位,距离,长度
        {
            replace_count = match_length;
            OutputBit(outputFile, 0);
            OutputBits(outputFile, (unsigned long)match_pos, INDEX_BIT_COUNT);
            OutputBits(outputFile, (unsigned long)(match_length - BREAK_EVEN - 1),LENGTH_BIT_COUNT);
        }
        
        for (i = 0; i < replace_count; i++)   //移动当前位置指针到尚未处理的位置上
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
            //将[current_pos, current_pos + 16]存到buf里.
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
                    flag = window[MOD_WINDOW(i + j)] - buf[j];
                    if (0 != flag)
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
    OutputBit(outputFile, 0);    //输出结束标记
    OutputBits(outputFile, (unsigned long)END_OF_STREAM, INDEX_BIT_COUNT);
}

void ExpandFile(BITFILE * inputFile, FILE * outputFile)
{
    int i;
    int current_pos;
    int c;
    int match_length;
    int match_pos;
    
    //初始化窗口
    memset(window, 0, WINDOW_SIZE * sizeof(unsigned char));
    
    current_pos = 1;
    while (1)
    {
        if (InputBit(inputFile))    //如果读到1,则输出后面8位为一个字节
        {
            c = (int)InputBits(inputFile, 8);
            putc(c, outputFile);
            window[current_pos] = (unsigned char)c;
            current_pos = MOD_WINDOW(current_pos + 1);
        }
        else    //如果读到0,则按照,位置,长度,找到前面的相同部分,并输出
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
