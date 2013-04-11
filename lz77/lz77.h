#ifndef lz77_lz77_h
#define lz77_lz77_h

#include "BITIO.h"

#define INDEX_BIT_COUNT       12
#define LENGTH_BIT_COUNT      4
#define WINDOW_SIZE           (1 << INDEX_BIT_COUNT)   //窗口大小
#define RAW_LOOK_AHEAD_SIZE   (1 << LENGTH_BIT_COUNT)  //缓冲区大小

#define BREAK_EVEN            ((1 + INDEX_BIT_COUNT + LENGTH_BIT_COUNT) / 9)  //找到多长的串的时候启动替换。也就是最小的替换匹配长度
#define LOOK_AHEAD_SIZE       (RAW_LOOK_AHEAD_SIZE + BREAK_EVEN)

#define TREE_ROOT             WINDOW_SIZE
#define END_OF_STREAM         0
#define MOD_WINDOW(A)         ((A) & (WINDOW_SIZE - 1))  //除以窗口大小-1后的余数

unsigned char window[WINDOW_SIZE];    //定义窗口
unsigned char buf[LOOK_AHEAD_SIZE];    //定义缓冲区

void CompressFile(FILE * inputFile, BITFILE * outputFile);  //将inputFile指向的文件压缩到outputFile中的文件指针指向的文件
void ExpandFile(BITFILE * inputFile, FILE * outputFile);    //将inputFile中的文件指针指向的文件解压缩到outputFile指向的文件中

#endif
