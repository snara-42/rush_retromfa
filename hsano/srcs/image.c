#include "image.h"
#include "utility.h"
#include "header.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "mlx.h"
#include "mlx_int.h"

void mapping(unsigned char *raw_img, t_mfa *mfa, int x_len, int y_len, int win_x, int win_y, bool reverse, t_mfa_img *img_info)
{
    (void)img_info;

    int size = img_info->size;
    printf("size=%d\n", img_info->size);
    printf("raw_img[0]=%x\n", raw_img[0]);
    printf("raw_img[1]=%x\n", raw_img[1]);
    printf("raw_img[2]=%x\n", raw_img[2]);
    printf("raw_img[3]=%x\n", raw_img[3]);

    //int size = mfa.height;
    printf("raw_img[-4]=%x\n", raw_img[size-4]);
    printf("raw_img[-3]=%x\n", raw_img[size-3]);
    printf("raw_img[-2]=%x\n", raw_img[size-2]);
    printf("raw_img[-1]=%x\n", raw_img[size-1]);
    t_image image = {0};
	image.img = (void *)mlx_new_image(mfa->mlx, x_len, y_len);
	image.addr = mlx_get_data_addr(image.img, \
			&image.bpp, &image.sl, &image.endian);

    int i = 0;
    for(int y=0;y<y_len;y++){
        for(int x=0;x<x_len;x++){
            unsigned char R = (raw_img[i+1] & 0xF8) ;
            unsigned char G = ((raw_img[i+1] & 0x07) << 5) + ((raw_img[i] & 0xE0) >> 5);
            unsigned char B = (raw_img[i] & 0x1F) << 3;

            if(raw_img[i] == 0xFF && raw_img[i+1] == 0xFF){
                R = 0xf3;
                G = 0xf3;
                B = 0xf3;
            }

            //if(y % 16 <= 2){
                //R = 255;
                //G = 0;
                //B = 0;
            //}
            unsigned int tmp_color = (R << 16) + (G << 8) + B;

            if(reverse){
                unsigned int *dst_ptr = image.addr + image.sl * (y_len - y -1);
                dst_ptr[x] = tmp_color;
            }else{
                unsigned int *dst_ptr = image.addr + image.sl * (y);
                dst_ptr[x] = tmp_color;
            }

            i += 2;
        }
    }
     
	mlx_put_image_to_window(mfa->mlx, mfa->window, \
								image.img, win_x, win_y);
}

void mapping2(unsigned char *raw_img, t_mfa *mfa, int x_len, int y_len, int win_x, int win_y, bool reverse, t_mfa_img *img_info)
{

    (void)reverse;
    (void)img_info;
    t_image image = {0};
	image.img = (void *)mlx_new_image(mfa->mlx, x_len, y_len);
	image.addr = mlx_get_data_addr(image.img, \
			&image.bpp, &image.sl, &image.endian);

    int i = 0;
    for(int y=0;y<y_len;y++){
        for(int x=0; x <x_len;x++){
            unsigned char R = raw_img[i+2];
            unsigned char G = raw_img[i+1];
            unsigned char B = raw_img[i];
            unsigned int tmp_color = (R << 16) + (G << 8) + B;
            unsigned int *dst_ptr = image.addr + image.sl * (y);
            dst_ptr[x] = tmp_color;
            i += 3;
        }
    }
     
	mlx_put_image_to_window(mfa->mlx, mfa->window, \
								image.img, win_x, win_y);
}

void mapping3(unsigned char *raw_img, t_mfa *mfa, int x_len, int y_len, int win_x, int win_y, bool reverse, t_mfa_img *img_info)
{
    (void)img_info;

    //int size = img_info->size;
    t_image image = {0};
	image.img = (void *)mlx_new_image(mfa->mlx, x_len, y_len);
	image.addr = mlx_get_data_addr(image.img, \
			&image.bpp, &image.sl, &image.endian);

    int i = 0;
    for(int y=0;y<y_len;y++){
        for(int x=0;x<x_len;x++){
            //unsigned char R = (raw_img[i+1] & 0xF8);
            //unsigned char G = ((raw_img[i+1] & 0x07) << 5) + ((raw_img[i] & 0xE0) >> 5);
            //unsigned char B = (raw_img[i] & 0x1F) << 3;

            /*
            unsigned char R = (raw_img[i+1] & 0x78)  << 1;
            unsigned char B = ((raw_img[i+1] & 0x03) << 6) + ((raw_img[i] & 0xE0) >> 5);
            unsigned char G = (raw_img[i] & 0x1F) << 3;
            */
            unsigned char R = (raw_img[i+1] & 0x7C)  << 1;
            unsigned char B = ((raw_img[i+1] & 0x03) << 6) + ((raw_img[i] & 0xE0) >> 5);
            unsigned char G = (raw_img[i] & 0x1F) << 3;
            if(i <= 6){
                printf("R=%u, G=%u, B=%u\n", R, G, B);
            }


            /*
            unsigned char G = (raw_img[i+1] & 0xF8);
            unsigned char B = ((raw_img[i+1] & 0x07) << 5) + ((raw_img[i] & 0xE0) >> 5);
            unsigned char R = (raw_img[i] & 0x1F) << 3;
            */
            //if(y % 16 <= 2){
                //R = 255;
                //G = 0;
                //B = 0;
            //}
            unsigned int tmp_color = (R << 16) + (G << 8) + B;

            if(reverse){
                unsigned int *dst_ptr = image.addr + image.sl * (y_len - y -1);
                dst_ptr[x] = tmp_color;
            }else{
                unsigned int *dst_ptr = image.addr + image.sl * (y);
                dst_ptr[x] = tmp_color;
            }

            i += 2;
        }
    }
     
	mlx_put_image_to_window(mfa->mlx, mfa->window, \
								image.img, win_x, win_y);
}

