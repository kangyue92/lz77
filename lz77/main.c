#include <stdio.h>
#include "lz77.h"

int main()
{
    char filename[50];
    FILE * pFile;
    BITFILE * pOutputBitFile;
    BITFILE * pInputBitFile;
    
    printf("请输入要压缩的文件名:");
    scanf("%s",filename);
    
    if ((pFile = fopen(filename, "rb"))!=NULL) {
        pOutputBitFile = OpenOutputBitFile("output.txt");
        CompressFile(pFile, pOutputBitFile);
        fclose(pFile);
        CloseOutputBitFile(pOutputBitFile);  //压缩完成
    }
    else{
        printf("无法打开此文件.\n");
        return 1;
    }
    
    pInputBitFile = OpenInputBitFile(filename);
    
    printf("请输入解压后文件名:");
    scanf("%s",filename);
    
    if ((pFile = fopen(filename, "wb"))!=NULL) {
        ExpandFile(pInputBitFile, pFile);
        fclose(pFile);
        CloseInputBitFile(pInputBitFile);   //解压还原完成
    }
    else{
        printf("无法写入此文件.\n");
        return 1;
    }
    
}
