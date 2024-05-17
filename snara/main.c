#include "mlx.h"
#include <stdio.h>

int	main(int ac, char *arg[])
{
	if (ac != 2)
		return printf("usage: %s file\n", arg[0]), 0;
	FILE *fp = fopen(arg[1], "r");
	if (!fp)
		return printf("failed to open %s\n", arg[1]), 1;
}
