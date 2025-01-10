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

		uint32_t		stride; // calculated in bytes
		int			total_size;

		uint8_t		*data;
	public:
	// constructors and destructors 
		Image();
		~Image();
		Image(xcb_format_t *f, uint16_t width, uint16_t height);

	// methods
		void	pixel_put(int x, int y, unsigned int color);

	// getters and setters
		uint8_t*		getImageData();
		int				getTotalSize();
		uint16_t		getWidth();
		uint16_t		getHeight();

};

#endif