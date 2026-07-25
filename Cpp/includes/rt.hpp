#ifndef RT_HPP
# define RT_HPP

# include <fstream>
# include <string>
# include <iostream>
# include <vector>
# include <array>

#define WIN_HEIGHT 400
#define WIN_WIDTH 400


# define EPSILON 0.00001

typedef float	t_f4 __attribute__((ext_vector_type(4)));

# define EPSILON 0.00001
# define PI 3.141592653589793

# define POINT 1
# define VECTOR 0

# define BPP 32
# define SCANLINE_PAD 32

typedef struct t_clr {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} t_clr;
// chaque composant de couleur r, g ou b ne peut contenir que 8 bits
// avec chaque valeur compris entre 0 et 255 donc un UNSIGNED CHAR pas un INT Jean Marc

#endif