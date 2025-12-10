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
	
	XImage *img = XCreateImage(
    display,
    DefaultVisual(display, 0),   // just default visual
    DefaultDepth(display, 0),    // just default depth 
    ZPixmap,                     // format (for colour this is the best)
    0,                            // offset
    (char*)frame_buffer,         // the framebuffer
    window_width, window_height, // width & height
    32,                           // bitmap_pad (32 bits per scanline)
    window_width * 4              // bytes per line (apparently also can leave it with 0 and x11 computes it)
);


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
