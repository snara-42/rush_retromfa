

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utility.h"
#include "header.h"
bool check_first_id(int fd)
{
    char *mfa_init="4D4D46320400000003000000F8000000";
    int header_size = 16;
    unsigned int mfa_init_header[header_size];
    cvt_hex_str_to_int_array(mfa_init, mfa_init_header);

    (void)fd;
    char data[header_size];
    int result = read(fd, data, header_size);
    if(result < 0){
        return (false);
    }
    return(memcmp( data, mfa_init_header, header_size) == 0);
}

unsigned char *get_header(int fd, int byte_size)
{
    bool result;
    unsigned int header_size;
    if(byte_size == 1){
        header_size = read1Byte(fd, &result);
    }else if(byte_size == 2){
        header_size = read2Byte(fd, &result);
    }else if(byte_size == 3){
        header_size = read3Byte(fd, &result);
    }else if(byte_size == 4){
        header_size = read4Byte(fd, &result);
    }

    if(!result){
        fprintf(stderr, "Error: Read Error \n");
        return (NULL);
        //exit(1);
    }
    printf("header_size=%u\n", header_size);
    //char id1[header_size];
    unsigned char *id1 = malloc(header_size);
    if(!id1){
        fprintf(stderr, "Error: malloc\n");
        return (NULL);
    }
    int size = read(fd, id1, header_size);
    if(size < 0){
        fprintf(stderr, "Error:read\n");
        return (NULL);
    }
    return id1;
}


void analyze_header_image(unsigned char *byte_info, t_mfa_img *img_info)
{
    (void)img_info;
    //t_mfa_img img_info;
    
    int i=0;
    unsigned int data1 = strToUInt3Byte(&(byte_info[i]));
    i+=3;
    unsigned int data2 = strToUInt4Byte(&(byte_info[i]));
    i+=4;
    unsigned int data3 = strToUInt4Byte(&(byte_info[i]));
    i+=4;
    unsigned int data4 = strToUInt4Byte(&(byte_info[i]));
    i+=4;
    unsigned int data5 = strToUInt2Byte(&(byte_info[i]));
    i+=2;
    unsigned int data6 = strToUInt2Byte(&(byte_info[i]));
    i+=2;
    unsigned int data7 = strToUInt4Byte(&(byte_info[i]));
    i+=4;
    unsigned int data8 = strToUInt4Byte(&(byte_info[i]));
    i+=4;
    unsigned int data9 = strToUInt4Byte(&(byte_info[i]));
    i+=4;
    unsigned int data10 = strToUInt4Byte(&(byte_info[i]));
    i+=4;
    unsigned int data11 = strToUInt4Byte(&(byte_info[i]));
    i+=4;
    unsigned int data12 = strToUInt4Byte(&(byte_info[i]));
    i+=4;
    unsigned int data13 = strToUInt4Byte(&(byte_info[i]));
    i+=4;
    unsigned int data14 = strToUInt4Byte(&(byte_info[i]));
    i+=4;
    unsigned int data15 = strToUInt4Byte(&(byte_info[i]));
    //unsigned int img_size = data7;

    img_info->width= data3;
    img_info->height= data4;
    img_info->size = data8;
    img_info->reverse = data5 == 0 ? false : true;
    img_info->rgb = data6;

    printf("data1=%d, 0x%x\n", data1, data1);
    printf("data2=%d, 0x%x\n\n", data2, data2);
    printf("data3=%d, 0x%x\n\n", data3, data3);
    printf("data4=%d, 0x%x\n\n", data4, data4);
    printf("data5=%d, 0x%x\n\n", data5, data5);
    printf("data6=%d, 0x%x\n\n", data6, data6);
    printf("data7=%d, 0x%x\n\n", data7, data7);
    printf("data8=%d, 0x%x\n\n", data8, data8);
    printf("data9=%d, 0x%x\n\n", data9, data9);
    printf("data10=%d, 0x%x\n\n", data10, data10);
    printf("data11=%d, 0x%x\n\n", data11, data11);
    printf("data12=%d, 0x%x\n\n", data12, data12);
    printf("data13=%d, 0x%x\n\n", data13, data13);
    printf("data14=%d, 0x%x\n\n", data14, data14);
    printf("data15=%d, 0x%x\n\n", data15, data15);

    //return (t_mfa_img);
}

void analyze_header_image2(unsigned char *byte_info, t_mfa_img *img_info)
{
    (void)img_info;
    //t_mfa_img img_info;
    
    int i=0;
    unsigned int data1 = strToUInt3Byte(&(byte_info[i]));
    i+=3;
    unsigned int data2 = strToUInt4Byte(&(byte_info[i]));
    i+=4;
    unsigned int data3 = strToUInt4Byte(&(byte_info[i]));
    i+=4;
    unsigned int data4 = strToUInt2Byte(&(byte_info[i]));
    i+=2;
    unsigned int data5 = strToUInt2Byte(&(byte_info[i]));
    i+=2;
    unsigned int data6 = strToUInt1Byte(&(byte_info[i]));
    i+=1;
    unsigned int data7 = strToUInt2Byte(&(byte_info[i]));
    i+=2;
    //unsigned int data8 = strToUInt1Byte(&(byte_info[i]));
    //i+=1;

    //unsigned int data9 = strToUInt4Byte(&(byte_info[i]));
    //i+=4;
    //unsigned int data10 = strToUInt4Byte(&(byte_info[i]));
    //i+=4;

    img_info->size = data3;

    img_info->width= data4;
    img_info->height= data5;
    img_info->reverse = false;
    img_info->rgb = data7;

    printf("data1=%d, 0x%x\n", data1, data1);
    printf("data2=%d, 0x%x\n\n", data2, data2);
    printf("data3=%d, 0x%x\n\n", data3, data3);
    printf("data4=%d, 0x%x\n\n", data4, data4);
    printf("data5=%d, 0x%x\n\n", data5, data5);
    printf("data6=%d, 0x%x\n\n", data6, data6);
    printf("data7=%d, 0x%x\n\n", data7, data7);
    //printf("data8=%d, 0x%x\n\n", data8, data8);
    //printf("data9=%d, 0x%x\n\n", data9, data9);
    //printf("data10=%d, 0x%x\n\n", data10, data10);

    //return (t_mfa_img);
}
