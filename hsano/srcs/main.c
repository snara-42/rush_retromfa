
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
//#include "window.h"

#include "mlx.h"
#include "mlx_int.h"
#include "mfa.h"

#include "utility.h"
#include "header.h"
#include "image.h"

#define ON_DESTROY (17)




void test1(int *fds)
{

    printf("fds[0]=%d\n", fds[0]);
    printf("fds[1]=%d\n", fds[1]);
    printf("fds[2]=%d\n", fds[2]);
    printf("fds[3]=%d\n", fds[3]);


    int cnt = 0;
    int diff_cnt = 0;
    //bool flag = false;
    while(1){
        //printf("No.1 cnt=%d\n", cnt);
        unsigned int tmp[10];
        bool result;
        for(int i=0;i<9;i++){
            tmp[i] =  read2Byte(fds[i], &result);
            if(cnt <= 2){
                printf("i=%d tmp[i]=%x\n", i, tmp[i]);
            }
        }
        //printf("No.2 cnt=%d\n", cnt);

        for(int i=1;i<9;i++){
            if(tmp[0] != tmp[i]){
                printf("i=%d\n",i);
                printf("cnt=%d\n", cnt);
                //printf("cnt=%d\n", cnt);
                //printf("cnt=%d\n", cnt);
                diff_cnt++;
                //flag = true;
                break;

            }
        }
        if(diff_cnt > 1){
            break;

        }
        cnt++;
    }

}


int start_binary(unsigned char *init, int len, char *path )
{
    struct stat fileInfo;
    
    if (stat(path, &fileInfo) != 0){
        return (-1);
    }
    size_t size = fileInfo.st_size;
    printf("file=%s, size=%zu\n", path, size);

    int fd1 = open(path, O_RDONLY );
    if(fd1 < 0){
        fprintf(stderr, "open error:%s" , path);
        exit(1);
    }
    

    char *mem = malloc(size);
    int result = read(fd1, mem, size);
    if(result < 0){
        fprintf(stderr, "read error:%s" , path);
        exit(1);
    }
    
    printf("check len=%d\n", len);
    //int j=0;
    for(size_t j=0;j<size;j++){
        int result_mem = memcmp( init, &(mem[j]), len);
        if(result_mem == 0){
            printf("result_mem=%zu\n", j);
            close(fd1);
            free(mem);
            return (int)(j+len);
        }
    }
    close(fd1);
    free(mem);
    fprintf(stderr, "invalid file :%s" , path);
    exit(1);
}

unsigned char *load_image(int offset, char *path, size_t *size_p)
{
    struct stat fileInfo;
    
    if (stat(path, &fileInfo) != 0){
        return (NULL);
    }
    size_t size = fileInfo.st_size;
    *size_p = size;
    printf("file=%s, size=%zu, offset=%d\n", path, size, offset);

    int fd1 = open(path, O_RDONLY );
    if(fd1 < 0){
        fprintf(stderr, "open error:%s" , path);
        exit(1);
    }
    

    unsigned char *mem = malloc(size);
    char dummy[offset];
    int result = read(fd1, dummy, offset);
    result = read(fd1, mem, size);
    if(result < 0){
        fprintf(stderr, "read error:%s" , path);
        exit(1);
    }
    return (mem);
}

int	close_mfa(t_mfa *mfa)
{
    (void)mfa;
    mlx_destroy_image(mfa->mlx, mfa->image->img);
    mlx_clear_window(mfa->mlx, mfa->window);
    mlx_destroy_window(mfa->mlx, mfa->window);
    //mlx_destroy_image(cub3d->mlx, walls->north->img);
	//clear_cub3d(mfa);
	exit(0);
}





