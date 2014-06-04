#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <linux/input.h>

#define MOUSEFILE "/dev/input/mice"

int main()
{
		int fd;
			struct input_event ie;

				if((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
							perror("opening device");
									exit(EXIT_FAILURE);
										}

					while(read(fd, &ie, sizeof(struct input_event))) {
								printf("type %d\tcode %d\tvalue %d\n",
												       ie.type, ie.code, ie.value);
									}

						return 0;
}
