#ifndef RT_HPP
# define RT_HPP

# include <fstream>
# include <algorithm>
# include <sstream>
# include <string>
# include <iostream>
# include <vector>
# include <array>

#define WIN_HEIGHT 800
#define WIN_WIDTH 1000

namespace colorprint {
    constexpr const char* reset  = "\033[0m";
    constexpr const char* red    = "\033[31m";
    constexpr const char* green  = "\033[32m";
    constexpr const char* yellow = "\033[33m";
    constexpr const char* blue   = "\033[34m";
}

typedef enum {
	NONE,
	SPHERE,
	CYLINDER,
	CAMERA
} ObjectId;


#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

# define EPSILON 0.00001

typedef float	t_f4 __attribute__((ext_vector_type(4)));

# define EPSILON 0.00001
# define PI 3.141592653589793

# define POINT 1
# define VECTOR 0

# define BPP 32
# define SCANLINE_PAD 32

typedef struct t_clr {
    float r;
    float g;
    float b;
} t_clr;

#endif