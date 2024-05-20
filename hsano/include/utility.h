#ifndef UTILITY_H
#define UTILITY_H

#include <unistd.h>

//bool jumpImageAddress(int fd);
size_t jumpImageAddress(unsigned char *img, size_t max_size);
int cvt_hex_str_to_int_array(char *src, unsigned int *dst);
unsigned int read1Byte(int fd, bool *result);
unsigned int read2Byte(int fd, bool *result);
unsigned int read3Byte(int fd, bool *result);
unsigned int read4Byte(int fd, bool *result);

unsigned int strToUInt4Byte(unsigned char *data);
unsigned int strToUInt3Byte(unsigned char *data);
unsigned int strToUInt2Byte(unsigned char *data);
unsigned int strToUInt1Byte(unsigned char *data);

#endif
