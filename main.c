#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <time.h>

uint16_t window_width;
uint16_t window_height;
uint32_t *frame_buffer;
int time_passed; 
int frame_time = 33000000;
Bool pressed_esc = False;

struct timespec start, end;
struct timespec sleep_time;

typedef struct {
	uint16_t x; 
	uint16_t y;
} Sprite_Position;

static Sprite_Position warrior_pos;

void change_pixel_colour(uint16_t x, uint16_t y, uint32_t colour);
Bool is_sky(uint16_t x, uint16_t y);
Bool draw_sprite(uint16_t x_pos, uint16_t y_pos);
void move_sprite(Sprite_Position *sprite_pos, KeySym keysym);
void render();
uint8_t input_handling(Display *display, Window *window, XImage *img, XEvent *event);


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

// Let split into timer, render, input handling. 
	render();
	while(1){
		clock_gettime(CLOCK_MONOTONIC, &start); //  Got the start time here
																						
		if(XPending(display) > 0){ // XEventsQueued(display, QueuedAfterFlush) is identical to XPending(display)
				input_handling(display, &window , img, &event); 
				if(pressed_esc){
					free(frame_buffer);
					XCloseDisplay(display);	
				}
		}
		clock_gettime(CLOCK_MONOTONIC, &end);	

		// Got the end time Then we start a blocking loop that checks if the time has been 33.3 ms, if not it sleeps it and if so then it creates the frame buffer and displays it 
		time_passed = (end.tv_nsec/1000000)-(start.tv_nsec/1000000);
		if(time_passed < frame_time){
			sleep_time.tv_nsec = frame_time - time_passed;
			nanosleep(&sleep_time, NULL);
			time_passed = 0;
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
			sprite->y = sprite->y - 5;
			break;
		case XK_a:
			sprite->x = sprite->x - 5;
			break;
		case XK_d:
			sprite->x = sprite->x + 5;
			break;
		case XK_s:
			sprite->y = sprite->y + 5;
			break;
	}
	render();
}

uint8_t input_handling(Display *display, Window *window, XImage *img, XEvent *event){
			XNextEvent(display, event); // This is a blocking event so i need to figure out how to check whether somehow if there is an event queued and then perform it and move on. 
			switch (event->type) {
				case KeyPress:
					if (XLookupKeysym(&event->xkey, 0) != XK_Escape )  {
							move_sprite(&warrior_pos, XLookupKeysym(&event->xkey, 0)); 
							XSync(display, True);
					}else{
								pressed_esc = True;
								XSync(display, True);
								return 0;
					}
					XPutImage(display, *window, XDefaultGC(display, 0), img, 0, 0, 0, 0, window_width, window_height);	
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
