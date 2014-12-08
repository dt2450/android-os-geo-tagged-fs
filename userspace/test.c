#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>

int main(int argc, char **argv)
{
	int ret;

	if (argc == 2) {
		unsigned int addr = 0;
		char *ptr;

		addr = strtoul(argv[1], &ptr, 10);
		printf("Address is: 0x%x\n", addr);
		printf("Address is: %s\n", argv[1]);
		ret = syscall(380, addr);
	} else
		ret = syscall(380, 0);

	printf("Ret: %d\n", ret);

	return 0;

}