int	main(int argc, char **argv)
{
    (void)argv;
    (void)argc;

    /*
    char *path1 = "./MFA/blue.mfa";
    char *path2 = "./MFA/brown.mfa";
    char *path3 = "./MFA/gray.mfa";
    char *path4 = "./MFA/green.mfa";
    char *path5 = "./MFA/white1.mfa";
    char *path6 = "./MFA/white2.mfa";
    char *path7 = "./MFA/white3.mfa";
    char *path8 = "./MFA/white4.mfa";
    char *path9 = "./MFA/white5.mfa";
    */
    //char *path3 = "./MFA/gray.mfa";
    char *path2 = "./MFA/white1.mfa";
    //char *path4 = "./MFA/green.mfa";
    char *path9 = "./MFA/white5.mfa";
    //char *path7 = "./MFA/green.mfa";
    char *path = path2;
    //char *path = path9;


    t_mfa mfa ={0};
	mfa.mlx = mlx_init();
	if (!mfa.mlx){
        fprintf(stderr, "Error: mlx init\n");
        exit(1);
    }
    (void)mfa;
    mfa.width = 800;
    mfa.height = 800;

	mfa.window = mlx_new_window(mfa.mlx, mfa.width, mfa.height, "retromfa");
    if(!mfa.window){
        fprintf(stderr, "Error: new window\n");
        exit(1);
    }

    //void	*img;
    t_image tmp_image = {0};
    mfa.image = &tmp_image;
    /*
    mfa.image->width = 600;
    mfa.image->height = 600;
	mfa.image->img = (void *)mlx_new_image(mfa.mlx, mfa.image->width, mfa.image->height);
    if(!mfa.image->img){
        fprintf(stderr, "Error: new image\n");
        exit(1);
    }
	mfa.image->addr = mlx_get_data_addr(mfa.image->img, \
			&mfa.image->bpp, &mfa.image->sl, &mfa.image->endian);


    t_image sample = {0};
    char *sample_img_path = "./koto_east.xpm";
	sample.img = mlx_xpm_file_to_image(mfa.mlx, sample_img_path \
									, &(sample.width), &(sample.height));
    printf("sample.width=%d, hieght=%d\n", sample.width, sample.height);
	if (!sample.img){
        fprintf(stderr, "Error: open image %s\n", sample_img_path);
        exit(1);
    }
	sample.addr = mlx_get_data_addr(sample.img, &(sample.bpp) \
										, &(sample.sl), &(sample.endian));

                                        */

    /*
	mlx_put_image_to_window(mfa.mlx, mfa.window, \
								sample.img, 0, 0);
    */

    /*
    size_t size=0;
    int start_b = 0;
    unsigned char *raw_img = load_image(start_b, path, &size);
    printf("raw_img[0]=%u\n", raw_img[0]);
    printf("raw_img[1]=%u\n", raw_img[1]);
    printf("raw_img[2]=%u\n", raw_img[2]);
    printf("img size=%zu\n", size);


    if(argc != 2){
        exit(1);
    }
    */

    /*
    int fds[10];
    fds[0] = drawThumbnail(path1,&mfa);
    fds[1] = drawThumbnail(path2,&mfa);
    fds[2] = drawThumbnail(path3,&mfa);
    fds[3] = drawThumbnail(path4,&mfa);
    fds[4] = drawThumbnail(path5,&mfa);
    fds[5] = drawThumbnail(path6,&mfa);
    fds[6] = drawThumbnail(path7,&mfa);
    fds[7] = drawThumbnail(path8,&mfa);
    fds[8] = drawThumbnail(path9,&mfa);
    test1(fds);
    */

    int fd = drawThumbnail(path,&mfa);

    unsigned char tmp_read[1200];
    // 37はヘッダーバイト数
    // 530は実測値
    read(fd, tmp_read, 530*2 + 38);
    if(tmp_read[1060] != 0xC7){
        drawIcon(fd, &mfa);
        drawLightBall(fd, &mfa);
        drawIcon2(fd, &mfa);
        drawLightBall2(fd, &mfa);
    }
    drawSomething(fd, &mfa);

    /*
    t_mfa_img img_info;
    img_info.width= 48;
    img_info.height= 96;
    img_info.size = img_info.width * img_info.height;
    img_info.reverse = false;
    img_info.rgb = 24;


    char tmp_read[50];
    read(fds[8], tmp_read, 38);
    unsigned char *raw_img = get_image(fds[8], &img_info);
    mapping(raw_img, &mfa, 48,   48, 100 ,0, false, &img_info);
    */


    t_mfa_img img_info;
    img_info.width= 320;
    img_info.height= 320;
    img_info.size = img_info.width * img_info.height;
    img_info.reverse = false;
    img_info.rgb = 24;

    unsigned char *raw_img = get_image(fd, &img_info);


    int fd9 = open(path9 , O_RDONLY);

    int tmp_size = 134210+6+100 + 616;
    char tmp[tmp_size];
    read(fd9, tmp, tmp_size);

    img_info.width= 39*3;
    img_info.height= 280;
    img_info.size = img_info.width * img_info.height;
    img_info.reverse = false;
    img_info.rgb = 24;
    unsigned char *raw_img2 = get_image(fd9, &img_info);

    //raw_img = get_image(fds[8], &img_info);
    //raw_img = get_image(fds[8], &img_info);
    //raw_img = get_image(fds[8], &img_info);
    //exit(0);



    //char *img10 = analayze(path10);

    /*
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
    char *id1 = get_header(fd, 4);
    printf("id1=[%s]\n", id1);

    // must free id2
    char *id2 = get_header(fd, 4);
    printf("id2=[%s]\n", id2);
    char *read_path = get_header(fd, 4);
    printf("path=[%s]\n", read_path);
    char *info = get_header(fd, 1);
    printf("path=[%s]\n", info);


    if(!id1 || !id2 | !path){
        fprintf(stderr, "Error:read ID\n");
        exit(1);
    }
    t_mfa_img img_info;
    analyze_header_image(info, &img_info);

    //bool tmp_result;
    //　なぜか3バイトのデータがあるので読み込み
    //read3Byte(fd, &tmp_result);

    //must free img;
    unsigned char *img = get_image(fd, &img_info);
    (void)img;
    mapping(img, &mfa, img_info.width, img_info.height, 0 ,0,  img_info.reverse);
    */

    /*
    free(id1);
    free(id2);
    free(img);
    */
    //exit(1);


    //char *start="1800002800000040000000300000000100100003000000001800000000000000000000000000000000000000f80000e00700001f000000";
    //unsigned int init_b[1000] = {0};
    //int len = cvt_hex_str_to_int_array(mfa_init, init_b);

    //(void)len;
    //(void)start;
    //todo will remove

    //int len = i/2;
    //
    /*
    unsigned int init_b[1000] = {0};
    init_b[0]=0x27;
    init_b[1]=0x00;
    init_b[2]=0x78;
    init_b[3]=0x00;
    init_b[4]=0x06;
    init_b[5]=0x10;
    init_b[6]=0x00;
    init_b[7]=0x00;
    int start_b = start_binary(init_b, len, path9);
    printf("start_b=%d\n", start_b);
    int id_len = start_b - len - 16;
    printf("id_len=%d\n", id_len);
    */

    //char id[1000];

    //mapping(raw_img, &mfa, 16,   256, 0   ,0);
    //mapping(raw_img, &mfa, 32,   256, 100 ,0);
    //mapping(raw_img, &mfa, 64,   256, 200 ,0);

    /*
    mapping(raw_img, &mfa, 18,   256, 0   ,100, false, &img_info);
    mapping(raw_img, &mfa, 24,   256, 100 ,100, false, &img_info);
    mapping(raw_img, &mfa, 39,   256, 200 ,100, false, &img_info);
    mapping(raw_img, &mfa, 48,   256, 300 ,100, false, &img_info);
    mapping(raw_img, &mfa, 96,   256, 400 ,100, false, &img_info);
    mapping(raw_img, &mfa, 128,  256, 500 ,100, false, &img_info);
    */
    mapping(raw_img, &mfa, 1,  2, 500 ,100, false, &img_info);
    mapping3(raw_img2, &mfa, 39,  260, 300 ,0, false, &img_info);

    //mapping(raw_img, &mfa, 128,  256, 300 ,0);
    //mapping(raw_img, &mfa, 128, 256, 200 ,0);
    //mapping(raw_img, &mfa, 256, 256, 400 ,0);

    //mapping(raw_img, &mfa, 512,  256, 0   ,300);
    //mapping(raw_img, &mfa, 64,  256, 100 ,300);
    //mapping(raw_img, &mfa, 128, 256, 200 ,300);
    //mapping(raw_img, &mfa, 256, 256, 400 ,300);

	mlx_hook(mfa.window, ON_DESTROY, NoEventMask, close_mfa, &mfa);
    mlx_loop(mfa.mlx);

	return (1);
}
