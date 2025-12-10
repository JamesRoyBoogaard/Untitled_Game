#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xlib.h>

uint16_t window_width;
uint16_t window_height;
uint32_t *frame_buffer;

void change_pixel_colour(uint16_t x, uint16_t y, uint32_t colour);
int main(){

	XEvent event;
	Display *display = XOpenDisplay(NULL); 
	if(!display){
		fprintf(stderr, "No display server could be found");
	}

	window_width = 500;
	window_height = 500;
	uint8_t border_width = 1;
	Window window = XCreateSimpleWindow(display,XDefaultRootWindow(display),50,50, window_width, window_height, border_width, XBlackPixel(display, 0), XWhitePixel(display, 0));
	XMapWindow(display,window);
	XSelectInput(display, window, KeyPressMask);

	frame_buffer = malloc(window_width*window_height*4);

	while(1){
		XNextEvent(display, &event);
		switch (event.type) {
			case KeyPress:
				XCloseDisplay(display);
				return 0;
		}
	}


	return 1;
	}
void change_pixel_colour(uint16_t x, uint16_t y, uint32_t colour){
		frame_buffer[y*window_width+x] = colour;	
}
