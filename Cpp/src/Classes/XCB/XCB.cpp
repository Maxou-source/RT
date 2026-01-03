#include "XCB.hpp"
#include "Image.hpp"
#include "rt.hpp"
#include <unistd.h>

/*===== Construcors and Destructors =====*/
XCB::XCB() {
	gc = 0;
}

/*===== Methods =====*/

bool XCB::setupConnection() {
	connection = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(connection)) {
		std::cout << "Connection error" << std::endl;
		return true;
	}
	return false;
}


bool XCB::setupScreenAndFormat()
{
	const xcb_setup_t *setup = xcb_get_setup(connection);
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
	screen = iter.data;

	const xcb_format_t* pixmap_format = xcb_setup_pixmap_formats(setup);
	int num_formats = xcb_setup_pixmap_formats_length(setup);
	for (int i = 0; i < num_formats; i++)
	{
		if (pixmap_format[i].depth == screen->root_depth &&
			pixmap_format[i].bits_per_pixel == BPP
			&& pixmap_format[i].scanline_pad == SCANLINE_PAD)
		{
			format = pixmap_format[i];
			std::cout << "depth " << (int)pixmap_format[i].depth << std::endl;
			std::cout << "bits_per_pixel " << (int)pixmap_format[i].bits_per_pixel << std::endl;
			std::cout << "scanline_pad " << (int)pixmap_format[i].scanline_pad << std::endl;
			return false;
		}
	}
	std::cout << "ERROR : appropriate xcb_format not found" << std::endl;
	return (true);
}

bool XCB::createWindowAndGC()
{
	window = xcb_generate_id(connection);

	// Set window attributes (background color and event masks)
	uint32_t value_mask = XCB_CW_EVENT_MASK;
	uint32_t value_list[] = {XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS};

	// Create the window
	std::cout << "arrived at creating window function" << std::endl;
	xcb_create_window(
		connection,
		XCB_COPY_FROM_PARENT,	// Depth
		window,					// Window ID
		screen->root,			// Parent window
		0, 0,				// x, y position
		400, 400,				// Width, height
		0,						// Border width
		XCB_WINDOW_CLASS_INPUT_OUTPUT, // Window class
		screen->root_visual,	// Visual
		value_mask,				// Value mask
		value_list				// Value list
	);

	xcb_map_window(connection, window);
	xcb_flush(connection);

	gc = xcb_generate_id(connection);
	uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
	uint32_t values[2] = {screen->black_pixel, screen->white_pixel};
	xcb_create_gc(connection, gc, window, mask, values);
	return false;
}

bool XCB::loop() {
	xcb_generic_event_t *event;

	while ((event = xcb_wait_for_event(connection))) {
		switch (event->response_type & ~0x80) {
			case XCB_EXPOSE:
				std::cout << "XCB_EXPOSE event received!" << std::endl;
				// Draw the image
				xcb_put_image(
					connection,
					XCB_IMAGE_FORMAT_Z_PIXMAP,
					window,
					gc,
					xcb_image.getHeight(), xcb_image.getWidth(),			// Image dimensions
					0, 0,				// x, y
					0,				   // Left-pad
					screen->root_depth,  // Depth
					xcb_image.getTotalSize(),
					xcb_image.getImageData());
				// need to check return 
				xcb_flush(connection);
				break;

			case XCB_KEY_PRESS:
				std::cout << "Key pressed, exiting..." << std::endl;
				free(event);
				xcb_disconnect(connection);
				return false;

			default:
				std::cout << "Unhandled event: " << (event->response_type & ~0x80) << std::endl;
				break;
		}
		free(event);
	}
	xcb_disconnect(connection);
	return false;
}

/*===== Setters and Getters =====*/

xcb_format_t*	XCB::getFormatPtr(void)
{
	return (&format);
}

void		XCB::setImage(Image& img) {
	this->xcb_image = img;
}