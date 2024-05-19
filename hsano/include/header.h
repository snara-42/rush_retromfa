#ifndef HEADER_H
#define HEADER_H
#include "mfa.h"

bool check_first_id(int fd);
unsigned char *get_header(int fd, int byte_size);
void analyze_header_image(unsigned char *byte_info, t_mfa_img *img_info);
void analyze_header_image2(unsigned char *byte_info, t_mfa_img *img_info);

#endif
