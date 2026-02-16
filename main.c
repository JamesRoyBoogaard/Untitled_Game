#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <time.h>
#include <stdbool.h>

uint16_t window_width;
uint16_t window_height;
uint32_t *frame_buffer;
uint32_t *keys_pressed_list;
int time_passed; 
int frame_time = 30;

int  keys_pressed_list_size = 4;

struct timespec start, end;
struct timespec sleep_time;

typedef struct {
	float x; 
	float y;
} Sprite_Position;

static Sprite_Position warrior_pos;

void change_pixel_colour(uint16_t x, uint16_t y, uint32_t colour);
Bool is_sky(uint16_t x, uint16_t y);
Bool draw_sprite(uint16_t x_pos, uint16_t y_pos);
void move_sprite(Sprite_Position *sprite_pos, KeySym keysym);
void stop_moving_sprite(Sprite_Position *sprite_pos, KeySym keysym);
void render();
uint8_t input_handling(Display *display, Window *window, XImage *img, XEvent *event);
void move(Sprite_Position *sprite_pos);

int main(){
	Display *display = XOpenDisplay(NULL); 
	//XSetCloseDownMode(display, );
	if(!display){
		fprintf(stderr, "No display server could be found");
	}
	XEvent event;
	window_width = 500;
	window_height = 500;
	uint8_t border_width = 1;

	keys_pressed_list = malloc(keys_pressed_list_size * sizeof(Bool));
	for(int key = 0; key < keys_pressed_list_size; key++){
		keys_pressed_list[key] = False;
	}	

	Window window = XCreateSimpleWindow(display,XDefaultRootWindow(display),50,50, window_width, window_height, border_width, XBlackPixel(display, 0), XWhitePixel(display, 0));
	XMapWindow(display,window);
	XSelectInput(display, window, KeyPressMask|ExposureMask|KeyReleaseMask);

	frame_buffer = malloc(window_width*window_height*4);
	
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

	render();
	while(1){
		clock_gettime(CLOCK_MONOTONIC, &start); //  Got the start time here
		// Gather input
		while(XPending(display) > 0){
			input_handling(display, &window , img, &event); // Gain a list of inputs 
		}
		// Execute input
		move(&warrior_pos);
		// Handle frame timing
		clock_gettime(CLOCK_MONOTONIC,&end);	
		time_passed = (end.tv_nsec/1000000) - (start.tv_nsec/1000000);
		if(time_passed<frame_time*1000000){
			sleep_time.tv_nsec = frame_time - time_passed;
			nanosleep(&sleep_time, NULL);
			time_passed = 0;
		}
		// draw the frame
		render();
		XPutImage(display, window, XDefaultGC(display, 0), img, 0, 0, 0, 0, window_width, window_height);	
		}
		return 1; 
	}

	void change_pixel_colour(uint16_t x, uint16_t y, uint32_t colour){
		frame_buffer[y*window_width+x] = colour;
	}

void render(){
	for(uint16_t x = 0; x < window_width; x++){
		for(uint16_t y = 0; y < window_height; y++){
			if(draw_sprite(x,y)){
				change_pixel_colour(x , y, 0xFF00FF00);
		}else{
				change_pixel_colour(x,y, 0x00000000);
			}
		}
	}
}

void move_sprite(Sprite_Position *sprite, KeySym keysym){	
	switch (keysym){
		case XK_w:
			keys_pressed_list[0] = True;
			break;
		case XK_a:
			keys_pressed_list[1] = True;
			break;
		case XK_s:
			keys_pressed_list[2] = True;
			break;
		case XK_d:
			keys_pressed_list[3] = True;
			break;
	}
}

void move(Sprite_Position *sprite){
	float net_distance_sprite_x = 0;
	float net_distance_sprite_y = 0;
	for (int i = 0; i < keys_pressed_list_size; i++){
		switch(i){
			case 0 :
				if(keys_pressed_list[i]){
					net_distance_sprite_y-=2;
					break;	
				}
			case 1:
				//sprite->x = sprite-> x - 2;
				if(keys_pressed_list[i]){
					net_distance_sprite_x-=2;
					break;
				}
			case 2: 
				//sprite->y = sprite-> y + 2;
				if(keys_pressed_list[i]){
					net_distance_sprite_y+=2;
					break;
				}
			case 3:
				//sprite->x = sprite-> x + 2;
				if(keys_pressed_list[i]){
					net_distance_sprite_x+=2;
					break;
				}
		}
	}
	sprite->x += net_distance_sprite_x;
	sprite->y += net_distance_sprite_y; 
}

void stop_moving_sprite(Sprite_Position *sprite, KeySym keysym){
	switch (keysym){
		case XK_w:
			keys_pressed_list[0] = False;
			break;
		case XK_a:
			keys_pressed_list[1] = False;
			break;
		case XK_s:
			keys_pressed_list[2] = False;
			break;
		case XK_d:
			keys_pressed_list[3] = False;
			break;
	}
}

uint8_t input_handling(Display *display, Window *window, XImage *img, XEvent *event){
			XNextEvent(display, event); 
			switch (event->type) {
				case KeyPress:
					if (XLookupKeysym(&event->xkey, 0) != XK_Escape )  {
						move_sprite(&warrior_pos, XLookupKeysym(&event->xkey, 0));
					}else{ 
						free(keys_pressed_list);
						free(frame_buffer);
						XCloseDisplay(display);	
						return 0;
					}
					break;
				case KeyRelease:
					stop_moving_sprite(&warrior_pos, XLookupKeysym(&event->xkey, 0));
					break;
				case Expose:
					XPutImage(display, *window, XDefaultGC(display, 0), img, 0, 0, 0, 0, window_width, window_height);	
					break;
			}
			return 0;
}

Bool draw_sprite(uint16_t x_pos, uint16_t y_pos){ 
	// There is a current x and y position for the sprite which all other pixels in the sprite then are referenced from.
	static Bool initialised = False;
	uint16_t warrior_height = 50;
	uint16_t warrior_width = 50;
	if(!initialised){
		warrior_pos.x = 300;
		warrior_pos.y = 200;
		initialised = True;
	}

	uint32_t colour = 0xFF00FF00;
	// Essentially going to check if the current pixel is within the sprite or not. Then i guess we draw the sprite
	if(x_pos >= warrior_pos.x && x_pos <= warrior_pos.x + warrior_width 
			&& y_pos<= warrior_pos.y && y_pos >= warrior_pos.y - warrior_height){
			return True;
	}
	//Also need to account for the warrior_pos x and y never being less than 50 pixel from the border (within our 500X500 block/frame)
	return False;
}
