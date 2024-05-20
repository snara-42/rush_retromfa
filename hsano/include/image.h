#ifndef IMAGE_H
#define IMAGE_H
#include "mfa.h"
#include <unistd.h>

unsigned char *get_image(int fd, t_mfa_img *img_info);
int drawThumbnail(char *path, t_mfa *mfa);
void drawIcon(int fd, t_mfa *mfa);
void drawIcon2(int fd, t_mfa *mfa);
void drawLightBall(int fd, t_mfa *mfa);
void drawLightBall2(int fd, t_mfa *mfa);
size_t drawSomething(unsigned char *img, size_t img_index, t_mfa *mfa, int offset_x, int offset_y, t_mfa_img *img_info);
void mapping(unsigned char *raw_img, t_mfa *mfa, int x_len, int y_len, int win_x, int win_y, bool reverse, t_mfa_img *img_info);
void mapping2(unsigned char *raw_img, t_mfa *mfa, int x_len, int y_len, int win_x, int win_y, bool reverse, t_mfa_img *img_info);
void mapping3(unsigned char *raw_img, t_mfa *mfa, int x_len, int y_len, int win_x, int win_y, bool reverse, t_mfa_img *img_info);
void mapping4(unsigned char *raw_img, t_mfa *mfa, int x_len, int y_len, int win_x, int win_y, bool reverse, t_mfa_img *img_info);

#endif