void mapping4(unsigned char *raw_img, t_mfa *mfa, int x_len, int y_len, int win_x, int win_y, bool reverse, t_mfa_img *img_info)
{
    (void)img_info;

    //int size = img_info->size;
    t_image image = {0};
	image.img = (void *)mlx_new_image(mfa->mlx, x_len, y_len);
	image.addr = mlx_get_data_addr(image.img, \
			&image.bpp, &image.sl, &image.endian);

    int i = 0;
    for(int y=0;y<y_len;y++){
        for(int x=0;x<x_len;x++){
            unsigned char R = (raw_img[i] & 0xE0);
            unsigned char G = ((raw_img[i] & 0x1C) << 3);
            unsigned char B = (raw_img[i] & 0x03) << 6;
            if(raw_img[i]  == 0xff){
                R = 0xf3;
                G = 0xf3;
                B = 0xf3;
            }
            //unsigned char R = (raw_img[i] & 0x30) << 2;
            //unsigned char G = ((raw_img[i] & 0x0C) << 4);
            //unsigned char B = (raw_img[i] & 0x03) << 6;

            if(i%10 == 0){
                printf("R=%u, G=%u, B=%u\n", R, G, B);
            }

            unsigned int tmp_color = (R << 16) + (G << 8) + B;

            if(reverse){
                unsigned int *dst_ptr = image.addr + image.sl * (y_len - y -1);
                dst_ptr[x] = tmp_color;
            }else{
                unsigned int *dst_ptr = image.addr + image.sl * (y);
                dst_ptr[x] = tmp_color;
            }

            i += 1;
        }
    }
     
	mlx_put_image_to_window(mfa->mlx, mfa->window, \
								image.img, win_x, win_y);
}



unsigned char *get_image(int fd, t_mfa_img *img_info)
{
    //bool reverse = img_info->reverse;

    unsigned char *img = malloc(img_info->size);
    if(!img){
        fprintf(stderr, "Error: malloc\n");
        return (NULL);
    }
    int result = read(fd, img, img_info->size);
    if(result < 0){
        free(img);
        fprintf(stderr, "Error: read\n");
        return (NULL);
    }
    return (img);
}


int drawThumbnail(char *path, t_mfa *mfa)
{
    int fd = open(path, O_RDONLY);
    if(fd < 0){
        fprintf(stderr, "Error: open file%s\n", path);
        exit(1);
    }

    if(check_first_id(fd) == false){
        fprintf(stderr, "Error: Not MFA file %s\n", path);
        exit(1);
    }
    

    // must free id1
    unsigned char *id1 = get_header(fd, 4);
    printf("id1=[%s]\n", id1);

    // must free id2
    unsigned char *id2 = get_header(fd, 4);
    printf("id2=[%s]\n", id2);
    unsigned char *read_path = get_header(fd, 4);
    printf("path=[%s]\n", read_path);
    unsigned char *info = get_header(fd, 1);
    printf("path=[%s]\n", info);


    if(!id1 || !id2 | !path){
        fprintf(stderr, "Error:read ID\n");
        exit(1);
    }
    t_mfa_img img_info;
    analyze_header_image(info, &img_info);

    bool tmp_result;
    //　なぜか3バイトのデータがあるので読み込み
    read3Byte(fd, &tmp_result);

    //must free img;
    unsigned char *img = get_image(fd, &img_info);
    (void)img;
    mapping(img, mfa, img_info.width, img_info.height, 0 ,0,  img_info.reverse, &img_info);
    // todo
    free(img);
    //return img;
    return fd;

}

