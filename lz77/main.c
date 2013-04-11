#include <stdio.h>
#include "lz77.h"

int main()
{
    char filename[50];   //存放输入的文件名
    FILE * pFile;        //文件指针
    BITFILE * pOutputBitFile;    //指向输出的BITFILE的指针
    BITFILE * pInputBitFile;    //指向输入的BITFILE的指针
    
    printf("请输入要压缩的文件名:");
    scanf("%s",filename);
    
    if ((pFile = fopen(filename, "rb"))!=NULL) {
        pOutputBitFile = OpenOutputBitFile("tempoutput.txt");    //中间文件文件名默认是可执行文件目录下的tempoutput.txt
        CompressFile(pFile, pOutputBitFile);
        fclose(pFile);
        CloseOutputBitFile(pOutputBitFile);
        printf("压缩完成！！\n");       //压缩完成
    }
    else{
        printf("无法打开此文件.\n");
        return 1;
    }
    
    pInputBitFile = OpenInputBitFile("tempoutput.txt");
    
    printf("请输入解压后文件名:");
    scanf("%s",filename);
    
    if ((pFile = fopen(filename, "wb"))!=NULL) {
        ExpandFile(pInputBitFile, pFile);
        fclose(pFile);
        CloseInputBitFile(pInputBitFile);
        printf("解压完成！！！\n");     //解压还原完成
    }
    else{
        printf("无法写入此文件.\n");
        return 1;
    }
    
}
