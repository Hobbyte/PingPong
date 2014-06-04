#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>

int main()
{
	int fbfd = 0;
	struct fb_var_screeninfo var_info;

	fbfd = open("/dev/fb1", O_RDWR);
	if(fbfd == -1)
	{
		printf("Error: cannot open framebuffer device /dev/fb1\n");
		return 1;
	}

	printf("Framebuffer device /dev/fb1 is open.\n");

	if(ioctl(fbfd,FBIOGET_VSCREENINFO, &var_info))
	{
		printf("Error reading screen info.\n");
	}

	printf("Display info %dx%d, %d bpp\n", var_info.xres, var_info.yres, var_info.bits_per_pixel);

	close(fbfd);

	return 0;
}
