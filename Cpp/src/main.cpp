#include <string>
#include <iostream>
#include <stdlib.h>
#include "AObject.hpp"
#include "parse.hpp"
// #include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
// #include <SDL2/SDL.h>
#include <xcb/xcb.h>
#include "XCB.hpp"
// #include <GL/glew.h>
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

int main(int ac, char **av)
{
	// AObject c;
	(void) av;
	(void) ac;
	// if (ac != 2)
	// {
	// 	std::cerr << "error no input file" << std::endl;
	// 	return (EXIT_FAILURE);
	// }
	// if (Parse::parse(av[1]))
	// {
	// 	return (EXIT_FAILURE);
	// }
	std::cout << "test 1 2 3"<< std::endl;
    // Connect to the X server

    xcb_connection_t *connection = xcb_connect(NULL, NULL);
	std::cout << "test" << std::endl;
    if (xcb_connection_has_error(connection)) {
        fprintf(stderr, "Error: Unable to connect to the X server\n");
        return -1;
    }

    // Get the first screen
    const xcb_setup_t *setup = xcb_get_setup(connection);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    xcb_screen_t *screen = iter.data;

    // Create a window ID
    xcb_window_t window = xcb_generate_id(connection);

    // Set window attributes (background color and event masks)
    uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t value_list[] = {screen->white_pixel, XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS};

    // Create the window
	std::cout << "arrived at creating window function" << std::endl;
    xcb_create_window(
        connection,
        XCB_COPY_FROM_PARENT,    // Depth
        window,                  // Window ID
        screen->root,            // Parent window
        100, 100,                // x, y position
        WIN_WIDTH, WIN_HEIGHT,                // Width, height
        10,                      // Border width
        XCB_WINDOW_CLASS_INPUT_OUTPUT, // Window class
        screen->root_visual,     // Visual
        value_mask,              // Value mask
        value_list               // Value list
    );

    // Make the window visible
    xcb_map_window(connection, window);

    // Flush the commands to the X server
    xcb_flush(connection);

    printf("Window opened! Press any key to close.\n");

    xcb_gcontext_t gc = xcb_generate_id(connection);
    uint32_t gc_mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    uint32_t gc_values[] = {screen->black_pixel, screen->white_pixel};

    xcb_create_gc(
        connection,  // Connection to X server
        gc,          // GC ID
        window,      // Drawable (the window)
        gc_mask,     // Value mask
        gc_values    // Value list
    );

    // Main event loop
    xcb_generic_event_t *event;
    while ((event = xcb_wait_for_event(connection))) {
        switch (event->response_type & ~0x80) {
            case XCB_EXPOSE: {
                // Draw a red dot in the middle of the screen
                int center_x = 200;
                int center_y = 150;

                xcb_gcontext_t red_gc = xcb_generate_id(connection);
                uint32_t red_color = 0xff0000; // RGB for red
                uint32_t red_gc_values[] = {red_color, screen->white_pixel};
                xcb_create_gc(connection, red_gc, window, gc_mask, red_gc_values);

                xcb_point_t point = {static_cast<int16_t>(center_x), static_cast<int16_t>(center_y)};
                xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, window, red_gc, 1, &point);

                xcb_free_gc(connection, red_gc);
                xcb_flush(connection);
                break;
            }
            case XCB_KEY_PRESS:
                // Exit on key press
                printf("Key pressed, exiting...\n");
                free(event);
                xcb_disconnect(connection);
                return 0;
        }
        free(event);
    }

    // Clean up
    xcb_disconnect(connection);
    return 0;
}