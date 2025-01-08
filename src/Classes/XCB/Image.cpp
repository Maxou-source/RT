#include "Image.hpp"

Image::Image() {
	width = 0;
	height = 0;
	depth = 0;
	scanline_pad = 0;
	// format = 0;
}

Image::Image(xcb_format_t *f, uint16_t width, uint16_t height)
{
	// initializing format
	depth = f->depth;
	scanline_pad = f->scanline_pad;
	bpp = f->bits_per_pixel;

	// other stuff
	this->width = width;
	this->height = height;
}