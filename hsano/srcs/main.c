
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
	exit(0);
}





int	main(int argc, char **argv)
{
    (void)argv;
    (void)argc;
    if(argc != 2){
        exit(1);

    }

    char *path = argv[1];


    t_mfa mfa ={0};
	mfa.mlx = mlx_init();
	if (!mfa.mlx){
        fprintf(stderr, "Error: mlx init\n");
        exit(1);
    }
    (void)mfa;
    mfa.width = 4000;
    mfa.height = 800;

	mfa.window = mlx_new_window(mfa.mlx, mfa.width, mfa.height, "retromfa");
    if(!mfa.window){
        fprintf(stderr, "Error: new window\n");
        exit(1);
    }



    int fd = drawThumbnail(path,&mfa);

    unsigned char tmp_read[12000];
    // 37はヘッダーバイト数
    // 530は実測値
    read(fd, tmp_read, 530*2 + 38);
    if(tmp_read[1060] != 0xC7){
        drawIcon(fd, &mfa);
        drawLightBall(fd, &mfa);
        drawIcon2(fd, &mfa);
        drawLightBall2(fd, &mfa);
    }
    close(fd);

    size_t img_size;
    unsigned char *raw_img = load_image(0, path, &img_size);
    size_t img_index = jumpImageAddress(raw_img, img_size);
    img_index += 16;
    if(img_index > 0){
        printf("start\n");
    
        int x = 300;
        int y=0;
        unsigned int y_max = 70;
        int cnt = 0;
        while(img_index < img_size){
            printf("img_index=%lx, img_size=%lx\n", img_index, img_size);
            cnt++;
            //if(cnt > 6){
                //break;
            //}
            t_mfa_img img_info;
            img_index =  drawSomething(raw_img, img_index, &mfa, x, y, &img_info);
            x += img_info.width + 20;
            if(y_max < img_info.height){
                y_max = img_info.height+5;
            }
            if(mfa.width < x + 150){
                x = 0;
                y += y_max;
                y_max = 0;
            }
            if(y > mfa.height){
                break;
            }
        }
        printf("end loop\n");
    }

	mlx_hook(mfa.window, ON_DESTROY, NoEventMask, close_mfa, &mfa);
    mlx_loop(mfa.mlx);

	return (1);
}
