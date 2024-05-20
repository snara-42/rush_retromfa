#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
//#include "utility.h"
#include <string.h>



unsigned int strToUInt4Byte(unsigned char *data)
{
    return (data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24));
}

unsigned int strToUInt3Byte(unsigned char *data)
{
    return (data[0] + (data[1] << 8) + (data[2] << 16));
}

unsigned int strToUInt2Byte(unsigned char *data)
{
    return (data[0] + (data[1] << 8));
}

unsigned int strToUInt1Byte(unsigned char *data)
{
    return (data[0]);
}

unsigned int read4Byte(int fd, bool *result)
{

    *result = true;
    unsigned  char data[4];
    int size = read(fd, data, 4);
    if(size < 0){
        *result = false;
    }
    return (data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24));
}

unsigned int read3Byte(int fd, bool *result)
{
    *result = true;
    unsigned  char data[3];
    int size = read(fd, data, 3);
    if(size < 0){
        *result = false;
    }
    return (data[0] + (data[1] << 8) + (data[2] << 16));
}

unsigned int read2Byte(int fd, bool *result)
{
    *result = true;
    unsigned  char data[2];
    int size = read(fd, data, 2);
    if(size < 0){
        *result = false;
    }
    return (data[0] + (data[1] << 8));
}

unsigned int read1Byte(int fd, bool *result)
{
    *result = true;
    unsigned  char data[1];
    int size = read(fd, data, 1);
    if(size < 0){
        *result = false;
    }
    return (data[0]);
}


int cvt_hex_str_to_int_array(char *src, unsigned char *dst)
{
    int i=0;
    while(src[i]){
        char tmp[2];
        tmp[0] = src[i];
        tmp[1] = src[i+1];
        char *end;
        unsigned char decimal = strtol(tmp, &end, 16);
        dst[i/2] = decimal;
        i = i+2;
    }
    return (i/2);
}


size_t jumpImageAddress(unsigned char *img, size_t max_size)
{
    printf("jumpImageAddress\n");
    char *code = "00FFFBF000A0A0A40080808000FF00000000FF0000FFFF00000000FF00FF00FF0000FFFF00FFFFFF00";
    int size=41;
    //unsigned char read_byte[size];
    unsigned char header_byte[size];
    cvt_hex_str_to_int_array(code, header_byte);

    size_t i=0x2000;
    while(i < max_size){
        if(memcmp(&(img[i]), header_byte, size) == 0){
            return (i+size);
            //break;
        }
        i++;
    }
    return 0;
}
