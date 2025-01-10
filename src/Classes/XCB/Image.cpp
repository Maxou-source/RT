#include "Image.hpp"
#include <iostream>
#include <cstring> // For memcpy


/*===== Constructor and Destructors =====*/

Image::Image() {
	width = 0;
	height = 0;
	depth = 0;
	scanline_pad = 0;
	stride = 0;
	data = 0;
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

	// stride (supposing the bpp = scanline_pad)
	this->stride = (width * bpp) / 8;
	std::cout << stride << std::endl;
	this->total_size = stride * height;
	std::cout << "totalt size " << total_size << std::endl;
	this->data = new uint8_t[total_size];
	std::memset(data, 0, total_size);
}

Image::~Image() {
	// delete data;
}

/*===== Methods =====*/

void	Image::pixel_put(int x, int y, unsigned int color)
{
	uint8_t *  row = data + (y * stride);
	row[x << 2] = color;
	row[(x << 2) + 1] = color >> 8;
	row[(x << 2) + 2] = color >> 16;
	row[(x << 2) + 3] = color >> 24;
}

/*===== Getters and Setters ====*/

uint8_t*	Image::getImageData() { return data;}
int			Image::getTotalSize() { return total_size;}
uint16_t	Image::getWidth() { return width;}
uint16_t	Image::getHeight() { return height;}
