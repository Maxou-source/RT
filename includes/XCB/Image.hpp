#ifndef IMAGE_HPP
# define IMAGE_HPP

# include <stdint.h>
# include <xcb/xcb.h>
/*

*/

class Image {
	private:
		uint16_t	width;
		uint16_t	height;
		// xcb_format_t	format;
		// we will always want a PIXMAP
		uint8_t		depth; // this will be a fix value of 32 bits
		uint8_t		scanline_pad;
		uint8_t		bpp;
	public:
		Image();
		Image(xcb_format_t *f, uint16_t width, uint16_t height);

};

#endif