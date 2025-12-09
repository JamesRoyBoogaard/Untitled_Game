#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xlib.h>


int main(){
	printf("lets no do it ourselves!!");
	Display *display = XOpenDisplay(NULL); 
	if(!display){
		fprintf(stderr, "No display server could be found");
	}else{
		printf("%p is the address", display);
	}	
	uint16_t window_width = 500;
	uint16_t window_height = 500;
	uint8_t border_width = 10;
	Window window = XCreateSimpleWindow(display,XDefaultRootWindow(display),50,50, window_width, window_height, border_width, XBlackPixel(display, 0), XWhitePixel(display, 0));
	XMapWindow(display,window);
	XSelectInput(display, window, ExposureMask);


	return 1;
}
