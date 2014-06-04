#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <linux/input.h>

#include "font.c"

short rgb16(float r, float g, float b)
{
	char red = (char)(r * 31);
	char green = (char)(g * 63);
	char blue = (char)(b * 31);

	short ret = (red << 11) | (green << 5) | blue;

	return ret;
}
void drawRect(short* fb, int x, int y, int w, int h, short color)
{

	if(x < 0 || y < 0)
		return;

	for(int cx = x; cx < x + w && cx < 800; cx++)
	{
		for(int cy = y; cy < y + h && cy < 480; cy++)
		{
			fb[(cx + cy * 800)] = color;
		}
	}

}

void drawChar(short* fbp, char code, int x, int y,int size, short color)
{
	short c= 0;
	int sx = code % 16 * 8;
	int sy = code / 16 * 12;

	for(int cx = 0; cx < 8; cx++)
	{
		for(int cy = 0; cy < 12; cy++)
		{
			c = 0;
			c |= (short)font.pixel_data[(cx + sx + (cy+sy) * font.width) * 2];
			c |= (short)font.pixel_data[(cx + sx + (cy+sy) * font.width) * 2 + 1] << 8;
			
			if(c == (short)0xffff)
				drawRect(fbp,x + cx * size,y + cy * size, size,size,color);

		}
	}

}

int main()
{
	int fbfd = 0;
	struct fb_var_screeninfo var_info;
	struct fb_fix_screeninfo f_info;
	long int screensize = 0;
	short* fbp = 0;

	int leftScore = 0;
	int rightScore = 0; 

	fbfd = open("/dev/fb1", O_RDWR);
	if(fbfd == -1)
	{
		printf("Error: cannot open framebuffer device /dev/fb1\n");
		return 1;
	}

	printf("Framebuffer device /dev/fb1 is open.\n");

	if(ioctl(fbfd, FBIOGET_FSCREENINFO, &f_info))
	{
		printf("Error reading fixed info.\n");
	}

	if(ioctl(fbfd,FBIOGET_VSCREENINFO, &var_info))
	{
		printf("Error reading screen info.\n");
	}

	printf("Display info %dx%d, %d bpp\n", var_info.xres, var_info.yres, var_info.bits_per_pixel);


	screensize = f_info.smem_len;

	fbp = (short*)mmap(0,screensize,PROT_READ | PROT_WRITE, MAP_SHARED,fbfd,0);

	if((int)fbp == -1)
	{
		printf("Failed to mmap.\n");
	}
	else
	{
		int msfd = 0;
		struct input_event ie;

		if((msfd = open("/dev/input/mouse1",O_RDONLY)) == -1)
		{
			printf("Error opening /dev/input/mouse1\n");
		}
		else
		{
			printf("fbp: %d\n",fbp);

			while(read(msfd, &ie, sizeof(struct input_event)))
			{
				printf("type %d\tcode %d\tvalue %d\n",ie.type,ie.code,ie.value);

				drawRect(fbp,0,0,800,480,rgb16(0.39f,0.54f,0.92f));
				drawRect(fbp,380,0,40,480,rgb16(1,1,1));

				char buf[50];
				sprintf(buf,"%2d",leftScore);

				drawChar(fbp,buf[0],40,100,20,rgb16(1,1,1));
				drawChar(fbp,buf[1],200,100,20,rgb16(1,1,1));
		
				sprintf(buf,"%2d",rightScore);

				drawChar(fbp,buf[0],480,100,20,rgb16(1,1,1));
				drawChar(fbp,buf[1],640,100,20,rgb16(1,1,1));


			}

		}
	}


	munmap(fbp, screensize);
	close(fbfd);


	return 0;
}
