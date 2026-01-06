#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xlib.h>

uint16_t window_width;
uint16_t window_height;
uint32_t *frame_buffer;
uint32_t *mc_sprite;

struct sprite_position{
	uint16_t x; 
	uint16_t y;
}sprite_position;

struct sprite_position warrior_pos;

struct sprite{
	uint16_t height;
	uint16_t width;
	uint32_t colour;
	uint16_t x_pos;
	uint16_t y_pos;
};

void change_pixel_colour(uint16_t x, uint16_t y, uint32_t colour);
Bool is_sky(uint16_t x, uint16_t y);
Bool draw_sprite(uint16_t x_pos, uint16_t y_pos);

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
	XSelectInput(display, window, KeyPressMask|ExposureMask);

	frame_buffer = malloc(window_width*window_height*4);
	mc_sprite = malloc(32*32*4);	
	
	XImage *img = XCreateImage(
    display,
    DefaultVisual(display, 0),   // just default visual
    DefaultDepth(display, 0),    // just default depth 
    ZPixmap,                     // format (for colour this is the best)
    0,                            // offset
    (char*)frame_buffer,         // the framebuffer
    window_width, window_height, // width & height
    32,                           // bitmap_pad (32 bits per scanline
    window_width * 4              // bytes per line (apparently also can leave it with 0 and x11 computes it)
);


	for(uint16_t x = 0; x < window_width; x++){
		for(uint16_t y = 0; y < window_height; y++){

		if(draw_sprite(x,y)){

		}
		if(is_sky(x, y)){
			change_pixel_colour(x , y, 0xFF00FF00);
	
			}else{
				change_pixel_colour(x,y, 0x00000000);
			}
		}
		}

	while(1){
		XNextEvent(display, &event);
		switch (event.type) {
			case KeyPress:
				free(frame_buffer);
				XCloseDisplay(display);
				return 0;
			case Expose:
				XPutImage(display, window, XDefaultGC(display, 0), img, 0, 0, 0, 0, window_width, window_height);	
				break;
		}
	}
	return 1;
}

void change_pixel_colour(uint16_t x, uint16_t y, uint32_t colour){
		frame_buffer[y*window_width+x] = colour;	
}


Bool is_sky(uint16_t x, uint16_t y){
	Bool is_sky; 
	if(y<=250){
		return False;
	}else{
		return True;
	}
}



Bool draw_sprite(uint16_t x_pos, uint16_t y_pos){ 
	// There is a current x and y position for the sprite which all other pixels in the sprite then are referenced from.
	uint16_t height, width = 50;
	uint32_t colour = 0xFF00FF00;
	// Essentially going to check if the current pixel is within the sprite or not. Then i guess we draw the sprite
	if(warrior_pos.x == NULL){
		warrior_pos.x = 100;
		warrior_pos.y = 100;

	}

	if(x_pos){
		
	}
	return True;
}
