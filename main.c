#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // 1. Open connection to X server
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Cannot open X display\n");
        exit(1);
    }

    // 2. Get default screen and root window
    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    // 3. Create the window
    Window win = XCreateSimpleWindow(
        display, root,
        100, 100,          // x, y position
        800, 600,          // width, height
        1,                 // border width
        BlackPixel(display, screen),
        WhitePixel(display, screen)
    );

    // 4. Select input events you care about
    XSelectInput(display, win, ExposureMask | KeyPressMask | StructureNotifyMask);

    // 5. Map the window to the screen (show it)
    XMapWindow(display, win);

    // 6. Event loop
    XEvent event;
    while (1) {
        XNextEvent(display, &event); // blocks until an event arrives

        switch (event.type) {
            case Expose:
                // Window needs redrawing
                printf("Expose event\n");
                break;

            case KeyPress:
                // Any key press exits the loop
                printf("Key pressed! Exiting.\n");
                goto cleanup;

            case DestroyNotify:
                // Window was closed (e.g. WM close button)
                printf("Window closed.\n");
                goto cleanup;
        }
    }

cleanup:
    XDestroyWindow(display, win);
    XCloseDisplay(display);
    return 0;
}

