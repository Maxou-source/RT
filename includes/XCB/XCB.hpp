#ifndef XCB_HPP
# define XCB_HPP

# include <iostream>
# include <stdbool.h>
# include <xcb/xcb.h>

# include "Image.hpp"

class XCB {
	private:
		// int					screen_number;
		xcb_connection_t	*connection;

		xcb_screen_t		*screen;

		xcb_window_t		window;

		xcb_gcontext_t		gc;

		xcb_format_t		format;
		Image				xcb_image;

	public:
		XCB();
		bool	setupConnection();
		bool	setupScreenAndFormat();

		bool	createWindow();

		bool	loop();

};

#endif