#ifndef XCB_HPP
# define XCB_HPP

# include <iostream>
# include <stdbool.h>
# include <xcb/xcb.h>

# include "Image.hpp"

class XCB {
	public:
	// int					screen_number;
		xcb_connection_t	*connection;

		xcb_screen_t		*screen;

		xcb_window_t		window;

		xcb_gcontext_t		gc;

		xcb_format_t		format;
		Image				xcb_image;

	public:
	// constructors and destructors
		XCB();
	// methods
		bool	setupConnection();
		bool	setupScreenAndFormat();

		bool	createWindowAndGC();

		bool	loop();

	// setters and getters
		xcb_format_t*	getFormatPtr(void);

		void			setImage(Image &);
};

#endif