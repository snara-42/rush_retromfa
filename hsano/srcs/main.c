
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


int start_binary(unsigned char *init, char *path, int i )
{
    int size = 500;
    unsigned char test[size];

    int fd1 = open(path, O_RDONLY );
    if(fd1 < 0){
        exit(1);
    }

    int result = read(fd1, test, size);
    
    printf("check i=%d\n", i/2);
    for(int j=0;j<size;j++){
        int result_mem = memcmp( init, &(test[j]), i/2);

        if(result_mem == 0){
            printf("result_mem=%d\n", j);
            close(fd1);

            return (j);
        }
    }
    printf("Error:%s\n\n", path);
    close(fd1);
    return (-1);
}


int	main(int argc, char **argv)
{
    (void)argv;
    (void)argc;
    /*
    if(argc != 2){
        exit(1);
    }
    */
    char *path1 = "./MFA/blue.mfa";
    char *path2 = "./MFA/brown.mfa";
    char *path3 = "./MFA/gray.mfa";
    char *path4 = "./MFA/green.mfa";
    char *path5 = "./MFA/white1.mfa";
    char *path6 = "./MFA/white2.mfa";
    char *path7 = "./MFA/white3.mfa";
    char *path8 = "./MFA/white4.mfa";
    char *path9 = "./MFA/white5.mfa";

    char *start="1800002800000040000000300000000100100003000000001800000000000000000000000000000000000000f80000e00700001f000000";

    int i=0;
    char init_b[1000] = {0};
    while(start[i]){
        unsigned char tmp[2];
        tmp[0] = start[i];
        tmp[1] = start[i+1];
        char *end;
        unsigned char decimal = strtol(tmp, &end, 16);
        printf("decimal[%d]=%u\n", i/2, decimal);
        init_b[i/2] = decimal;
        i = i+2;
    }

    start_binary(init_b, path1, i);
    start_binary(init_b, path2, i);
    start_binary(init_b, path3, i);
    start_binary(init_b, path4, i);
    start_binary(init_b, path5, i);
    start_binary(init_b, path6, i);
    start_binary(init_b, path7, i);
    start_binary(init_b, path8, i);
    start_binary(init_b, path9, i);

	return (1);
}
