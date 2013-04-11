#ifndef lz77_lz77_h
#define lz77_lz77_h

#include "BITIO.h"

#define INDEX_BIT_COUNT       12
#define LENGTH_BIT_COUNT      4
#define WINDOW_SIZE           (1 << INDEX_BIT_COUNT)
#define RAW_LOOK_AHEAD_SIZE   (1 << LENGTH_BIT_COUNT)

#define BREAK_EVEN            ((1 + INDEX_BIT_COUNT + LENGTH_BIT_COUNT) / 9)
#define LOOK_AHEAD_SIZE       (RAW_LOOK_AHEAD_SIZE + BREAK_EVEN)

#define TREE_ROOT             WINDOW_SIZE
#define END_OF_STREAM         0
#define MOD_WINDOW(A)         ((A) & (WINDOW_SIZE - 1))

unsigned char window[WINDOW_SIZE];
unsigned char buf[LOOK_AHEAD_SIZE];

void CompressFile(FILE * inputFile, BITFILE * outputFile);
void ExpandFile(BITFILE * inputFile, FILE * outputFile);

#endif
