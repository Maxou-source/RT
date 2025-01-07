#include <string>
#include <iostream>
#include <stdlib.h>
#include "Sphere.hpp"
#include "parse.hpp"
#include "Ray.hpp"
// #include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
// #include <SDL2/SDL.h>
#include <xcb/xcb.h>
// #include <GL/glew.h>
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

int main(int ac, char **av)
{
	(void) ac;
	(void) av;
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
        400, 400,                // Width, height
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
				Tuple origin(0,0,-5,POINT);
				Tuple direction(0,0,0, VECTOR);
				Ray ray(origin, direction);
				Sphere sp;
				// float canvas_pixels = 400;
				float wall_z = 10;
				float wall_size = 7;
				float pixel_size = wall_size / 400;
				float half = wall_size /2;
				xcb_gcontext_t red_gc = xcb_generate_id(connection);
				for (int y = 0; y < 399; y++)
				{
					float world_y = half - pixel_size * y;
					for (int x = 0; x < 399; x++)
					{
						float world_x = -half - pixel_size * x;
						Tuple position(world_x, world_y, wall_z, POINT);
						// Ray r();
						Tuple tmp((position - ray.getOrigin()).normalize());
						ray.setDirection(tmp);
						if (sp.intersect(&ray))
						{
							uint32_t red_color = 0xff0000; // RGB for red
							uint32_t red_gc_values[] = {red_color, screen->white_pixel};
							xcb_create_gc(connection, red_gc, window, gc_mask, red_gc_values);

							xcb_point_t point = {static_cast<int16_t>(x), static_cast<int16_t>(y)};
							xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, window, red_gc, 1, &point);
						}
					}
				}

				std::cout << "im done" << std::endl;
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