void drawIcon(int fd, t_mfa *mfa)
{
    t_mfa_img img_info;
    img_info.width= 48;
    img_info.height= 48*2;
    img_info.size = img_info.width * img_info.height;
    img_info.reverse = false;
    img_info.rgb = 24;

    unsigned char *raw_img = get_image(fd, &img_info);
    mapping(raw_img, mfa, 48,   48, 80 ,0, false, &img_info);

}

void drawIcon2(int fd, t_mfa *mfa)
{
    t_mfa_img img_info;
    img_info.width= 32;
    img_info.height= 32*2;
    img_info.size = img_info.width * img_info.height;
    img_info.reverse = false;
    img_info.rgb = 24;


    unsigned char tmp_read[33];
    read(fd, tmp_read, 33);

    unsigned char *raw_img = get_image(fd, &img_info);
    mapping(raw_img, mfa, 32, 32, 200 ,0, false, &img_info);
}


void drawLightBall(int fd, t_mfa *mfa)
{
    t_mfa_img img_info;
    img_info.width= 48;
    img_info.height= 48;
    img_info.size = img_info.width * img_info.height;
    img_info.reverse = false;
    img_info.rgb = 24;


    unsigned char tmp_read[1];
    read(fd, tmp_read, 1);
    unsigned char *raw_img = get_image(fd, &img_info);
    mapping4(raw_img, mfa, 48, 48, 140 ,0, false, &img_info);

}

void drawLightBall2(int fd, t_mfa *mfa)
{
    t_mfa_img img_info;
    img_info.width= 32;
    img_info.height= 32;
    img_info.size = img_info.width * img_info.height;
    img_info.reverse = false;
    img_info.rgb = 24;


    unsigned char tmp_read[5];
    read(fd, tmp_read, 4);
    unsigned char *raw_img = get_image(fd, &img_info);
    mapping4(raw_img, mfa, 32, 32, 240 ,0, false, &img_info);
}


void drawIconInGreen(int fd, t_mfa *mfa)
{
    t_mfa_img img_info;
    img_info.width= 300;
    img_info.height= 300;
    img_info.size = img_info.width * img_info.height;
    img_info.reverse = false;
    img_info.rgb = 24;


    unsigned char tmp_read[1200];
    read(fd, tmp_read, 530*2 );
    printf("tmp_read=%x\n", tmp_read[1160]);
    printf("tmp_read=%x\n", tmp_read[1161]);
    printf("tmp_read=%x\n", tmp_read[1162]);
    printf("tmp_read=%x\n", tmp_read[1163]);
    printf("tmp_read=%x\n", tmp_read[1164]);
    printf("tmp_read=%x\n", tmp_read[1165]);
    printf("tmp_read=%x\n", tmp_read[1166]);
    printf("tmp_read=%x\n", tmp_read[1167]);
    //read(fd, tmp_read,  size);
    unsigned char *raw_img = get_image(fd, &img_info);
    mapping(raw_img, mfa, 100,   480, 180 ,0, false, &img_info);
    mapping(raw_img, mfa, 200,   480, 300 ,0, false, &img_info);

}

bool drawSomething(int fd, t_mfa *mfa, int offset_x, int offset_y, t_mfa_img *img_info)
{
    //t_mfa_img img_info;

    /*
    unsigned char tmp_read[10000];
    read(fd, tmp_read, 9000);
    read(fd, tmp_read, 9000);
    read(fd, tmp_read, 9000);
    read(fd, tmp_read, 9000);
    read(fd, tmp_read, 9000); //45000+7967=52967=0xCEE7
    read(fd, tmp_read, 7950);
    */



    unsigned char tmp_read[40];
    read(fd, tmp_read, 28);
    analyze_header_image2(tmp_read, img_info );
    if(img_info->width > 1000 || img_info->height > 1000 || img_info->width == 0 || img_info->height == 0 || img_info->size == 0){
        return false;
    }
    if(img_info->width * img_info->width  > img_info->size){
        return false;
    }
    //for(int i=0;i<28;i++){
        //printf("raw_img[%d]=%x\n", i, tmp_read[i]);
    //}
    //img_info.width= 800;
    //img_info.height= 800;
    //img_info.size = img_info.width * img_info.height;
    //img_info.reverse = false;
    //img_info.rgb = 24;
    printf("x=%u\n", img_info->width);
    printf("y=%u\n", img_info->height);

    /*
    img_info.width= 75;
    img_info.height= 400;
    img_info.size = img_info.width * img_info.height;
    img_info.reverse = false;
    img_info.rgb = 24;
    */


    unsigned char *raw_img = get_image(fd, img_info);

    //mapping2(raw_img, mfa, 76, 100,   offset_x,offset_y, false, &img_info);
    mapping2(raw_img, mfa, img_info->width+1, img_info->height,   offset_x,offset_y, false, img_info);

    //調整
    read(fd, tmp_read, 8);

    return true;
}
