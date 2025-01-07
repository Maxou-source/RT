#ifndef XCB_HPP
# define XCB_HPP

# include <iostream>
# include <stdbool.h>
# include <xcb/xcb.h>

class XCB {
	private:
		// int					screen_number;
		xcb_connection_t	*connection;

		xcb_screen_t		*screen;

		xcb_window_t		window;
	public:
		XCB();
		bool	setupConnection();
		bool	setupScreen();

		bool	createWindow();

		bool	loop();

};

#endif